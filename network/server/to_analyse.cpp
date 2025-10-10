#include <iostream>
#include <memory>
#include <array>
#include <asio.hpp>

using asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    explicit Session(tcp::socket socket)
        : socket_(std::move(socket)) {}

    void start() {
        doRead();
    }

private:
    void doRead() {
        auto self = shared_from_this();
        socket_.async_read_some(
            asio::buffer(data_),
            [this, self](std::error_code ec, std::size_t length) {
                if (!ec) {
                    std::cout << "Received: " << std::string(data_.data(), length) << "\n";
                    doWrite(length);
                }
            });
    }

    void doWrite(std::size_t length) {
        auto self = shared_from_this();
        asio::async_write(
            socket_,
            asio::buffer(data_, length),
            [this, self](std::error_code ec, std::size_t /*bytes_sent*/) {
                if (!ec)
                    doRead(); // continue reading
            });
    }

    tcp::socket socket_;
    std::array<char, 1024> data_;
};

class Server {
public:
    Server(asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        doAccept();
    }

private:
    void doAccept() {
        acceptor_.async_accept(
            [this](std::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<Session>(std::move(socket))->start();
                }
                doAccept(); // accept the next client
            });
    }

    tcp::acceptor acceptor_;
};

int main() {
    try {
        asio::io_context io;
        Server server(io, 8080);

        std::cout << "Server listening on port 8080...\n";
        io.run();
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}



#include <iostream>
#include <asio.hpp>

using asio::ip::tcp;

int main() {
    try {
        asio::io_context io;

        tcp::resolver resolver(io);
        auto endpoints = resolver.resolve("127.0.0.1", "8080");

        tcp::socket socket(io);
        asio::connect(socket, endpoints);

        std::string msg = "Hello from client!\n";
        asio::write(socket, asio::buffer(msg));

        std::array<char, 1024> reply;
        size_t reply_length = socket.read_some(asio::buffer(reply));

        std::cout << "Reply from server: " << std::string(reply.data(), reply_length) << "\n";
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}
