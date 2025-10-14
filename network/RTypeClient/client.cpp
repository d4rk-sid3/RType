#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <atomic>
#include <string>

using boost::asio::ip::tcp;
namespace asio = boost::asio;

#include <iostream>
#include <string>
#include <deque>
#include <memory>

class Client : public std::enable_shared_from_this<Client> {
public:
    Client(asio::io_context& io_context, tcp::resolver::results_type endpoints)
        : socket_(io_context) {
        do_connect(endpoints);
    }

    void write(const std::string& msg) {
        auto self = shared_from_this();
        asio::post(socket_.get_executor(),
            [this, self, msg]() {
                bool write_in_progress = !write_queue_.empty();
                write_queue_.push_back(msg);
                if (!write_in_progress) {
                    do_write();
                }
            });
    }

private:
    void do_connect(tcp::resolver::results_type endpoints) {
        auto self = shared_from_this();
        asio::async_connect(socket_, endpoints,
            [this, self](std::error_code ec, tcp::endpoint) {
                if (!ec) {
                    do_read();
                } else {
                    std::cerr << "Connect failed: " << ec.message() << "\n";
                }
            });
    }

    void do_read() {
        auto self = shared_from_this();
        asio::async_read_until(socket_, read_buffer_, '\n',
            [this, self](std::error_code ec, std::size_t) {
                if (!ec) {
                    std::istream is(&read_buffer_);
                    std::string line;
                    std::getline(is, line);
                    std::cout << "[server] " << line << "\n";
                    do_read(); // relancer lecture
                } else {
                    std::cerr << "Read error: " << ec.message() << "\n";
                    socket_.close();
                }
            });
    }

    void do_write() {
        auto self = shared_from_this();
        asio::async_write(socket_,
            asio::buffer(write_queue_.front()),
            [this, self](std::error_code ec, std::size_t) {
                if (!ec) {
                    write_queue_.pop_front();
                    if (!write_queue_.empty()) {
                        do_write();
                    }
                } else {
                    socket_.close();
                }
            });
    }

    tcp::socket socket_;
    asio::streambuf read_buffer_;
    std::deque<std::string> write_queue_;
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: client <host> <port>\n";
        return 1;
    }

    try {
        asio::io_context io_context;
        tcp::resolver resolver(io_context.get_executor());
        auto endpoints = resolver.resolve(argv[1], argv[2]);

        auto client = std::make_shared<Client>(io_context, endpoints);

        std::thread t([&io_context]() { io_context.run(); });

        std::string line;
        while (std::getline(std::cin, line)) {
            line.push_back('\n');
            client->write(line);
            if (line == "QUIT\n")
                break;
        }

        io_context.stop();
        t.join();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

