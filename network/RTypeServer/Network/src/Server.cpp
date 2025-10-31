#include "../include/Server.hpp"

//-----------------------SESSION-------------------------------

#include <string>
#include <sstream>
#include <vector>

static std::string interpret_command(const std::string& line)
{
    std::string cmd = line;
    if (!cmd.empty() && (cmd.back() == '\r' || cmd.back() == '\n'))
        cmd.pop_back();

    std::istringstream iss(cmd);
    std::string keyword;
    std::vector<std::string> args;
    iss >> keyword;
    std::string token;
    while (iss >> token)
        args.push_back(token);

    if (keyword == "REGISTER") {
        if (args.size() < 2)
            return "ERROR REGISTER\n"; //Missing username or password

        const std::string& username = args[0];
        const std::string& password = args[1];

        if (username.empty() || password.empty()) {
            std::cout << "Joking" << std::endl;
            return "ERROR REGISTER\n"; //Missing username or password
        }

        UserManager::getInstance().createUser(username, password, DatabaseManager::getInstance().getDB());
        return "REGISTER_OK " + username + "\n";
    }

    if (keyword == "LOGIN") {
        if (args.size() < 2)
            return "ERROR LOGIN\n"; //Missing username or password

        const std::string& username = args[0];
        const std::string& password = args[1];

        if (username.empty() || password.empty())
            return "ERROR LOGIN\n"; //Missing username or password

        bool valid = UserManager::getInstance().authenticate(username, password, DatabaseManager::getInstance().getServerKey());

        if (valid) {
            auto userOpt = UserManager::getInstance().getUserByUsername(username);
            if (userOpt) {
                User& user = userOpt.value().get();
                std::string result = std::to_string(user.getId()) + " " + username + " " +
                    std::to_string(user.getUserStats().getNbGamesPlayed()) + " " + std::to_string(user.getUserStats().getNbGamesWon()) +
                    " " + std::to_string(user.getUserStats().getLevel());
                return "LOGIN_OK " + result + "\n";
            }
            std::cout << "Damn!! I don't know what happens here!!";
        } else
            return "LOGIN_FAIL\n";
    }

    else if (keyword == "QUIT") {
        return "BYE\n";
    }
    return "ERROR Unknown command\n";
}


Session::Session(tcp::socket socket, asio::io_context& ioc)
        : socket_(std::move(socket)),
          strand_(asio::make_strand(ioc))
{}

void Session::start() {
    do_read();
}

void Session::stop() {
    asio::post(strand_, [self = shared_from_this()]() {
        boost::system::error_code ec;
        self->socket_.close(ec);
    });
}

void Session::do_read() {
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

void Session::enqueue_write(const std::string& msg) {
    auto self = shared_from_this();
    asio::post(strand_, [this, self, msg]() {
        bool write_in_progress = !write_queue_.empty();
        write_queue_.push_back(msg);
        if (!write_in_progress) {
            do_write();
        }
    });
}

void Session::do_write() {
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

void Session::do_close() {
    boost::system::error_code ec;
    socket_.shutdown(tcp::socket::shutdown_both, ec);
    socket_.close(ec);
}

//----------------------SERVER-----------------------------------------------

Server::Server(asio::io_context& ioc, const tcp::endpoint& endpoint)
    : ioc_(ioc), acceptor_(ioc, endpoint)
{
    init_database();
    do_accept();
}

Server::~Server() {
    DatabaseManager::getInstance().close();
}

void Server::do_accept() {
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

void Server::init_database()
{
    DatabaseManager::getInstance().open("Database/r-type.db");
    DatabaseManager::getInstance().initialize();
    DatabaseManager::getInstance().generateServerKey();
    UserManager::getInstance().loadAllUsers(DatabaseManager::getInstance().getDB());
    UserManager::getInstance().updateId();
    return;
}