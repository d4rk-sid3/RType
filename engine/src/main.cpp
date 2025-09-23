#include <thread>
#include "registry.hpp"
#include "components.hpp"

int main() {

    sf::RenderWindow win(sf::VideoMode(800, 600), "SFML Drawable");

    registry registry(win);


    entity e1 = registry.spawn_entity();
    entity e2 = registry.spawn_entity();
    entity e3 = registry.spawn_entity();

    registry.add_component<component::position>(e1, {265, 265});
    registry.add_component<component::velocity>(e1, {0, 0});
    auto &sprite1 = registry.add_component<component::drawable>(e1, component::drawable());
    sprite1.setTextureFromPath("leaf.png");
    registry.add_component<component::controllable>(e1, component::controllable(true, true, false, false, false));
    
    registry.add_component<component::position>(e2, {100, 50});
    registry.add_component<component::velocity>(e2, {0, 0});
    auto &sprite2 = registry.add_component<component::drawable>(e2, component::drawable());
    sprite2.setTextureFromPath("cloud.png");
    registry.add_component<component::controllable>(e2, component::controllable(false, false, true, true, false));
    
    registry.add_component<component::position>(e3, {300, 300});
    registry.add_component<component::velocity>(e3, {0, 0});
    auto &sprite3 = registry.add_component<component::drawable>(e3, component::drawable());
    sprite3.setTextureFromPath("cloud.png");
    registry.add_component<component::controllable>(e3, component::controllable(false, false, true, true, false));

    win.setFramerateLimit(100);

    sf::Event event;
    while (win.isOpen())
    {
        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                win.close();
        }
        registry.run_systems();
    }

    this_thread::sleep_for(chrono::seconds(3));
    return 0;
}
