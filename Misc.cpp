#include "Misc.hpp"
#include <Engine/Factory.hpp>
engine::Node* CreateExplosion(engine::Scene* scene, float force, const sf::Vector2f& pos, float sizeScale,  uint8_t numParticles) {
	engine::SpriteNode* explosion = static_cast<engine::SpriteNode*> (engine::Factory::CreateChildFromFile("assets/script/explosion.json", scene));
	explosion->setScale(sizeScale, sizeScale);
	explosion->setPosition(pos);
	explosion->GetAnimation()->OnOver = [explosion]() {
		explosion->Delete();
	};
	Json::Value ep;
	if (engine::Factory::LoadJson("assets/script/shockwave_particle.json", ep)) {
		for (uint32_t i = 0; i < numParticles; i++) {
			float angle = (i / static_cast<float>(numParticles)) * 360.0 * engine::util::fPI / 180.0;
			b2Vec2 rayDir(sinf(angle), cosf(angle));
			engine::Node* particle = engine::Factory::CreateChild(ep, explosion);
			explosion->AddNode(particle);
			particle->SetPosition(pos.x, pos.y);
			particle->GetBody()->SetLinearVelocity(force * rayDir);
		}
	}
	return explosion;
}