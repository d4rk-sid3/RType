#include "interface.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(738, 432), "R-Type Dashboard");
    ActionRegistry::getInstance().setDefaultActions();
    loadUIFromFile("homepage.cfg");
    EventHandler eventHandler;
    UIRenderer renderer;
    UIManager::getInstance().setUI("Homepage");

    while (window.isOpen()) {
        eventHandler.pollEvents(window);
        window.clear();
        renderer.render(eventHandler, window);
        window.display();
    }
    return 0;
}
