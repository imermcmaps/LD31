#include <iostream>
#include "Factory.hpp"
#include "LevelScene.hpp"
#include "LD31.hpp"
#include "Snowman.hpp"
#include "Cannon.hpp"
#include "Projectile.hpp"
#include "Explosive.hpp"

int main() {
	engine::Factory::RegisterType("snowman", Snowman::manufacture);
	engine::Factory::RegisterType("cannon", engine::Factory::CreateChildNode<Cannon>);
	engine::Factory::RegisterType("snowmanPart", engine::Factory::CreateChildNode<Snowman::BodyPart>);
	engine::Factory::RegisterType("projectile", engine::Factory::CreateChildNode<Projectile>);
	engine::Factory::RegisterType("explosive", engine::Factory::CreateChildNode<Explosive>);
    LD31 game;
    game.run();
    return 0;
}