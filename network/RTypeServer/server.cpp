// server.cpp
#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <thread>
#include <deque>
#include <string>
#include <vector>
#include <functional>
#include <chrono>
#include <ctime>

using boost::asio::ip::tcp;
namespace asio = boost::asio;

static std::string interpret_command(const std::string& line) {
    std::string cmd = line;
    if (!cmd.empty() && cmd.back() == '\r')
        cmd.pop_back();

    if (cmd == "PING") {
        return "PONG\n";
    } else if (cmd.rfind("ECHO ", 0) == 0) {
        return cmd.substr(5) + "\n";
    } else if (cmd == "TIME") {
        std::time_t t = std::time(nullptr);
        return std::string(std::ctime(&t)); // ctime includes newline
    } else if (cmd == "QUIT") {
        return "BYE\n";
    } else {
        return "UNKNOWN COMMAND\n";
    }
}

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket, asio::io_context& ioc)
    : socket_(std::move(socket)),
      strand_(asio::make_strand(ioc))
    {}

    void start() {
        do_read();
    }

    void stop() {
        asio::post(strand_, [self = shared_from_this()]() {
            boost::system::error_code ec;
            self->socket_.close(ec);
        });
    }

private:
    void do_read() {
        auto self = shared_from_this();
        asio::async_read_until(socket_, streambuf_, '\n',
            asio::bind_executor(strand_,
                [this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
                    if (!ec) {
                        std::string line;
                        std::istream is(&streambuf_);
                        std::getline(is, line);
                        std::string response = interpret_command(line);

                        if (!response.empty())
                            enqueue_write(response);
                        else
                            do_read();
                    } else
                        do_close();
                }
            )
        );
    }

    void enqueue_write(const std::string& msg) {
        auto self = shared_from_this();
        asio::post(strand_, [this, self, msg]() {
            bool write_in_progress = !write_queue_.empty();
            write_queue_.push_back(msg);
            if (!write_in_progress) {
                do_write();
            }
        });
    }

    void do_write() {
        auto self = shared_from_this();
        asio::async_write(socket_,
            asio::buffer(write_queue_.front()),
            asio::bind_executor(strand_,
                [this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
                    if (!ec) {
                        write_queue_.pop_front();
                        if (!write_queue_.empty())
                            do_write();
                        else
                            do_read();
                    } else
                        do_close();
                }
            )
        );
    }

    void do_close() {
        boost::system::error_code ec;
        socket_.shutdown(tcp::socket::shutdown_both, ec);
        socket_.close(ec);
    }

    tcp::socket socket_;
    asio::strand<asio::io_context::executor_type> strand_;
    boost::asio::streambuf streambuf_;
    std::deque<std::string> write_queue_;
};

// --- Server: accepts new connections and spawns sessions ---
class Server {
public:
    Server(asio::io_context& ioc, const tcp::endpoint& endpoint)
    : ioc_(ioc), acceptor_(ioc, endpoint)
    {
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept(
            asio::make_strand(ioc_),
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::cout << "New connection from " << socket.remote_endpoint() << "\n";
                    auto session = std::make_shared<Session>(std::move(socket), ioc_);
                    session->start();
                } else {
                    std::cerr << "Accept error: " << ec.message() << "\n";
                }
                do_accept();
            }
        );
    }

    asio::io_context& ioc_;
    tcp::acceptor acceptor_;
};

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: server <address> <port>\nExample: server 0.0.0.0 8080\n";
            return 1;
        }

        std::string address = argv[1];
        unsigned short port = static_cast<unsigned short>(std::stoi(argv[2]));

        asio::io_context ioc{1};

        tcp::endpoint endpoint{asio::ip::make_address(address), port};
        Server server(ioc, endpoint);

        unsigned int nThreads = std::max(1u, std::thread::hardware_concurrency());
        std::cout << "Server running on " << address << ":" << port << " with " << nThreads << " threads\n";

        std::vector<std::thread> v;
        for (unsigned int i = 0; i < nThreads; ++i) {
            v.emplace_back([&ioc]() { ioc.run(); });
        }

        for (auto& t : v) t.join();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}
