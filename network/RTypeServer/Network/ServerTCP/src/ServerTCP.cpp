#include "../include/ServerTCP.hpp"

//-----------------------SESSION-------------------------------

std::string generate_five_digit_random() {
    static std::random_device rd;
    static std::mt19937 generator(rd());

    const int min_value = 10000;
    const int max_value = 99999;
    
    std::uniform_int_distribution<int> distribution(min_value, max_value);

    int randomNumberInt = distribution(generator);
    return std::to_string(randomNumberInt);
}

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
            return "ERROR REGISTER\n"; //Missing username or password
        }

        UserManager::getInstance().createUser(username, password, DatabaseManager::getInstance().getDB());
        return "REGISTER_OK " + username + "\n";
    } else if (keyword == "LOGIN") {
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
                if (user.isUserBanned())
                    return "BANNED\n";
                if (user.getUserRole() == "ADMIN")
                    return "ADMIN_OK\n";
                std::vector<std::string> keyMap = user.getUserControl().getKeys();
                std::string final_result = "";
                for (auto value : keyMap) {
                    final_result += value;
                    final_result += " ";
                }
                final_result.pop_back();
                std::string result = std::to_string(user.getId()) + " " + username + " " +
                    std::to_string(user.getUserStats().getNbGamesPlayed()) 
                    + " " + std::to_string(user.getUserStats().getNbGamesWon()) +
                    " " + std::to_string(user.getUserStats().getLevel()) + " " + final_result;
                return "LOGIN_OK " + result + "\n";
            }
        } else
            return "LOGIN_FAIL\n";
    } else if (keyword == "SAVE") {
        auto userOpt = UserManager::getInstance().getUserByUsername(args[0]);
        if (userOpt) {
            User& user = userOpt.value().get();
            std::vector<std::string> _keyMap = {args[1], args[2], args[3], args[4], args[5]};
            user.bind_keys(_keyMap);
            user.save(DatabaseManager::getInstance().getDB());
            return "SAVE_OK\n";
        }
    } else if (keyword == "CREATE_GAME") {
        std::string code = generate_five_digit_random();

        GameManager::getInstance().create_game(code);
        std::string message = "CODE " + code + "\n";
        std::cout << message;
        return message;
    } else if (keyword == "LIST") {
        std::vector<std::string> _users = UserManager::getInstance().getUsersList();
        std::string begin = "List of users : \n";
        int i = 1;

        for (auto user : _users) {
            begin += std::to_string(i) + ". " + user + "\n";
            i++;
        }
        return begin;
    } else if (keyword == "BAN") {
        auto userOpt = UserManager::getInstance().getUserByUsername(args[0]);
        if (userOpt) {
            User& user = userOpt.value().get();
            user.setBanned(true);
            user.save(DatabaseManager::getInstance().getDB());
            std::string value = "User " + args[0] + " has been successfully banned from the server.\n";
            return value;
        }
        return "No user with username " + args[0] + " has been found in the database.\n";
    }  else if (keyword == "RELEASE") {
        auto userOpt = UserManager::getInstance().getUserByUsername(args[0]);
        if (userOpt) {
            User& user = userOpt.value().get();
            user.setBanned(false);
            user.save(DatabaseManager::getInstance().getDB());
            std::string value = "User " + args[0] + " has been successfully released from the server.\n";
            return value;
        }
        return "No user with username " + args[0] + " has been found in the database.\n";
    }
    else if (keyword == "QUIT") {
        return "BYE\n";
    }
    return "ERROR Unknown command\n";
}


Session::Session(asio::ip::tcp::socket socket, asio::io_context& ioc)
        : socket_(std::move(socket)),
          strand_(asio::make_strand(ioc))
{}

void Session::start() {
    do_read();
}

void Session::stop() {
    asio::post(strand_, [self = shared_from_this()]() {
        std::error_code ec;
        self->socket_.close(ec);
    });
}

void Session::do_read() {
    auto self = shared_from_this();
    asio::async_read_until(socket_, streambuf_, '\n', 
        asio::bind_executor(strand_,
            [this, self](std::error_code ec, std::size_t bytes_transferred) {
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
            [this, self](std::error_code ec, std::size_t bytes_transferred) {
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
    std::error_code ec;
    socket_.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
    socket_.close(ec);
}

//----------------------SERVER-----------------------------------------------

ServerTCP::ServerTCP(asio::io_context& ioc, int port)
    : ioc_(ioc), acceptor_(ioc, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
{
    init_database();
    do_accept();
}

ServerTCP::~ServerTCP() {
    DatabaseManager::getInstance().close();
}

void ServerTCP::do_accept() {
    acceptor_.async_accept(
        asio::make_strand(ioc_),
        [this](std::error_code ec, asio::ip::tcp::socket socket) {
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

void ServerTCP::init_database()
{
    DatabaseManager::getInstance().open("Database/r-type.db");
    DatabaseManager::getInstance().initialize();
    DatabaseManager::getInstance().generateServerKey();
    UserManager::getInstance().loadAllUsers(DatabaseManager::getInstance().getDB());
    UserManager::getInstance().updateId();
    return;
}