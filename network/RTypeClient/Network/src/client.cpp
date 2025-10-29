#include "../include/client.hpp"

Client::Client(asio::io_context& io_context, tcp::resolver::results_type endpoints)
    : socket_(io_context) {
    do_connect(endpoints);
}

void Client::write(const std::string& msg) {
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

void Client::do_connect(tcp::resolver::results_type endpoints) {
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

void Client::do_read() {
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

void Client::do_write() {
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