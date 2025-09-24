#include "server.hpp"

int main() {
    try {
        Server server(8080);

    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    
}
