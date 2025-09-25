#include <gtest/gtest.h>

#include "../include/components.hpp"
#include "../include/registry.hpp"

class registryTest : public testing::Test {
  protected:
    registry reg;
    registry emptyReg;
};

TEST_F(registryTest, collisionTest) {
    testing::internal::CaptureStdout();

    std::string output;
    entity e1 = reg.spawn_entity();
    entity e2 = reg.spawn_entity();

    reg.add_component<component::position>(e1, {100, 50});
    reg.add_component<component::velocity>(e1, {0, 0});
    reg.add_component<component::hurtbox>(e1, {500, 1, 32, 32});

    reg.add_component<component::position>(e2, {100, 100});
    reg.add_component<component::velocity>(e2, {0, 0});
    reg.add_component<component::hitbox>(e2, {5, 1, 32, 32});

    reg.run_systems();

    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "");

    testing::internal::CaptureStdout();
    reg.add_component<component::velocity>(e2, {0, -35});

    reg.run_systems();

    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "COLLISION 0 1\n");
}
