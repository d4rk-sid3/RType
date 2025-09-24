#include "client.hpp"

int main() {
    try {
        Client client(8080, "");

    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}
