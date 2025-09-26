#include <gtest/gtest.h>
#include "../include/systems.hpp"

using namespace std;

class RegistrySystemTest : public testing::Test {
    protected:
        registry reg;
        entity e1 = reg.spawn_entity();
        entity e2 = reg.spawn_entity();
};

TEST_F(RegistrySystemTest, PositionSystem) {
    auto &pos_table = reg.get_components<component::position>();
    auto &vel_table = reg.get_components<component::velocity>();

    // Adding components to entities
    reg.add_component<component::position>(e1, {0, 0});
    reg.add_component<component::velocity>(e1, {5, 3});

    reg.add_component<component::position>(e2, {10, 10});
    reg.add_component<component::velocity>(e2, {-2, 4});

    // Running the position system
    position_system(1, reg, pos_table, vel_table);

    // Checking updated positions
    ASSERT_EQ(pos_table[e1].value().x, 5);
    ASSERT_EQ(pos_table[e1].value().y, 3);

    ASSERT_EQ(pos_table[e2].value().x, 8);
    ASSERT_EQ(pos_table[e2].value().y, 14);
}

TEST_F(RegistrySystemTest, ControlSystem) {
    auto &vel_table = reg.get_components<component::velocity>();
    auto &ctrl_table = reg.get_components<component::controllable>();

    // Adding components to entities
    reg.add_component<component::velocity>(e1, {0, 0});
    reg.add_component<component::controllable>(e1, component::controllable()); 

    reg.add_component<component::velocity>(e2, {0, 0});
    reg.add_component<component::controllable>(e2, component::controllable());

    // Running the control system
    control_system(1, reg, ctrl_table, vel_table);

    // Checking updated velocities
    ASSERT_EQ(vel_table[e1].value().vx, 0);
    ASSERT_EQ(vel_table[e1].value().vy, -1); // Moving up decreases y

    ASSERT_EQ(vel_table[e2].value().vx, 0);
    ASSERT_EQ(vel_table[e2].value().vy, 1); // Moving down increases y
}

TEST_F(RegistrySystemTest, CollisionSystem)
{
    testing::internal::CaptureStdout();

    std::string output;

    reg.add_component<component::position>(e1, {100, 50});
    reg.add_component<component::velocity>(e1, {0, 0});
    reg.add_component<component::hurtbox>(e1, {500, 1, 32, 32});

    reg.add_component<component::position>(e2, {100, 100});
    reg.add_component<component::velocity>(e2, {0, 0});
    reg.add_component<component::hitbox>(e2, {5, 1, 32, 32});

    reg.run_systems(1);
    
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "");
    
    testing::internal::CaptureStdout();
    reg.add_component<component::velocity>(e2, {0, -35});
    
    reg.run_systems(1);
    
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "COLLISION 0 1\n");
}
