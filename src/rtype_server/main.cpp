#include <iostream>
#include "header/server.hpp"

int main() {
    try {     
        Server s(8080);
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    
}
