#ifndef CLIENTUI
    #define CLIENTUI
    #include "interface.hpp"

class ClientUI {
    public:
        sf::RenderWindow window;
        EventHandler eventHandler;
        sf::Clock clock;
        float deltaTime;
        std::shared_ptr<TextElement> hour;

        void init_date(void);

        void init_hour(void);

        void init(void);
        ~ClientUI();
        ClientUI();
};
#endif