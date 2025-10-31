#ifndef SERVER
    #define SERVER
    #include "../../Auth/include/librairies.hpp"
    #include "../../Managers/DatabaseManager.hpp"
    #include "../../Managers/UserManager.hpp"

class Session : public std::enable_shared_from_this<Session> {
    public:
        Session(tcp::socket socket, asio::io_context& ioc);
    
        void start();
    
        void stop();
    
    private:
        void do_read();
    
        void enqueue_write(const std::string& msg);
    
        void do_write();
    
        void do_close();
    
        tcp::socket socket_;
        asio::strand<asio::io_context::executor_type> strand_;
        boost::asio::streambuf streambuf_;
        std::deque<std::string> write_queue_;
};
    
// --- Server: accepts new connections and spawns sessions ---
class Server {
    public:
        Server(asio::io_context& ioc, const tcp::endpoint& endpoint);
        ~Server();
        void init_database();

    private:
        void do_accept();

        asio::io_context& ioc_;
        tcp::acceptor acceptor_;
};
#endif
