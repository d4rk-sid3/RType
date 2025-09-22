#include <thread>

#include "registry.hpp"
#include "components.hpp"

int main() {
    registry registry;


    entity e1 = registry.spawn_entity();
    entity e2 = registry.spawn_entity();

    registry.add_component<component::position>(e1, {5, 5});
    registry.add_component<component::velocity>(e1, {1, 1});

    // auto &sprite = registry.add_component<component::drawable>(e1, component::drawable());

    // sprite.setTextureFromPath("leaf.png");
    // registry.add_component<component::position>(e2, {20, 40});
    // registry.add_component<component::velocity>(e2, {0, 0});

    // sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Drawable");

    registry.run_systems();
    registry.run_systems();
    registry.run_systems();

    registry.add_component<component::position>(e2, {10, 10});
    registry.add_component<component::velocity>(e2, {100, 100});

    registry.run_systems();
    registry.run_systems();
    registry.run_systems();

    return 0;
}
