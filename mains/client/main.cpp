#include "client.hpp"

int main(int ac, char **av)
{
    Client client(std::stoi(av[1]), av[2]);

    client.run();
}
