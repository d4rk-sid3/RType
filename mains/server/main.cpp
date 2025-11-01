#include "../../rtype_gameManager/include/gameManager.hpp"

#include "logic_functions.hpp"

void printServerUsage() {
    std::cerr << "Usage:\n\t./r-type_server [port] [difficulty]\n" << std::endl;
    std::cerr << "\t\tport: The port number to run the server on" << std::endl;
    std::cerr << "\t\tdifficulty: The difficulty of the game (easy | medium | hard)" << std::endl;
}

diff_mode_t get_diff_mode(std::string mode) {
    if (mode == "-f")
        return CUSTOM;
    if (mode == "easy")
        return EASY;
    if (mode == "medium")
        return MEDIUM;
    if (mode == "hard")
        return HARD;
    if (mode == "pvp")
        return PVP;
    printServerUsage();
    exit(84);
}

void checkServerArgs(int ac, char **av) {
    if (ac == 2 && std::string(av[1]) == "-h") {
        printServerUsage();
        exit(0);
    }
    if (ac == 4 && std::string(av[2]) == "-f") {
        custom_conf_path = av[3];
        return;
    }
    if (ac != 3) {
        printServerUsage();
        exit(84);
    }
    try {
        std::stoi(av[1]);
    } catch (...) {
        printServerUsage();
        exit(84);
    }
}

int main(int ac, char **av)
{
    checkServerArgs(ac, av);

    diff_mode = get_diff_mode(av[3]);

    unsigned int nThreads = std::max(1u, std::thread::hardware_concurrency());

    asio::io_context ioc(nThreads);

    std::vector<std::thread> v;

    for (unsigned int i = 0; i < nThreads; ++i) {
        v.emplace_back([&ioc]() { ioc.run(); });
    }

    ServerTCP server(ioc, std::stoi(av[1]));

    std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>> messages;

    std::mutex mtx;

    NetworkManager server_(std::stoi(av[2]), std::ref(messages), std::ref(mtx), ioc);

    GameManager gameManager(messages, mtx, server_);

    for (auto& t : v) t.join();
}
