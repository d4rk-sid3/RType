#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include <vector>

enum class ElementTag { TEXT, BUTTON, RECTANGLE, CIRCLE, INPUTFIELD, TEXTURE };
enum class ButtonState { NORMAL, HOVER, CLICKED };

class UIElement {
public:
    std::string id;
    ElementTag tag;
    sf::Vector2f pos;

    UIElement(const std::string& id, ElementTag tag, sf::Vector2f pos)
        : id(id), tag(tag), pos(pos) {}
    virtual ~UIElement() {}
    virtual void display(sf::RenderWindow& window) = 0;
    virtual void update(EventHandler& eventHandler, sf::RenderWindow& window) = 0;
};

class TextElement : public UIElement {
public:
    sf::Text text;
    sf::Font font;

    TextElement(const std::string& id, const std::string& fontPath, const std::string& str,
        sf::Color color, sf::Vector2f pos, unsigned int size)
    : UIElement(id, ElementTag::TEXT, pos)
    {
        if (!font.loadFromFile(fontPath)) {
            std::cerr << "Erreur : impossible to load the font " << fontPath << std::endl;
        }
        text.setFont(font);
        text.setString(str);
        text.setFillColor(color);
        text.setCharacterSize(size);
        text.setPosition(pos);
    }

    void display(sf::RenderWindow& window) override {
        window.draw(text);
    }

    void update(EventHandler& eventHandler, sf::RenderWindow& window) override {
        return;
    }
};

class RectangleElement : public UIElement {
    public:
        sf::RectangleShape rect;
        sf::Color innerColor;
        sf::Color outerColor;
        float outlineThickness;
    
        RectangleElement(const std::string& id, sf::Vector2f pos, sf::Vector2f size,
                         sf::Color inner, sf::Color outer, float thickness = 2.0f)
            : UIElement(id, ElementTag::RECTANGLE, pos), innerColor(inner), outerColor(outer), outlineThickness(thickness)
        {
            rect.setPosition(pos);
            rect.setSize(size);
            rect.setFillColor(innerColor);
            rect.setOutlineColor(outerColor);
            rect.setOutlineThickness(outlineThickness);
        }
    
        void display(sf::RenderWindow& window) override {
            window.draw(rect);
        }
    
        void setPosition(sf::Vector2f pos) {
            rect.setPosition(pos);
        }
    
        void setSize(sf::Vector2f size) {
            rect.setSize(size);
        }
    
        void setInnerColor(sf::Color color) {
            innerColor = color;
            rect.setFillColor(color);
        }
    
        void setOuterColor(sf::Color color) {
            outerColor = color;
            rect.setOutlineColor(color);
        }
    
        void setOutlineThickness(float thickness) {
            outlineThickness = thickness;
            rect.setOutlineThickness(thickness);
        }

        void update(EventHandler& eventHandler, sf::RenderWindow& window) override {
            return;
        }
};
    
class ButtonElement : public UIElement {
    public:
        RectangleElement rect;
        TextElement text;
        std::function<void()> onHover;
        std::function<void()> onClick;
        ButtonState state = ButtonState::NORMAL;
    
        sf::Color normalColor;
        sf::Color hoverColor;
        sf::Color clickedColor;
    
        ButtonElement(const std::string& id, const RectangleElement& rectangle, const TextElement& textEl,
                      sf::Color normal, sf::Color hover, sf::Color clicked,
                      std::function<void()> hoverCb = nullptr,
                      std::function<void()> clickCb = nullptr)
            : UIElement(id, ElementTag::BUTTON, rectangle.pos),
              rect(rectangle),
              text(textEl),
              onHover(hoverCb),
              onClick(clickCb),
              normalColor(normal),
              hoverColor(hover),
              clickedColor(clicked)
        {
            rect.setInnerColor(normalColor);
        }
    
        void display(sf::RenderWindow& window) override {
            rect.display(window);
            text.display(window);
        }
    
        void updateState(sf::RenderWindow& window) {
            bool over = isMouseOver(window);
    
            if (over && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                if (state != ButtonState::CLICKED) {
                    state = ButtonState::CLICKED;
                    if (onClick) onClick();
                }
            } else if (state == ButtonState::CLICKED && !sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                state = over ? ButtonState::HOVER : ButtonState::NORMAL;
            }
    
            if (over && state != ButtonState::HOVER) {
                state = ButtonState::HOVER;
                if (onHover) onHover();
            } else if (!over && state != ButtonState::NORMAL) {
                state = ButtonState::NORMAL;
            }
    
            switch (state) {
                case ButtonState::NORMAL: rect.setInnerColor(normalColor); break;
                case ButtonState::HOVER: rect.setInnerColor(hoverColor); break;
                case ButtonState::CLICKED: rect.setInnerColor(clickedColor); break;
            }
        }
        void update(EventHandler& eventHandler, sf::RenderWindow& window) override {
            updateState(window);
        }
    
    private:
        bool isMouseOver(sf::RenderWindow& window) {
            sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
            return rect.rect.getGlobalBounds().contains(mousePos);
        }
};
    
class ButtonBuilder {
    private:
        std::string id;
        std::string str = "Button";
        std::string fontPath;
        sf::Font font;
        sf::Vector2f pos = {0,0};
        sf::Vector2f size = {100,50};
        sf::Color normalColor = sf::Color::Green;
        sf::Color hoverColor = sf::Color::Yellow;
        sf::Color clickedColor = sf::Color::Red;
        sf::Color textColor = sf::Color::Black;
        unsigned int textSize = 20;
        float outlineThickness = 2.0f;
        sf::Color outlineColor = sf::Color::White;
        std::function<void()> onHover = nullptr;
        std::function<void()> onClick = nullptr;
    
    public:
        ButtonBuilder(const std::string& id) : id(id) {}
    
        ButtonBuilder& setText(const std::string& s, const std::string & fontPath) { 
            str = s;
            if (!font.loadFromFile(fontPath)) {
                std::cerr << "Erreur : impossible to load the font " << fontPath << std::endl;
            }
            return *this; 
        }
        ButtonBuilder& setTextColor(sf::Color c) { textColor = c; return *this; }
        ButtonBuilder& setTextSize(unsigned int size) { textSize = size; return *this; }
        ButtonBuilder& setPosition(sf::Vector2f p) { pos = p; return *this; }
        ButtonBuilder& setSize(sf::Vector2f s) { size = s; return *this; }
        ButtonBuilder& setColors(sf::Color normal, sf::Color hover, sf::Color clicked) { 
            normalColor = normal; hoverColor = hover; clickedColor = clicked; return *this; 
        }
        ButtonBuilder& setOutline(float thickness, sf::Color color) { outlineThickness = thickness; outlineColor = color; return *this; }
        ButtonBuilder& setOnHover(std::function<void()> f) { onHover = f; return *this; }
        ButtonBuilder& setOnClick(std::function<void()> f) { onClick = f; return *this; }
    
        ButtonElement build() {
            RectangleElement rect(id, pos, size, normalColor, outlineColor, outlineThickness);
            TextElement textEl(id, fontPath, str, textColor, pos + sf::Vector2f(5,5), textSize);
            return ButtonElement(id, rect, textEl, normalColor, hoverColor, clickedColor, onHover, onClick);
        }
};
        

class CircleElement : public UIElement {
    public:
        sf::CircleShape circle;
        sf::Color innerColor;
        sf::Color outerColor;
        float outlineThickness;
    
        CircleElement(const std::string& id, sf::Vector2f pos, float radius,
                        sf::Color inner, sf::Color outer, float thickness = 2.0f)
            : UIElement(id, ElementTag::CIRCLE, pos), innerColor(inner), outerColor(outer), outlineThickness(thickness)
        {
            circle.setRadius(radius);
            circle.setPosition(pos);
            circle.setFillColor(innerColor);
            circle.setOutlineColor(outerColor);
            circle.setOutlineThickness(outlineThickness);
            circle.setOrigin(radius, radius);
        }
    
        void display(sf::RenderWindow& window) override {
            window.draw(circle);
        }
    
        void setPosition(sf::Vector2f pos) {
            this->pos = pos;
            circle.setPosition(pos);
        }
    
        void setRadius(float radius) {
            circle.setRadius(radius);
            circle.setOrigin(radius, radius);
        }
    
        void setInnerColor(sf::Color color) {
            innerColor = color;
            circle.setFillColor(color);
        }
    
        void setOuterColor(sf::Color color) {
            outerColor = color;
            circle.setOutlineColor(color);
        }
    
        void setOutlineThickness(float thickness) {
            outlineThickness = thickness;
            circle.setOutlineThickness(thickness);
        }

        void update(EventHandler& eventHandler, sf::RenderWindow& window) override {
            return;
        }
};

class TextureElement : public UIElement {
    public:
        sf::Sprite sprite;
        sf::Texture texture;
        sf::Vector2f scale;
    
        TextureElement(const std::string& id, const std::string& texturePath, sf::Vector2f pos, sf::Vector2f scale = {1.f, 1.f})
            : UIElement(id, ElementTag::TEXTURE, pos), scale(scale)
        {
            if (!texture.loadFromFile(texturePath)) {
                std::cerr << "Erreur: impossible de charger la texture " << texturePath << std::endl;
            }
            sprite.setTexture(texture);
            sprite.setPosition(pos);
            sprite.setScale(scale);
        }
    
        void display(sf::RenderWindow& window) override {
            window.draw(sprite);
        }
    
        void setPosition(sf::Vector2f pos) {
            this->pos = pos;
            sprite.setPosition(pos);
        }
    
        void setScale(sf::Vector2f s) {
            scale = s;
            sprite.setScale(scale);
        }
    
        void setTexture(const std::string& texturePath) {
            if (!texture.loadFromFile(texturePath)) {
                std::cerr << "Erreur: impossible de charger la texture " << texturePath << std::endl;
                return;
            }
            sprite.setTexture(texture);
        }

        void update(EventHandler& eventHandler, sf::RenderWindow& window) override {
            return;
        }
};
        
class EventHandler {
    public:
        std::vector<sf::Event> events;
    
        void pollEvents(sf::RenderWindow& window) {
            events.clear();
            sf::Event event;
            while (window.pollEvent(event)) {
                events.push_back(event);
            }
        }
    
        const std::vector<sf::Event>& getEvents() const {
            return events;
        }
};
        
class InputFieldElement : public UIElement {
    public:
        RectangleElement rect;
        sf::Text text;
        std::string content;
        bool isPassword;
        bool active = false;
        sf::Color activeOutlineColor;
        const sf::Font font;
    
        InputFieldElement(const std::string& id, const std::string& fontPath, sf::Vector2f pos, sf::Vector2f size,
                bool password = false, sf::Color activeColor = sf::Color::Blue)
            : UIElement(id, ElementTag::INPUTFIELD, pos),
                rect(id, pos, size, sf::Color::White, sf::Color::Black, 2.0f),
                isPassword(password), activeOutlineColor(activeColor)
        {
            text.setFont(font);
            text.setFillColor(sf::Color::Black);
            text.setPosition(pos + sf::Vector2f(5, 5));
            text.setCharacterSize(20);
        }
    
        void addChar(char c) {
            content += c;
            text.setString(isPassword ? std::string(content.size(), '*') : content);
        }
    
        void backspace() {
            if (!content.empty()) {
                content.pop_back();
                text.setString(isPassword ? std::string(content.size(), '*') : content);
            }
        }
    
        void updateActive(sf::RenderWindow& window) {
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
    
        void handleInput(const sf::Event& event) {
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
    
        void update(EventHandler& eventHandler, sf::RenderWindow& window) override {
            updateActive(window);
            for (const auto& event : eventHandler.getEvents()) {
                handleInput(event);
            }
        }
    
        void display(sf::RenderWindow& window) override {
            rect.display(window);
            window.draw(text);
        }
};

class UI {
    public:
        std::string id;
        std::vector<std::shared_ptr<UIElement>> elements;

        UI(const std::string& id) : id(id) {}
        void addElement(std::shared_ptr<UIElement> el) { elements.push_back(el); }
        std::shared_ptr<UIElement> getElementById(const std::string& elementId) {
            for (auto& el : elements) {
                if (el->id == elementId)
                    return el;
            }
            return nullptr;
        }
};

class UIManager {
    private:
        std::unordered_map<std::string, std::shared_ptr<UI>> uis;
        std::shared_ptr<UI> currentUI;

        UIManager() = default;
    public:
        UIManager(const UIManager&) = delete;
        UIManager& operator=(const UIManager&) = delete;

        static UIManager& getInstance() {
            static UIManager instance;
            return instance;
        }

        void addUI(std::shared_ptr<UI> ui) { uis[ui->id] = ui; }

        void setUI(const std::string& id) {
            auto it = uis.find(id);
            if (it != uis.end()) {
                currentUI = it->second;
            } else {
                std::cerr << "UIManager: UI with id" << id << " not found !" << std::endl;
            }
        }
        std::shared_ptr<UI> getCurrentUI() const { return currentUI; }

        std::shared_ptr<UI> getUI(const std::string& id) {
            auto it = uis.find(id);
            if (it != uis.end()) return it->second;
            return nullptr;
        }
};

class UIRenderer {
    public:
        void render(EventHandler& eventHandler, sf::RenderWindow& window) {
            if (!UIManager::getInstance().getCurrentUI())
                return;
    
            for (auto& el : UIManager::getInstance().getCurrentUI()->elements) {
                el->update(eventHandler, window);
                el->display(window);
            }
        }
};

class ActionRegistry {
    public:
        using Action = std::function<void()>;
        std::unordered_map<std::string, Action> actions;
    
        static ActionRegistry& getInstance() {
            static ActionRegistry instance;
            return instance;
        }
    
        void registerAction(const std::string& name, Action action) {
            actions[name] = std::move(action);
        }
    
        Action get(const std::string& name) {
            auto it = actions.find(name);
            if (it != actions.end())
                return it->second;
            return nullptr;
        }
};
    

// -----------------------------
// Main pour tester
// -----------------------------
int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Menu Test");
    sf::Font font;
    font.loadFromFile("Gabriela-Regular.ttf");  

    UIManager uiManager;
    Drawer drawer;

    // Création UI
    auto mainMenu = std::make_shared<UI>("mainMenu");

    auto text = std::make_shared<TextElement>("title", font, "Bienvenue", sf::Color::White, sf::Vector2f(300,50), 50);
    mainMenu->addElement(text);

    // auto button = std::make_shared<ButtonElement>("startBtn", sf::Vector2f(300, 200), sf::Vector2f(200,50),
    //                                               "Start", font, sf::Color::Green, sf::Color::White,
    //                                               nullptr,
    //                                               [&](){ std::cout << "Button clicked!" << std::endl; });
    //mainMenu->addElement(button);

    auto input = std::make_shared<InputFieldElement>("username", sf::Vector2f(300, 300), sf::Vector2f(200,40), font);
    mainMenu->addElement(input);

    uiManager.addUI(mainMenu);
    uiManager.setUI("mainMenu");

    // Boucle principale
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Gestion input clavier pour InputField
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    char c = static_cast<char>(event.text.unicode);
                    if (c == 8) input->backspace(); // backspace
                    else input->addChar(c);
                }
            }

            // Gestion click
            // if (event.type == sf::Event::MouseButtonPressed) {
            //     if (button->isMouseOver(window) && button->onClick) button->onClick();
            // }
        }

        window.clear(sf::Color::Black);
        drawer.display(uiManager, window);
        window.display();
    }

    return 0;
}
