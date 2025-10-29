#ifndef INPUTFIELDELEMENT
    #define INPUTFIELDELEMENT
    #include "../../include/librairies.hpp"
    #include "UIElement.hpp"
    #include "RectangleElement.hpp"

class InputFieldElement : public UIElement {
    public:
        RectangleElement rect;
        sf::Text text;
        std::string content;
        bool isPassword;
        bool active = false;
        sf::Color activeOutlineColor;
    
        InputFieldElement(const std::string& id, const std::string& fontPath, sf::Vector2f pos, sf::Vector2f size,
                bool password = false, sf::Color activeColor = sf::Color::Blue);
    
        void addChar(char c);
    
        void backspace();
    
        void updateActive(sf::RenderWindow& window);
    
        void handleInput(const sf::Event& event);
    
        void update(EventHandler& eventHandler, sf::RenderWindow& window) override;
    
        void display(sf::RenderWindow& window) override;

        void display(sf::RenderTexture& window) override;
};
#endif