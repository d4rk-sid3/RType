#include "../include/InputFieldElement.hpp"
#include "../../../Managers/FontManager.hpp"

InputFieldElement::InputFieldElement(const std::string& id, const std::string& fontPath, sf::Vector2f pos, sf::Vector2f size,
    bool password, sf::Color activeColor)
    : UIElement(id, ElementTag::INPUTFIELD, pos),
        rect(id, pos, size, sf::Color::White, sf::Color::Black, 2.0f),
        isPassword(password), activeOutlineColor(activeColor)
{
    text.setFont(FontManager::getInstance().getFont(fontPath));
    text.setFillColor(sf::Color::Black);
    text.setPosition(pos + sf::Vector2f(5, 5));
    text.setCharacterSize(17);
}

void InputFieldElement::addChar(char c) {
    content += c;
    text.setString(isPassword ? std::string(content.size(), '*') : content);
}

void InputFieldElement::backspace() {
    if (!content.empty()) {
        content.pop_back();
        text.setString(isPassword ? std::string(content.size(), '*') : content);
    }
}

void InputFieldElement::updateActive(sf::RenderWindow& window) {
    sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (rect.rect.getGlobalBounds().contains(mousePos)) {
            active = true;
        } else {
            active = false;
        }
    }
    rect.setInnerColor(rect.innerColor);
    rect.setOuterColor(active ? activeOutlineColor : sf::Color::Black);
}

void InputFieldElement::handleInput(const sf::Event& event) {
    if (!active)
        return;

    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode >= 32 && event.text.unicode <= 126) {
            addChar(static_cast<char>(event.text.unicode));
        }
    } else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Backspace) {
            backspace();
        }
    }
}

void InputFieldElement::update(EventHandler& eventHandler, sf::RenderWindow& window) {
    updateActive(window);
    for (const auto& event : eventHandler.getEvents()) {
        handleInput(event);
    }
}

void InputFieldElement::display(sf::RenderWindow& window) {
    rect.display(window);
    window.draw(text);
}

void InputFieldElement::display(sf::RenderTexture& window) {
    rect.display(window);
    window.draw(text);
}