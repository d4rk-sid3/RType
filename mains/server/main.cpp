#include "server.hpp"
#include "logic_functions.hpp"

int main(int ac, char **av)
{
    Server server(std::stoi(av[1]));

    server.run();
}
