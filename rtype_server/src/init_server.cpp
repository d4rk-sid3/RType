/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** init_server
*/

#include "../include/server.hpp"

void load_textures(void)
{
    ResourceManager::Instance().load("assets/sprites/player/player1.gif", "player1", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/player/player2.gif", "player2", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/player/player_up.gif", "player_up", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/player/player_down.gif", "player_down", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/player/player_missile.gif", "player_missile", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/enemies/red_trooper.gif", "red_trooper", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/enemies/walker_walk.gif", "walker", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/enemies/enemy_missile.gif", "enemy_missile", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/effects/Explosion.png", "explosion", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/effects/hit_effect.gif", "hit_effect", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/background/background.jpg", "background", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/background/wall1_shadow.png", "ceiling", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/enemies/boss.gif", "boss", TEXTURE);

    ResourceManager::Instance().load("assets/fonts/ARCADECLASSIC.TTF", "arcade", FONT);
}

void Server::initializeGame(void)
{
    Factory factory(reg);
    factory.make_background();
    player1_entity_id = factory.make_entity("player1");
    player2_entity_id = factory.make_entity("player2");
    factory.make_ceiling();
    factory.make_floor();
    
    auto &pos1 = reg.get_components<component::position>()[player1_entity_id].value();
    auto &pos2 = reg.get_components<component::position>()[player2_entity_id].value();
    pos1.x = 50;
    pos1.y = 250;

    pos2.x = 50;
    pos2.y = 150;
    factory.make_menu_background_music();
}

Server::Server(int p) :
    win(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "R-Type Server"),
    reg(win), server_(p, std::ref(messages), std::ref(mtx)), p_(p)
{
    reg.control_active = false;
    counter = 0;

    load_textures();
    initializeGame();
    loadLevel("assets/levels/test.txt");

    networkThread = std::thread([this]() { server_.run(); });
}

Server::~Server()
{

}

void Server::run()
{
    isRunning = true;

    networkThread = std::thread(
        [this]()
        {
            while (isRunning)
            {
                logGameEntities();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            
        }
    );

    while (win.isOpen()) {
        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                win.close();
            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::Escape)
                    win.close();
        }

        double dt = frameClock.restart().asSeconds();
        reg.run_systems(dt);
        runLevel(dt);

        if (player1_entity_id == -1 && player2_entity_id == -1) {
            printf("GAME OVER\n");
            break;
        }
        if (boss_dead) {
            printf("BOSS DEAD\n");
            break;
        }
    }

    server_.getContext().stop();
    networkThread.join();
    isRunning = false;
}
