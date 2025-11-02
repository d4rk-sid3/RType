#include "../../rtype_graphicsClient/include/graphicsClient.hpp"
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

    auto eventQueue = std::make_shared<ThreadSafeQueue>();
    ClientGraphics clientGraphics(eventQueue);

    asio::io_context context;

    asio::ip::tcp::resolver resolver(context);
    auto endpoints = resolver.resolve(address, std::to_string(port));
    auto client = std::make_shared<ClientTCP>(context, endpoints, eventQueue);
    client->start(endpoints);
    ActionRegistry::getInstance().setNetworkActions(client);
    clientGraphics.loadPages();

    std::vector<int8_t> lastmsg;
    std::mutex mtx;

    NetworkManager networkManager(port, address, lastmsg, mtx, context);

    std::thread t([&context]() { context.run(); });

    GraphicsClient grClient(networkManager, lastmsg, mtx);

    grClient.run();

    context.stop();
}
