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

        void init_date(void);

        void init_hour(void);

        void init(void);
        ~ClientUI();
        ClientUI();
};
#endif