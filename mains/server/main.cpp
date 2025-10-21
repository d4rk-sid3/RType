#include "server.hpp"
#include "logic_functions.hpp"

void printServerUsage() {
    std::cerr << "Usage:\n\t./r-type_server [port] [difficulty]\n" << std::endl;
    std::cerr << "\t\tport: The port number to run the server on" << std::endl;
    std::cerr << "\t\tdifficulty: The difficulty of the game (easy | medium | hard)" << std::endl;
}

diff_mode_t get_diff_mode(std::string mode) {
    if (mode == "easy")
        return EASY;
    if (mode == "medium")
        return MEDIUM;
    if (mode == "hard")
        return HARD;
    printServerUsage();
    exit(84);
}

void checkServerArgs(int ac, char **av) {
    if (ac == 2 && std::string(av[1]) == "-h") {
        printServerUsage();
        exit(0);
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


int main(int ac, char **av) {

    checkServerArgs(ac, av);
    diff_mode = get_diff_mode(av[2]);

    Server server(std::stoi(av[1]));

    server.run();
}
