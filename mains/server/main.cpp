#include "server.hpp"

int main() {
    sf::RenderWindow win(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "R-Type") ;
    registry reg(win);

    sf::Event event;
    sf::Clock frameClock;
    Server server(8080, reg);

    NetworkManager &c = server.getManager();

    std::thread t([&c]() { c.run(); });


    while (win.isOpen()) {
        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                win.close();
            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::Escape)
                    win.close();
        }

        double dt = frameClock.restart().asSeconds();
        reg.run_systems(dt);
        server.runLevel(dt);
    }   

    c.getContext().stop();
    t.join();
}
