#include <iostream>
#include "Factory.hpp"
#include "LevelScene.hpp"
#include "LD31.hpp"
#include "Snowman.hpp"

int main() {
	engine::Factory::RegisterType("snowman", Snowman::manufacture);
    LD31 game;
    game.run();
    return 0;
}