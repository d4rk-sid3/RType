class ResourceRegistry {
    public:
        static ResourceRegistry& instance() {
            static ResourceRegistry instance;
            return instance;
        }
    
        TextManager text;
        SongManager song;
        TextureManager texture;
        ButtonManager button;
        InputHandler input;
    };

    template<typename Resource>
class ResourceManager {
public:
    void add(const std::string& id, const std::string& path) {
        Resource res;
        if (!res.loadFromFile(path))
            throw std::runtime_error("Failed to load " + path);
        resources_[id] = std::move(res);
    }

    Resource& get(const std::string& id) {
        return resources_.at(id);
    }

protected:
    std::unordered_map<std::string, Resource> resources_;
};

class TextureManager : public ResourceManager<sf::Texture> {};
class SongManager : public ResourceManager<sf::Music> {};
class FontManager : public ResourceManager<sf::Font> {};

class Button {
    public:
        Button(const sf::Vector2f& pos, const sf::Vector2f& size, const std::string& label);
    
        void setCallback(std::function<void()> onClick);
        void handleEvent(const sf::Event& event);
        void draw(sf::RenderWindow& window);
        bool isHovered(const sf::Vector2f& mousePos) const;
    
    private:
        sf::RectangleShape shape_;
        sf::Text text_;
        std::function<void()> onClick_;
        bool hovered_ = false;
    };

    class ButtonManager {
        public:
            void addButton(const std::string& id, Button button) {
                buttons_[id] = std::move(button);
            }
            void handleEvent(const sf::Event& e);
            void draw(sf::RenderWindow& window);
            std::string capture_string();
        private:
            std::unordered_map<std::string, Button> buttons_;
        };
        class InputHandler {
            public:
                void processEvent(const sf::Event& event);
                bool isKeyPressed(sf::Keyboard::Key key) const;
                bool isMousePressed(sf::Mouse::Button button) const;
                sf::Vector2i getMousePosition() const;
            
            private:
                std::unordered_map<sf::Keyboard::Key, bool> keyStates_;
                std::unordered_map<sf::Mouse::Button, bool> mouseStates_;
            };
                    

            class UIScreen {
                public:
                    virtual ~UIScreen() = default;
                    virtual void handleEvent(const sf::Event& event) = 0;
                    virtual void update(float dt) = 0;
                    virtual void draw(sf::RenderWindow& window) = 0;
                };

                
            class LoginScreen : public UIScreen {
                public:
                    LoginScreen();
                    void handleEvent(const sf::Event& e) override;
                    void update(float dt) override;
                    void draw(sf::RenderWindow& window) override;
                };
            
            class InterfaceManager {
                public:
                    void setScreen(std::unique_ptr<UIScreen> screen) {
                        currentScreen_ = std::move(screen);
                    }
                    void handleEvent(const sf::Event& e) { if (currentScreen_) currentScreen_->handleEvent(e); }
                    void update(float dt) { if (currentScreen_) currentScreen_->update(dt); }
                    void draw(sf::RenderWindow& win) { if (currentScreen_) currentScreen_->draw(win); }
                private:
                    std::unique_ptr<UIScreen> currentScreen_;
                };
            
                uiManager.setScreen(std::make_unique<DashboardScreen>());

                    