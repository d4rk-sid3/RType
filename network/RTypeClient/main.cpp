#include "./UI/include/ClientUI.hpp"
#include "./UI/include/UIRenderer.hpp"

int main()
{
    ClientUI clientUI;
    UIRenderer renderer;

    clientUI.init();
    while (clientUI.window.isOpen()) {
        clientUI.deltaTime = clientUI.clock.restart().asSeconds();
        clientUI.eventHandler.pollEvents(clientUI.window);
        clientUI.window.clear();
        renderer.render(clientUI);
        clientUI.window.display();
    }
    return 0;
}
