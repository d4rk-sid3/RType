#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include <vector>
#include <libconfig.h++>

enum class ElementTag { TEXT, BUTTON, RECTANGLE, CIRCLE, INPUTFIELD, TEXTURE };
enum class ButtonState { NORMAL, HOVER, CLICKED };

class EventHandler {
    public:
        std::vector<sf::Event> events;
    
        void pollEvents(sf::RenderWindow& window) {
            events.clear();
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {                        
                        int x = event.mouseButton.x;
                        int y = event.mouseButton.y;
                        
                        std::cout << "Clic Gauche détecté en X: " << x 
                                    << ", Y: " << y << std::endl;
                    }
                }
                events.push_back(event);
            }
        }
    
        const std::vector<sf::Event>& getEvents() const {
            return events;
        }
};

class FontManager {
public:
    static FontManager& getInstance() {
        static FontManager instance;
        return instance;
    }

    FontManager(const FontManager&) = delete;
    FontManager& operator=(const FontManager&) = delete;

    sf::Font& getFont(const std::string& path) {
        if (fonts.find(path) == fonts.end()) {
            if (!fonts[path].loadFromFile(path))
                std::cerr << "Error: impossible to load the font " << path << std::endl;
        }
        return fonts[path];
    }

private:
    FontManager() = default;
    ~FontManager() = default;
    std::unordered_map<std::string, sf::Font> fonts;
};


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
    virtual ElementTag getTag() {return tag;}
};

class TextElement : public UIElement {
public:
    sf::Text text;
    std::string fontpath;

    TextElement(const std::string& id, const std::string& fontPath, const std::string& str,
        sf::Color color, sf::Vector2f pos, unsigned int size)
    : UIElement(id, ElementTag::TEXT, pos)
    {
        fontpath = fontPath;
        text.setFont(FontManager::getInstance().getFont(fontpath));
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

    void displayFontPath() {std::cout << fontpath << std::endl;}

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
            return;
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
        sf::Vector2f textPos = {0,0};
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
    
        ButtonBuilder& setText(const std::string& s, const std::string & fontpath) { 
            str = s;
            fontPath = fontpath;
            return *this; 
        }
        ButtonBuilder& setTextPos(const sf::Vector2f pos) { 
            textPos = pos;
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
            TextElement textEl(id, fontPath, str, textColor, textPos, textSize);
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
        
class InputFieldElement : public UIElement {
    public:
        RectangleElement rect;
        sf::Text text;
        std::string content;
        bool isPassword;
        bool active = false;
        sf::Color activeOutlineColor;
    
        InputFieldElement(const std::string& id, const std::string& fontPath, sf::Vector2f pos, sf::Vector2f size,
                bool password = false, sf::Color activeColor = sf::Color::Blue)
            : UIElement(id, ElementTag::INPUTFIELD, pos),
                rect(id, pos, size, sf::Color::White, sf::Color::Black, 2.0f),
                isPassword(password), activeOutlineColor(activeColor)
        {
            text.setFont(FontManager::getInstance().getFont(fontPath));
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

        void setDefaultActions() {
            const std::vector<std::string> pages = {
                "HomePage", "Register", "Login", "Dashboard", 
                "Settings", "Scoreboard", "CreateParty", "JoinParty"
            };
    
            for (const auto& page : pages) {
                registerAction(page, [page]() {
                    UIManager::getInstance().setUI(page);
                });
            }
        }
};
    
std::shared_ptr<UIElement> parseTextElement(const libconfig::Setting& setting)
{
    try {
        std::string id;
        std::string fontPath;
        std::string str;
        unsigned int size = 20;
        sf::Vector2f pos(0.f, 0.f);
        sf::Color color = sf::Color::White;

        setting.lookupValue("id", id);
        setting.lookupValue("font", fontPath);
        setting.lookupValue("text", str);
        setting.lookupValue("textSize", size);

        const libconfig::Setting& posSetting = setting.lookup("pos");
        if (posSetting.getLength() == 2) {
            pos.x = static_cast<float>(posSetting[0]);
            pos.y = static_cast<float>(posSetting[1]);
        }

        const libconfig::Setting& colorSetting = setting.lookup("textColor");
        int r = 255, g = 255, b = 255, a = 255;

        if (colorSetting.getLength() >= 3) {
            r = colorSetting[0];
            g = colorSetting[1];
            b = colorSetting[2];
            if (colorSetting.getLength() == 4) {
                a = colorSetting[3];
            }
        }

        color = sf::Color(
            static_cast<sf::Uint8>(r),
            static_cast<sf::Uint8>(g),
            static_cast<sf::Uint8>(b),
            static_cast<sf::Uint8>(a)
        );
        return std::make_shared<TextElement>(id, fontPath, str, color, pos, size);
    }
    catch (const libconfig::SettingNotFoundException& e) {
        std::cerr << "Erreur : champ manquant dans TextElement (" << e.getPath() << ")" << std::endl;
    }
    catch (const libconfig::SettingTypeException& e) {
        std::cerr << "Erreur : type invalide dans TextElement (" << e.getPath() << ")" << std::endl;
    }

    return nullptr;
}

std::shared_ptr<UIElement> parseRectangleElement(const libconfig::Setting& setting) {
    try {
        std::string id;
        sf::Vector2f pos(0.f, 0.f), size(100.f, 50.f);
        sf::Color innerColor = sf::Color::White;
        sf::Color outerColor = sf::Color::Black;
        float thickness = 2.0f;
        int r = 255, g = 255, b = 255, a = 255;


        setting.lookupValue("id", id);

        if (setting.exists("pos")) {
            const libconfig::Setting& posSetting = setting.lookup("pos");

            if (posSetting.getLength() == 2) {
                float x = static_cast<float>(posSetting[0]);
                float y = static_cast<float>(posSetting[1]);
                pos.x = x;
                pos.y = y;
            }
        }

        if (setting.exists("size")) {
            const libconfig::Setting& sizeSetting = setting.lookup("size");
            if (sizeSetting.getLength() == 2)
                size = { static_cast<float>(sizeSetting[0]), static_cast<float>(sizeSetting[1]) };
        }

        if (setting.exists("innerColor")) {
            const libconfig::Setting& color = setting.lookup("innerColor");

            if (color.getLength() >= 3) {
                r = color[0];
                g = color[1];
                b = color[2];
                if (color.getLength() == 4) {
                    a = color[3];
                }
            }
            innerColor = sf::Color(
                static_cast<sf::Uint8>(r),
                static_cast<sf::Uint8>(g),
                static_cast<sf::Uint8>(b),
                static_cast<sf::Uint8>(a)
            );
        }

        if (setting.exists("outerColor")) {
            const libconfig::Setting& color = setting.lookup("outerColor");
            if (color.getLength() >= 3) {
                r = color[0];
                g = color[1];
                b = color[2];
                if (color.getLength() == 4) {
                    a = color[3];
                }
            }
            outerColor = sf::Color(
                static_cast<sf::Uint8>(r),
                static_cast<sf::Uint8>(g),
                static_cast<sf::Uint8>(b),
                static_cast<sf::Uint8>(a)
            );
        }
        setting.lookupValue("outlineThickness", thickness);
        return std::make_shared<RectangleElement>(id, pos, size, innerColor, outerColor, thickness);
    }
    catch (const libconfig::SettingNotFoundException& e) {
        std::cerr << "RectangleElement: champ manquant (" << e.getPath() << ")\n";
    }
    catch (const libconfig::SettingTypeException& e) {
        std::cerr << "RectangleElement: type invalide (" << e.getPath() << ")\n";
    }

    return nullptr;
}

std::shared_ptr<UIElement> parseCircleElement(const libconfig::Setting& setting) {
    try {
        std::string id;
        sf::Vector2f pos(0.f, 0.f);
        float radius = 50.f;
        sf::Color innerColor = sf::Color::White;
        sf::Color outerColor = sf::Color::Black;
        float thickness = 2.0f;
        int r = 255, g = 255, b = 255, a = 255;

        setting.lookupValue("id", id);

        if (setting.exists("pos")) {
            const libconfig::Setting& posSetting = setting.lookup("pos");
            if (posSetting.getLength() == 2)
                pos = { posSetting[0], posSetting[1] };
        }

        setting.lookupValue("radius", radius);

        if (setting.exists("innerColor")) {
            const libconfig::Setting& color = setting.lookup("innerColor");
            if (color.getLength() >= 3) {
                r = color[0];
                g = color[1];
                b = color[2];
                if (color.getLength() == 4) {
                    a = color[3];
                }
            }
            innerColor = sf::Color(
                static_cast<sf::Uint8>(r),
                static_cast<sf::Uint8>(g),
                static_cast<sf::Uint8>(b),
                static_cast<sf::Uint8>(a)
            );
        }

        if (setting.exists("outerColor")) {
            const libconfig::Setting& color = setting.lookup("outerColor");
            if (color.getLength() >= 3) {
                r = color[0];
                g = color[1];
                b = color[2];
                if (color.getLength() == 4) {
                    a = color[3];
                }
            }
            outerColor = sf::Color(
                static_cast<sf::Uint8>(r),
                static_cast<sf::Uint8>(g),
                static_cast<sf::Uint8>(b),
                static_cast<sf::Uint8>(a)
            );
        }

        setting.lookupValue("outlineThickness", thickness);
        return std::make_shared<CircleElement>(id, pos, radius, innerColor, outerColor, thickness);
    }
    catch (const libconfig::SettingNotFoundException& e) {
        std::cerr << "CircleElement: champ manquant (" << e.getPath() << ")\n";
    }
    catch (const libconfig::SettingTypeException& e) {
        std::cerr << "CircleElement: type invalide (" << e.getPath() << ")\n";
    }

    return nullptr;
}

std::shared_ptr<UIElement> parseTextureElement(const libconfig::Setting& setting) {
    try {
        std::string id;
        std::string texturePath;
        sf::Vector2f pos(0.f, 0.f);
        sf::Vector2f scale(1.f, 1.f);

        setting.lookupValue("id", id);
        setting.lookupValue("texturePath", texturePath);

        if (setting.exists("pos")) {
            const libconfig::Setting& posSetting = setting.lookup("pos");
            if (posSetting.getLength() == 2)
                pos = { static_cast<float>(posSetting[0]), static_cast<float>(posSetting[1]) };
        }

        if (setting.exists("scale")) {
            const libconfig::Setting& scaleSetting = setting.lookup("scale");
            if (scaleSetting.getLength() == 2)
                scale = { static_cast<float>(scaleSetting[0]), static_cast<float>(scaleSetting[1]) };
        }

        return std::make_shared<TextureElement>(id, texturePath, pos, scale);
    }
    catch (const libconfig::SettingNotFoundException& e) {
        std::cerr << "TextureElement: champ manquant (" << e.getPath() << ")\n";
    }
    catch (const libconfig::SettingTypeException& e) {
        std::cerr << "TextureElement: type invalide (" << e.getPath() << ")\n";
    }

    return nullptr;
}

std::shared_ptr<UIElement> parseInputFieldElement(const libconfig::Setting& setting) {
    try {
        std::string id;
        std::string fontPath;
        sf::Vector2f pos(0.f, 0.f), size(100.f, 30.f);
        bool isPassword = false;
        sf::Color activeColor = sf::Color::Blue;
        int r = 255, g = 255, b = 255, a = 255;

        setting.lookupValue("id", id);
        setting.lookupValue("font", fontPath);

        if (setting.exists("pos")) {
            const libconfig::Setting& posSetting = setting.lookup("pos");
            if (posSetting.getLength() == 2)
                pos = { static_cast<float>(posSetting[0]), static_cast<float>(posSetting[1]) };
        }

        if (setting.exists("size")) {
            const libconfig::Setting& sizeSetting = setting.lookup("size");
            if (sizeSetting.getLength() == 2)
                size = { static_cast<float>(sizeSetting[0]), static_cast<float>(sizeSetting[1]) };
        }

        setting.lookupValue("isPassword", isPassword);

        if (setting.exists("activeOutlineColor")) {
            const libconfig::Setting& color = setting.lookup("activeOutlineColor");
            if (color.getLength() >= 3) {
                r = color[0];
                g = color[1];
                b = color[2];
                if (color.getLength() == 4) {
                    a = color[3];
                }
            }
            activeColor = sf::Color(
                static_cast<sf::Uint8>(r),
                static_cast<sf::Uint8>(g),
                static_cast<sf::Uint8>(b),
                static_cast<sf::Uint8>(a)
            );
        }

        return std::make_shared<InputFieldElement>(id, fontPath, pos, size, isPassword, activeColor);
    }
    catch (const libconfig::SettingNotFoundException& e) {
        std::cerr << "InputFieldElement: champ manquant (" << e.getPath() << ")\n";
    }
    catch (const libconfig::SettingTypeException& e) {
        std::cerr << "InputFieldElement: type invalide (" << e.getPath() << ")\n";
    }

    return nullptr;
}

std::shared_ptr<UIElement> parseButtonElement(const libconfig::Setting& setting) {
    try {
        std::string id, text, fontPath;
        sf::Vector2f pos(0,0), size(100,50), textPos(0,0);
        sf::Color normalColor = sf::Color::Green;
        sf::Color hoverColor = sf::Color::Yellow;
        sf::Color clickedColor = sf::Color::Red;
        sf::Color outlineColor = sf::Color::White;
        float outlineThickness = 2.0f;

        unsigned int textSize = 20;
        sf::Color textColor = sf::Color::Black;
        int r = 255, g = 255, b = 255, a = 255;


        setting.lookupValue("id", id);
        setting.lookupValue("text", text);
        setting.lookupValue("font", fontPath);

        setting.lookupValue("textSize", textSize);
        if (setting.exists("textColor")) {
            const auto& c = setting.lookup("textColor");
            if (c.getLength() >= 3) {
                r = c[0];
                g = c[1];
                b = c[2];
                if (c.getLength() == 4) {
                    a = c[3];
                }
            }
            textColor = sf::Color(
                static_cast<sf::Uint8>(r),
                static_cast<sf::Uint8>(g),
                static_cast<sf::Uint8>(b),
                static_cast<sf::Uint8>(a)
            );
        }

        if (setting.exists("pos")) {
            const auto& p = setting.lookup("pos");
            pos = {static_cast<float>(p[0]), static_cast<float>(p[1])};
        }
        if (setting.exists("textPos")) {
            const auto& p = setting.lookup("textPos");
            textPos = {static_cast<float>(p[0]), static_cast<float>(p[1])};
        }
        if (setting.exists("size")) {
            const auto& s = setting.lookup("size");
            size = {static_cast<float>(s[0]), static_cast<float>(s[1])};
        }

        if (setting.exists("colors")) {
            const auto& colors = setting.lookup("colors");
            if (colors.exists("normal")) { 
                const auto& c = colors["normal"]; 
                if (c.getLength() >= 3) {
                    r = c[0];
                    g = c[1];
                    b = c[2];
                    if (c.getLength() == 4) {
                        a = c[3];
                    }
                }
                normalColor = sf::Color(
                    static_cast<sf::Uint8>(r),
                    static_cast<sf::Uint8>(g),
                    static_cast<sf::Uint8>(b),
                    static_cast<sf::Uint8>(a)
                ); 
            }
            if (colors.exists("hover")) { 
                const auto& c = colors["hover"]; 
                if (c.getLength() >= 3) {
                    r = c[0];
                    g = c[1];
                    b = c[2];
                    if (c.getLength() == 4) {
                        a = c[3];
                    }
                }
                hoverColor = sf::Color(
                    static_cast<sf::Uint8>(r),
                    static_cast<sf::Uint8>(g),
                    static_cast<sf::Uint8>(b),
                    static_cast<sf::Uint8>(a)
                ); 
            }
            if (colors.exists("clicked")) { 
                const auto& c = colors["clicked"]; 
                if (c.getLength() >= 3) {
                    r = c[0];
                    g = c[1];
                    b = c[2];
                    if (c.getLength() == 4) {
                        a = c[3];
                    }
                }
                clickedColor = sf::Color(
                    static_cast<sf::Uint8>(r),
                    static_cast<sf::Uint8>(g),
                    static_cast<sf::Uint8>(b),
                    static_cast<sf::Uint8>(a)
                ); 
            }
        }

        if (setting.exists("outline")) {
            const auto& outline = setting.lookup("outline");
            outline.lookupValue("thickness", outlineThickness);
            if (outline.exists("color")) {
                const auto& c = outline.lookup("color");
                if (c.getLength() >= 3) {
                    r = c[0];
                    g = c[1];
                    b = c[2];
                    if (c.getLength() == 4) {
                        a = c[3];
                    }
                }
                outlineColor = sf::Color(
                    static_cast<sf::Uint8>(r),
                    static_cast<sf::Uint8>(g),
                    static_cast<sf::Uint8>(b),
                    static_cast<sf::Uint8>(a)
                ); 
            }
        }

        std::function<void()> clickCb = nullptr;
        std::function<void()> hoverCb = nullptr;
        std::string clickId, hoverId;
        setting.lookupValue("onClick", clickId);
        setting.lookupValue("onHover", hoverId);
        if (ActionRegistry::getInstance().get(clickId)) 
            clickCb = ActionRegistry::getInstance().get(clickId);
        if (ActionRegistry::getInstance().get(hoverId)) 
            hoverCb = ActionRegistry::getInstance().get(hoverId);

        ButtonBuilder builder(id);
        builder.setText(text, fontPath)
               .setTextPos(textPos)
               .setPosition(pos)
               .setSize(size)
               .setColors(normalColor, hoverColor, clickedColor)
               .setOutline(outlineThickness, outlineColor)
               .setTextSize(textSize)
               .setTextColor(textColor)
               .setOnClick(clickCb)
               .setOnHover(hoverCb);

        return std::make_shared<ButtonElement>(builder.build());
    }
    catch (const libconfig::SettingNotFoundException& e) {
        std::cerr << "ButtonElement: champ manquant (" << e.getPath() << ")\n";
    }
    catch (const libconfig::SettingTypeException& e) {
        std::cerr << "ButtonElement: type invalide (" << e.getPath() << ")\n";
    }

    return nullptr;
}

std::shared_ptr<UI> parseUI(const libconfig::Setting& uiSetting)
{
    try {
        std::string uiId;
        uiSetting.lookupValue("id", uiId);

        auto ui = std::make_shared<UI>(uiId);

        if (uiSetting.exists("elements")) {
            const libconfig::Setting& elements = uiSetting.lookup("elements");
            for (int i = 0; i < elements.getLength(); ++i) {
                const libconfig::Setting& el = elements[i];
                std::string type;
                el.lookupValue("type", type);

                std::shared_ptr<UIElement> element = nullptr;

                if (type == "text")
                    element = parseTextElement(el);
                else if (type == "rectangle")
                    element = parseRectangleElement(el);
                else if (type == "circle")
                    element = parseCircleElement(el);
                else if (type == "texture")
                    element = parseTextureElement(el);
                else if (type == "inputfield")
                    element = parseInputFieldElement(el);
                else if (type == "button")
                    element = parseButtonElement(el);

                if (element)
                    ui->addElement(element);
            }
        }

        return ui;
    }
    catch (const libconfig::SettingNotFoundException& e) {
        std::cerr << "parseUI: champ manquant (" << e.getPath() << ")\n";
    }
    catch (const libconfig::SettingTypeException& e) {
        std::cerr << "parseUI: type invalide (" << e.getPath() << ")\n";
    }
    return nullptr;
}

void loadUIFromFile(const std::string& filePath) {
    try {
        libconfig::Config cfg;
        cfg.readFile(filePath.c_str());

        const libconfig::Setting& root = cfg.getRoot();

        for (int i = 0; i < root.getLength(); ++i) {
            const libconfig::Setting& uiSetting = root[i];
            auto ui = parseUI(uiSetting);
            if (ui) {
                UIManager::getInstance().addUI(ui);
            }
        }
    }
    catch (const libconfig::FileIOException& e) {
        std::cerr << "Impossible to read the file: " << filePath << std::endl;
    }
    catch (const libconfig::ParseException& e) {
        std::cerr << "Parsing error in file " << filePath 
                  << " at line " << e.getLine() << ": " << e.getError() << std::endl;
    }
}
