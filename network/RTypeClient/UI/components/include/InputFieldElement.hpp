#ifndef INPUTFIELDELEMENT
    #define INPUTFIELDELEMENT
    #include "../../include/librairies.hpp"
    #include "UIElement.hpp"
    #include "RectangleElement.hpp"

class InputFieldElement : public UIElement {
    public:
        RectangleElement rect;
        sf::Text text;
        std::string content = "";
        bool isPassword;
        bool active = false;
        bool isRemapMode = false;
        sf::Color activeOutlineColor;
        unsigned int size;
    
        InputFieldElement(const std::string& id, const std::string& fontPath, sf::Vector2f pos, sf::Vector2f size,
                bool password = false, sf::Color activeColor = sf::Color::Blue, bool remapMode = false, 
                unsigned int characterSize = 17);
    
        void addChar(char c);
    
        void backspace();
    
        void updateActive(sf::RenderWindow& window);
    
        void handleInput(const sf::Event& event);
    
        void update(EventHandler& eventHandler, sf::RenderWindow& window) override;
    
        void display(sf::RenderWindow& window) override;

        void display(sf::RenderTexture& window) override;

        const std::string &getText() const;

        void setText(const std::string &text);
};
#endif