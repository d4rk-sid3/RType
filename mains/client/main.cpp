#include "client.hpp"
#include "../../network/RTypeClient/UI/include/ClientGraphics.hpp"

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

    int port = std::stoi(av[1]);
    std::string address = av[2];
    std::vector<int8_t> lastmsg;
    std::mutex mtx;

    auto eventQueue = std::make_shared<ThreadSafeQueue>();
    ClientGraphics clientGraphics(eventQueue);

    unsigned int nThreads = std::max(1u, std::thread::hardware_concurrency());
    asio::io_context context(nThreads);


    tcp::resolver resolver(context);
    auto endpoints = resolver.resolve(address, std::to_string(port));
    auto client = std::make_shared<ClientTCP>(context, endpoints, eventQueue);
    client->start(endpoints);
    ActionRegistry::getInstance().setNetworkActions(client);
    clientGraphics.loadPages();

    std::vector<std::thread> v;

    for (unsigned int i = 0; i < nThreads; ++i) {
        v.emplace_back([&context]() { context.run(); });
    }

    NetworkManager networkManager(port, address, lastmsg, mtx, context);

    Client client(networkManager, lastmsg, mtx);

    client.run();

    context.stop();
    for (auto& thread : v) {
        thread.join();
    }

}
