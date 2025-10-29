#include "../include/ClientGraphics.hpp"

ClientGraphics::ClientGraphics() {
    clientUI.init();
}

ClientGraphics::~ClientGraphics() = default;

void ClientGraphics::run()
{
    while (clientUI.window.isOpen()) {
        clientUI.deltaTime = clientUI.clock.restart().asSeconds();
        clientUI.eventHandler.pollEvents(clientUI.window);
        clientUI.window.clear();
        renderer.render(clientUI);
        clientUI.window.display();
    }
    return;
}