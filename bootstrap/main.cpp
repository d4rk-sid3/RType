#include <thread>

#include "registry.hpp"
#include "components.hpp"

int main() {

    registry registry;
    
    sf::RenderWindow &window = registry.get_window().value();
    sf::Event &event = registry.get_event().value();

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

    window.setFramerateLimit(100);

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        registry.run_systems();
    }

    return 0;
}
