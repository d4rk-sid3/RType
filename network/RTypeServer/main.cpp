#include "./Network/include/Server.hpp"

int main(int argc, char* argv[])
{
    try {
        if (argc != 3) {
            std::cerr << "Usage: server <address> <port>\nExample: server 0.0.0.0 8080\n";
            return 84;
        }

        std::string address = argv[1];
        unsigned short port = static_cast<unsigned short>(std::stoi(argv[2]));

        asio::io_context ioc{1};

        tcp::endpoint endpoint{asio::ip::make_address(address), port};
        Server server(ioc, endpoint);

        unsigned int nThreads = std::max(1u, std::thread::hardware_concurrency());

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
