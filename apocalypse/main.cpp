/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Mon, Nov, 2025                                                     *
 * Title           - G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     -                                                                    *
 *     main                                                                             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *             ███████╗██████╗ ██╗████████╗███████╗ ██████╗██╗  ██╗                     *
 *             ██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝██║  ██║                     *
 *             █████╗  ██████╔╝██║   ██║   █████╗  ██║     ███████║                     *
 *             ██╔══╝  ██╔═══╝ ██║   ██║   ██╔══╝  ██║     ██╔══██║                     *
 *             ███████╗██║     ██║   ██║   ███████╗╚██████╗██║  ██║                     *
 *             ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚══════╝ ╚═════╝╚═╝  ╚═╝                     *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "registry.hpp"

using namespace component;

int player_entity_id = -1;

entity make_player(registry& reg) {
    entity player = reg.spawn_entity();
    reg.add_component<position>(player, position({300, 200}));
    reg.add_component<velocity>(player, velocity({0, 0}));
    reg.add_component<hurtbox>(player, hurtbox({1, 1, 32, 32}));
    drawable &sprite = reg.add_component<drawable>(player, drawable());
    sprite.setTextureFromName("player");
    reg.add_component<controllable>(player, controllable());

    return player;
}

entity make_background(registry& reg) {
    entity background_id = reg.spawn_entity();

    reg.add_component<position>(background_id, position({0, 0}));
    auto& background_sprite = reg.add_component<component::drawable>(
        background_id, component::drawable()
    );
    background_sprite.setTextureFromName("background");

    return background_id;
}

void initializeGame(registry &reg) {
    player_entity_id = make_player(reg);
    make_background(reg);
}

void loadTextures()
{
    ResourceManager::Instance().load("assets/sprites/apocalypse/background_apo.png", "background", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/apocalypse/player.png","player",  TEXTURE);
    ResourceManager::Instance().load("assets/sprites/apocalypse/platform.png", "platform", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/apocalypse/asteroid.png", "asteroid",  TEXTURE);
    ResourceManager::Instance().load("assets/sprites/apocalypse/explosion.png", "explosion", TEXTURE);
}

int main() {
    sf::RenderWindow win(sf::VideoMode(768, 432), "Apocalypse");
    sf::Event event;
    sf::Clock clock;
    registry reg(win);

    reg.toggleMovement();
    reg.toggleRendering();
    reg.toggleControl();
    reg.togglePhysics();

    loadTextures();
    initializeGame(reg);

    while (win.isOpen()) {
        double delta = clock.restart().asSeconds();
        while (win.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                win.close();
        }

        reg.run_systems(delta);
    }
    
    return 0;
}