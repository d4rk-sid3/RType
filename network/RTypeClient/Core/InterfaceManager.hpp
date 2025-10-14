#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class TextField {
public:
    TextField(const sf::Font& font, const sf::Vector2f& position, const sf::Vector2f& size,
              bool isPassword = false)
        : active_(false), isPassword_(isPassword)
    {
        // Boîte d’arrière-plan
        box_.setPosition(position);
        box_.setSize(size);
        box_.setFillColor(sf::Color(50, 50, 50));
        box_.setOutlineThickness(2);
        box_.setOutlineColor(sf::Color::White);

        // Texte
        text_.setFont(font);
        text_.setCharacterSize(22);
        text_.setFillColor(sf::Color::White);
        text_.setPosition(position.x + 8, position.y + 5);

        // Curseur
        cursor_.setSize({2.f, text_.getCharacterSize()});
        cursor_.setFillColor(sf::Color::White);
    }

    // Gère les événements clavier/souris
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window)
    {
        if (event.type == sf::Event::MouseButtonPressed) {
            // Active le champ si clic à l’intérieur
            sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
            active_ = box_.getGlobalBounds().contains(mousePos);
            box_.setOutlineColor(active_ ? sf::Color::Cyan : sf::Color::White);
        }

        if (!active_)
            return;

        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode == '\b') { // Backspace
                if (!input_.empty()) input_.pop_back();
            }
            else if (event.text.unicode == '\r' || event.text.unicode == '\n') {
                // Enter : on peut éventuellement déclencher une action ici
            }
            else if (event.text.unicode >= 32 && event.text.unicode < 127) {
                input_ += static_cast<char>(event.text.unicode);
            }
        }
    }

    // Met à jour le curseur clignotant
    void update()
    {
        if (!active_) return;

        if (cursorClock_.getElapsedTime().asSeconds() > 0.5f) {
            showCursor_ = !showCursor_;
            cursorClock_.restart();
        }
    }

    // Affiche le champ de texte
    void draw(sf::RenderWindow& window)
    {
        window.draw(box_);

        std::string displayed = isPassword_ ? std::string(input_.size(), '*') : input_;
        text_.setString(displayed);
        window.draw(text_);

        if (active_ && showCursor_) {
            sf::FloatRect bounds = text_.getGlobalBounds();
            cursor_.setPosition(bounds.left + bounds.width + 2, text_.getPosition().y);
            window.draw(cursor_);
        }
    }

    // Accès au contenu
    const std::string& getText() const { return input_; }

    void clear() { input_.clear(); }

    bool isActive() const { return active_; }

private:
    sf::RectangleShape box_;
    sf::Text text_;
    sf::RectangleShape cursor_;
    std::string input_;

    bool active_;
    bool isPassword_;
    bool showCursor_ = true;

    sf::Clock cursorClock_;
};

#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Login Interface");
    sf::Font font;
    font.loadFromFile("assets/arial.ttf");

    TextField usernameField(font, {250, 200}, {300, 40}, false);
    TextField passwordField(font, {250, 260}, {300, 40}, true);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            usernameField.handleEvent(event, window);
            passwordField.handleEvent(event, window);
        }

        usernameField.update();
        passwordField.update();

        window.clear(sf::Color(20, 20, 20));
        usernameField.draw(window);
        passwordField.draw(window);
        window.display();
    }
}
