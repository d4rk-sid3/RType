#include "./Network/include/client.hpp"
#include "./UI/include/ClientGraphics.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: client <host> <port>\n";
        return 84;
    }

    try {
        ClientGraphics clientGraphics;
        asio::io_context io_context;
        tcp::resolver resolver(io_context.get_executor());
        auto endpoints = resolver.resolve(argv[1], argv[2]);

        auto client = std::make_shared<Client>(io_context, endpoints);

        std::thread t([&io_context]() { io_context.run(); });

        clientGraphics.run();

        io_context.stop();
        t.join();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
