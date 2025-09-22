#include <iostream>

#include "header/Network.hpp"

int main() {
    try {
        NetworkManager network(8080);
        std::cout << "Serveur en cours." << std::endl;
        network.run();

    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}
