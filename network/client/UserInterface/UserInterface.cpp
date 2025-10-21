#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <string>


/**
 * @brief Simple Button class for SFML
 */
class Button {
public:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Font font;

    Button(const std::string& label, sf::Vector2f position, sf::Vector2f size) {
        shape.setSize(size);
        shape.setFillColor(sf::Color(100, 100, 100)); // Dark Gray
        shape.setPosition(position);

        text.setString(label);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);

        // Center the text on the button
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(position.x + size.x / 2.0f, position.y + size.y / 2.0f);
    }

    void addFont(const sf::Font& newFont) {
        font = newFont;
        text.setFont(font);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }

    bool isClicked(sf::Vector2i mousePos) {
        return shape.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    }
    
    void setHover(bool isHovered) {
        if (isHovered) {
            shape.setFillColor(sf::Color(150, 150, 150)); // Lighter Gray on hover
        } else {
            shape.setFillColor(sf::Color(100, 100, 100)); // Dark Gray
        }
    }
};

// A more complex class would be needed for a fully functional TextBox (handling keyboard input, cursor, etc.)
// For simplicity, we'll just use a visual representation here.

/**
 * @brief Simple visual placeholder for a Text Box.
 */
class TextBox {
public:
    sf::RectangleShape shape;
    sf::Text label;
    sf::Font font;

    std::string content; // In a real app, you'd handle text input
    sf::Text enteredText; // To display entered text
    bool isActive = false;

    TextBox(const std::string& labelText, sf::Vector2f position, sf::Vector2f size) {
        shape.setSize(size);
        shape.setFillColor(sf::Color::Black);
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(1);
        shape.setPosition(position);

        label.setString(labelText + ":");
        label.setCharacterSize(18);
        label.setFillColor(sf::Color::White);
        label.setPosition(position.x - 120, position.y + (size.y / 2.0f) - 10); // Position label to the left
    }

    void addFont(const sf::Font& newFont) {
        font = newFont;
        label.setFont(font);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(label);
        window.draw(shape);
    }

    bool isClicked(sf::Vector2i mousePos) {
        return shape.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    }

    void setActive(bool active) {
        isActive = active;
        if (isActive) {
            shape.setOutlineColor(sf::Color::Cyan);
        } else {
            shape.setOutlineColor(sf::Color::White);
        }
    }

    bool getActive() const {
        return isActive;
    }
};


// --- Application State Management ---

enum GameState {
    REGISTER,
    LOGIN,
    MENU,
    SETTINGS,
    SESSION_JOIN,
    SESSION_CREATE,
    SESSION_LAUNCH
};

// --- Main Application Class ---

class Application {
public:
    sf::RenderWindow window;
    sf::Font font;
    GameState currentState;

    // GUI Elements for different states
    std::map<std::string, Button> menuButtons;
    std::map<std::string, Button> registerButtons;
    std::map<std::string, TextBox> registerFields;

    Application() : window(sf::VideoMode(800, 600), "SFML Interface"), currentState(LOGIN) {
        if (!font.loadFromFile("Gabriela-Regular.ttf")) {
            std::cerr << "Error loading font." << std::endl;
        }
        
        // Initialize all GUI components
        initializeGUI();
    }

    void initializeGUI() {
        // --- LOGIN/REGISTER SETUP ---
        float centerX = 800 / 2.0f;
        float topY = 150.0f;

        // Register/Login Text Boxes (Used for both)
        registerFields.emplace("username", TextBox("Username", {centerX - 50, topY}, {300, 40}));
        registerFields.at("username").addFont(font);
        registerFields.emplace("password", TextBox("Password", {centerX - 50, topY + 60}, {300, 40}));
        registerFields.at("password").addFont(font);

        // Register/Login Buttons
        registerButtons.emplace("register_submit", Button("S'inscrire", {centerX - 150, topY + 140}, {140, 40}));
        registerButtons.at("register_submit").addFont(font);
        registerButtons.emplace("login_submit", Button("Se Connecter", {centerX + 10, topY + 140}, {140, 40}));
        registerButtons.at("login_submit").addFont(font);
        registerButtons.emplace("to_login", Button("Aller au Login", {centerX - 75, topY + 200}, {150, 30}));
        registerButtons.at("to_login").addFont(font);
        registerButtons.emplace("to_register", Button("S'inscrire", {centerX - 75, topY + 200}, {150, 30}));
        registerButtons.at("to_register").addFont(font);

        // --- MENU SETUP ---
        float menuY = 100.0f;
        float btnWidth = 200.0f;
        float btnHeight = 50.0f;
        float spacing = 20.0f;

        menuButtons.emplace("join", Button("Integrer une session", {centerX - btnWidth/2.0f, menuY}, {btnWidth, btnHeight}));
        menuButtons.at("join").addFont(font);
        menuButtons.emplace("create", Button("Creer une session", {centerX - btnWidth/2.0f, menuY + (btnHeight + spacing)}, {btnWidth, btnHeight}));
        menuButtons.at("create").addFont(font);
        menuButtons.emplace("launch", Button("Lancer une session", {centerX - btnWidth/2.0f, menuY + 2 * (btnHeight + spacing)}, {btnWidth, btnHeight}));
        menuButtons.at("launch").addFont(font);
        menuButtons.emplace("settings", Button("Parametres", {centerX - btnWidth/2.0f, menuY + 3 * (btnHeight + spacing)}, {btnWidth, btnHeight}));
        menuButtons.at("settings").addFont(font);
        menuButtons.emplace("logout", Button("Deconnecter", {centerX - btnWidth/2.0f, menuY + 4 * (btnHeight + spacing)}, {btnWidth, btnHeight}));
        menuButtons.at("logout").addFont(font);
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                handleClick({event.mouseButton.x, event.mouseButton.y});
            }
            
            // Check for hover state changes
            if (event.type == sf::Event::MouseMoved) {
                handleHover({event.mouseMove.x, event.mouseMove.y});
            }
            
            // In a real application, you'd handle sf::Event::TextEntered for the TextBoxes here
            if (event.type == sf::Event::TextEntered) {
                for (auto& pair : registerFields) {
                    if (pair.second.getActive()) {
                        if (event.text.unicode == 8) { // Backspace
                            if (!pair.second.content.empty()) {
                                pair.second.content.pop_back();
                            }
                        } else if (event.text.unicode < 128) { // Basic ASCII
                            pair.second.content += static_cast<char>(event.text.unicode);
                        }
                        // Update visual representation (not implemented for brevity)
                    }
                }
            }
        }
    }
    
    void handleHover(sf::Vector2i mousePos) {
        if (currentState == LOGIN || currentState == REGISTER) {
            for (auto& pair : registerButtons) {
                pair.second.setHover(pair.second.isClicked(mousePos));
            }
        } else if (currentState == MENU) {
             for (auto& pair : menuButtons) {
                pair.second.setHover(pair.second.isClicked(mousePos));
            }
        }
        // ... extend for other states
    }

    void handleClick(sf::Vector2i mousePos) {
        if (currentState == REGISTER) {
            if (registerButtons.at("register_submit").isClicked(mousePos)) {
                std::cout << "Attempting to Register..." << std::endl;
                // In a real app: Send data to server, if successful:
                currentState = LOGIN; 
            } else if (registerButtons.at("to_login").isClicked(mousePos)) {
                currentState = LOGIN;
            } else if (registerFields.at("username").isClicked(mousePos)) {
                registerFields.at("username").setActive(true);
                registerFields.at("password").setActive(false);
            } else if (registerFields.at("password").isClicked(mousePos)) {
                registerFields.at("username").setActive(false);
                registerFields.at("password").setActive(true);  
            }
        }
        else if (currentState == LOGIN) {
            if (registerButtons.at("login_submit").isClicked(mousePos)) {
                std::cout << "Attempting to Login..." << std::endl;
                // In a real app: Send data to server, if successful:
                currentState = MENU; 
            } else if (registerButtons.at("to_register").isClicked(mousePos)) {
                currentState = REGISTER;
            }
        } 
        else if (currentState == MENU) {
            if (menuButtons.at("join").isClicked(mousePos)) {
                std::cout << "Go to 'Integrer une session'" << std::endl;
                // currentState = SESSION_JOIN;
            } else if (menuButtons.at("create").isClicked(mousePos)) {
                std::cout << "Go to 'Creer une session'" << std::endl;
                // currentState = SESSION_CREATE;
            } else if (menuButtons.at("launch").isClicked(mousePos)) {
                std::cout << "Go to 'Lancer une session'" << std::endl;
                // currentState = SESSION_LAUNCH;
            } else if (menuButtons.at("settings").isClicked(mousePos)) {
                currentState = SETTINGS;
            } else if (menuButtons.at("logout").isClicked(mousePos)) {
                std::cout << "Logged out." << std::endl;
                currentState = LOGIN; 
            }
        }
        else if (currentState == SETTINGS) {
            // Logic for settings buttons (not implemented for brevity)
        }
    }

    void drawCurrentState() {
        if (currentState == REGISTER) {
            drawRegisterInterface();
        } else if (currentState == LOGIN) {
            drawLoginInterface();
        } else if (currentState == MENU) {
            drawMenu();
        } else if (currentState == SETTINGS) {
            drawSettings();
        }
        // ... extend for other states
    }

    void drawTitle(const std::string& title) {
        sf::Text titleText(title, font, 36);
        titleText.setFillColor(sf::Color::Cyan);
        sf::FloatRect textRect = titleText.getLocalBounds();
        titleText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        titleText.setPosition(800 / 2.0f, 50.0f);
        window.draw(titleText);
    }
    
    // --- DRAWING FUNCTIONS FOR EACH INTERFACE ---

    void drawRegisterInterface() {
        drawTitle("Interface de Registration");
        
        registerFields.at("username").draw(window);
        registerFields.at("password").draw(window);
        
        registerButtons.at("register_submit").draw(window);
        registerButtons.at("to_login").draw(window); // Option to switch
    }

    void drawLoginInterface() {
        drawTitle("Interface de Login");
        
        registerFields.at("username").draw(window);
        registerFields.at("password").draw(window);
        
        registerButtons.at("login_submit").draw(window);
        registerButtons.at("to_register").draw(window); // Option to switch
    }

    void drawMenu() {
        drawTitle("Menu Principal");
        
        menuButtons.at("join").draw(window);
        menuButtons.at("create").draw(window);
        menuButtons.at("launch").draw(window);
        menuButtons.at("settings").draw(window);
        
        // Disconnect button at the bottom
        menuButtons.at("logout").draw(window);
    }

    void drawSettings() {
        drawTitle("Parametres");
        
        // Simple visual placeholders for settings
        sf::Text soundText("Son: ON/OFF (Needs a Slider/Checkbox)", font, 24);
        soundText.setFillColor(sf::Color::White);
        soundText.setPosition(100, 150);
        window.draw(soundText);
        
        sf::Text nameText("Changer Nom: (Needs a Text Box)", font, 24);
        nameText.setFillColor(sf::Color::White);
        nameText.setPosition(100, 200);
        window.draw(nameText);
        
        // A simple back button
        Button backButton("Retour", {50, 500}, {100, 40});
        if (backButton.isClicked(sf::Mouse::getPosition(window))) {
            backButton.setHover(true);
        } else {
             backButton.setHover(false);
        }
        backButton.draw(window);
        
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
             if (backButton.isClicked(sf::Mouse::getPosition(window))) {
                currentState = MENU;
             }
        }
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();

            window.clear(sf::Color(50, 50, 50)); // Dark background
            
            drawCurrentState();

            window.display();
        }
    }
};

int main() {
    Application app;
    app.run();
    return 0;
}