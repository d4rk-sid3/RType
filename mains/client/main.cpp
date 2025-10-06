#include "client.hpp"

int main() {
    sf::RenderWindow win(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "R-Type") ;
    registry reg(win);

    sf::Event event;
    sf::Clock frameClock;
    Client client(8080, "127.0.0.1", reg);

    NetworkManager &c = client.getManager();

    std::vector<u_int8_t> msg(10, 0x5);

    c.send(msg, msg.size(), c.getServerendpoint());

    std::thread t([&c]() { c.run(); });

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
        if (client.state == GAME) {
            client.runLevel(dt);
        }

        reg.run_systems(dt);
    }   

    c.getContext().stop();
    t.join();
}
