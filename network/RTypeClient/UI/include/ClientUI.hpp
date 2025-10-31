#ifndef CLIENTUI
    #define CLIENTUI
    #include "./librairies.hpp"
    #include "EventHandler.hpp"
    #include "../components/include/TextElement.hpp"
    #include "../include/utils.hpp"

class ClientUI {
    public:
        sf::RenderWindow window;
        EventHandler eventHandler;
        sf::Clock clock;
        float deltaTime;
        std::shared_ptr<TextElement> hour;
        std::shared_ptr<TextElement> message_login;
        std::shared_ptr<TextElement> message_register;

        void init_date(void);

        void init_hour(void);

        void init(void);
        void loadPages();
        ~ClientUI();
        ClientUI();
};
#endif