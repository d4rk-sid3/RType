#include "client.hpp"

void printClientUsage() {
    std::cerr << "Usage:\n\t./r-type_client [port] [ip_address]\n" << std::endl;
    std::cerr << "\t\tport: The port number of the server" << std::endl;
    std::cerr << "\t\tip_address: The ip address of the server" << std::endl;
}

void checkClientArgs(int ac, char **av) {
    if (ac == 2 && std::string(av[1]) == "-h") {
        printClientUsage();
        exit(0);
    }
    if (ac != 3) {
        printClientUsage();
        exit(84);
    }
    try {
        std::stoi(av[1]);
    } catch (...) {
        printClientUsage();
        exit(84);
    }
}

int main(int ac, char **av) {
    checkClientArgs(ac, av);
    Client client(std::stoi(av[1]), av[2]);

    client.run();

    // auto eventQueue = std::make_shared<ThreadSafeQueue>();
    //     ClientGraphics clientGraphics(eventQueue);
    //     asio::io_context io_context;
    //     tcp::resolver resolver(io_context.get_executor());
    //     auto endpoints = resolver.resolve(argv[1], argv[2]);

    //     auto client = std::make_shared<ClientTCP>(io_context, endpoints, eventQueue);
    //     client->start(endpoints);
        
        
    //     ActionRegistry::getInstance().setNetworkActions(client);
    //     clientGraphics.loadPages();

    //     std::thread t([&io_context]() { io_context.run(); });

    //     clientGraphics.run();

    //     io_context.stop();
    //     t.join();
}
