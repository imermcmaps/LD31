#include <iostream>
#include "Factory.hpp"
#include "LevelScene.hpp"
#include "LD31.hpp"
#include "Snowman.hpp"
#include "Cannon.hpp"

int main() {
	engine::Factory::RegisterType("snowman", Snowman::manufacture);
	engine::Factory::RegisterType("cannon", engine::Factory::CreateChildNode<Cannon>);
    LD31 game;
    game.run();
    return 0;
}