#include "client.hpp"

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
    Client client(std::stoi(av[1]), av[2]);


    client.run();

    while (win.isOpen()) {
        double dt = frameClock.restart().asSeconds();
        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                win.close();
            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::Escape)
                    win.close();
        }
        if (client.state == MENU || client.state == TRANSITION) {
            client.runMenu(dt);
        }
        if (client.state == LEVEL1 || client.state == LEVEL2 || client.state == LEVEL3) {
            client.runLevel(dt);
            client.handleSubStates(dt, win);
        }

        reg.run_systems(dt);
        // FIXME: The engine should not clear
        // nor display, so the main loop will do that and possibly add more draws
        client.ui_handler.draw(win);
    }
}
