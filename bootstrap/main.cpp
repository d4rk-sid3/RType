#include "registry.hpp"
#include "components.hpp"

void position_system(registry &reg)
{
    const auto &positions = reg.get_components<component::position>();
    const auto &velocities = reg.get_components<component::velocity>();


    for (size_t i = 0; i < reg.getEntityNum(); ++ i) {
        const auto &pos = i < positions.size() ? positions[i] : nullopt;
        const auto &vel = i < velocities.size() ? velocities[i] : nullopt;

        if (pos && vel) {
            std::cerr << i << " : Position = {" << pos.value().x << " ," << pos.value().y
                << "}, Velocity = {" << vel.value().vx << " ," << vel.value().vy
                << "}" << std::endl;
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

    position_system(registry);

    return 0;
}
