#ifndef CLIENT
    #define CLIENT
    #include "../../UI/include/librairies.hpp"

class Client : public std::enable_shared_from_this<Client> {
public:
    Client(asio::io_context& io_context, tcp::resolver::results_type endpoints);

    void write(const std::string& msg);

private:
    void do_connect(tcp::resolver::results_type endpoints);

    void do_read();

    void do_write();

    tcp::socket socket_;
    asio::streambuf read_buffer_;
    std::deque<std::string> write_queue_;
};
#endif