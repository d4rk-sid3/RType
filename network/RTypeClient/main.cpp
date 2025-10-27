#include "interface.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "R-Type Dashboard");
    ActionRegistry::getInstance().setDefaultActions();
    loadUIFromFile("dashboard.cfg");
    EventHandler eventHandler;
    UIRenderer renderer;
    UIManager::getInstance().setUI("Dashboard");

    while (window.isOpen()) {
        eventHandler.pollEvents(window);
        window.clear();
        renderer.render(eventHandler, window);
        window.display();
    }
    return 0;
}
