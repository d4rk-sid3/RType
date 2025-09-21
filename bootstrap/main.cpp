#include <thread>

#include "registry.hpp"
#include "components.hpp"

void position_system(registry &reg)
{
    auto &positions = reg.get_components<component::position>();
    auto &velocities = reg.get_components<component::velocity>();

        for (size_t i = 0; i < reg.getEntityNum(); ++ i) {
            try {
                auto &pos = positions.at(i);
                auto &vel = velocities.at(i);

                if (pos && vel) {
                    pos.value().setPosition( vel.value().vx,  vel.value().vy);
                    cout << i << " : Position = {" << pos.value().x << " ," << pos.value().y
                            << "}, Velocity = {" << vel.value().vx << " ," << vel.value().vy
                            << "}" << std::endl;
                }
            } catch (...) {
            }
        }
}

void draw_system(registry &reg)
{
    auto &positions = reg.get_components<component::position>();
    auto &draws = reg.get_components<component::drawable>();

    for (size_t i = 0; i < reg.getEntityNum(); ++ i) {
        try {
            auto &pos = positions.at(i);
            auto &draw = draws.at(i);

            if (pos && draw) {
                auto tmp =  draw.value();
                cout << i << " : Position = {" << pos.value().x << " ," << pos.value().y
                        << "}, Velocity = {" << draw.value().vx << " ," << draw.value().vy
                        << "}" << std::endl;
            }
        } catch (...) {
        }
    }
}

int main() {
    registry registry;

    registry.register_components<component::position>();
    registry.register_components<component::velocity>();
    registry.register_components<component::drawable>();
    registry.register_components<component::controllable>();


    entity e1 = registry.spawn_entity();
    entity e2 = registry.spawn_entity();

    registry.add_component<component::position>(e1, {5, 10});
    registry.add_component<component::velocity>(e1, {1, 2});

    registry.add_component<component::position>(e2, {20, 40});
    registry.add_component<component::velocity>(e2, {0, 0});

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Drawable");

    sf::CircleShape circle(50.f);

    circle.setFillColor(sf::Color::Green);

    circle.setPosition(
        (window.getSize().x / 3.f) - circle.getRadius(),
        (window.getSize().y / 3.f) - circle.getRadius()
    );

    while (window.isOpen()) {
        position_system(registry);
        this_thread::sleep_for(chrono::milliseconds((500)));
    }

    return 0;
}
