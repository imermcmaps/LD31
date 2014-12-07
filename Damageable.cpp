#include "Damageable.hpp"
#include <Scene.hpp>
Damageable::ContactHandler::ContactHandler(Damageable* damageable) : m_damageable(damageable) {

}

void Damageable::ContactHandler::handle(b2Contact* contact, const b2ContactImpulse* impulse) {
	engine::Node* a = static_cast<engine::Node*> (contact->GetFixtureA()->GetBody()->GetUserData());
	engine::Node* b = static_cast<engine::Node*> (contact->GetFixtureB()->GetBody()->GetUserData());

	Damageable* damageable = nullptr;
	engine::Node* other = nullptr;
	if (a == m_damageable) {
		damageable = static_cast<Damageable*> (a);
		other = b;
	} else if (b == m_damageable) {
		damageable = static_cast<Damageable*> (b);
		other = a;
	}
	if (damageable && other->GetIdentifier() != "particle") {
		float force = 0;
		for (uint32_t i = 0; i < impulse->count; i++) {
			force += impulse->normalImpulses[i];
		}
		if (abs(force) > 0.2) {
			b2WorldManifold worldManifold;
			contact->GetWorldManifold(&worldManifold);
			damageable->Damage(abs(force), other, worldManifold);
		}
	}
}

Damageable::Damageable(engine::Scene* scene) : SpriteNode(scene), m_contactHandler(this) {
	m_scene->OnContactPostSolve.AddHandler(&m_contactHandler);
}

Damageable::~Damageable() {
	m_scene->OnContactPostSolve.RemoveHandler(&m_contactHandler);
}

void Damageable::Damage(float damage, Node* by, const b2WorldManifold& manifold){
	m_health-=damage;
	if (m_health < 0){
		Delete();
	}
}