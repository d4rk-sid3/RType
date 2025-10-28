#include "UIRenderer.hpp"

void UIRenderer::render(ClientUI &clientUI) {
    auto& uiManager = UIManager::getInstance();
    uiManager.updateTransition(clientUI.deltaTime);

    auto current = uiManager.getCurrentUI();

    if (current && current->getId() == "Dashboardpage") {
        auto elem = current->getElementById("Hour");
        auto cast = std::dynamic_pointer_cast<TextElement>(elem);

        std::string date = getHour();
        cast->setText(date);
    }

    auto next = uiManager.getNextUI();

    if (!current && !next)
        return;

    if (current) {
        for (auto& el : current->elements) {
            el->update(clientUI.eventHandler, clientUI.window);
            el->display(clientUI.window);
        }
    }

    if (next && uiManager.isTransitioning()) {
        sf::RenderTexture temp;
        temp.create(clientUI.window.getSize().x, clientUI.window.getSize().y);
        temp.clear(sf::Color::Transparent);

        for (auto& el : next->elements) {
            el->update(clientUI.eventHandler, clientUI.window);
            el->display(temp);
        }

        temp.display();

        sf::Sprite fadeSprite(temp.getTexture());
        fadeSprite.setColor(sf::Color(255, 255, 255, static_cast<int>(uiManager.getAlpha() * 255)));
        clientUI.window.draw(fadeSprite);
    }
}