/* 
 * File:   Snowman.cpp
 * Author: iMer
 * 
 * Created on 6. Dezember 2014, 03:50
 */

#include "Snowman.hpp"
#include "Factory.hpp"
#include "Constants.hpp"
#include "LevelScene.hpp"
#include "util/Random.hpp"

Snowman::BodyPart::BodyPart(engine::Scene* scene) : SpriteNode(scene), m_health(0.8), m_dead(false) {

}

void Snowman::BodyPart::Damage(float impact) {
	if (m_dead) {
		return;
	}
	m_health -= impact;
	if (m_health < 0) {
		engine::SpriteNode* explosion = static_cast<engine::SpriteNode*> (engine::Factory::CreateChildFromFile("assets/script/explosion.json", GetScene()));
		explosion->setScale(GetSize().x * 2 / explosion->GetSize().x, GetSize().x * 2 / explosion->GetSize().x);
		explosion->setPosition(GetGlobalPosition());
		explosion->GetAnimation()->OnOver = [explosion]() {
			explosion->Delete();
		};

		PlayAnimation("death");
		GetAnimation()->OnOver = [this]() {
			Delete();
		};
		m_dead = true;
	}
}

void Snowman::BodyPart::AddParticle(const b2Vec2& point, float force) {
	ParticleDef p;
	p.point = point;
	p.force = force;
	m_particles.push_back(p);
}

void Snowman::BodyPart::OnUpdate(sf::Time interval) {
	engine::SpriteNode::OnUpdate(interval);
	while (!m_particles.empty()) {
		const ParticleDef& p = m_particles.back();
		engine::SpriteNode* particle = static_cast<engine::SpriteNode*> (engine::Factory::CreateChildFromFile("assets/script/snow_particle.json", GetScene()));
		particle->GetBody()->SetTransform(p.point, 0);
		auto rand = engine::util::RandomFloat(-0.01, 0.01);
		particle->GetBody()->ApplyForceToCenter(b2Vec2(rand(), rand()) , true);
		particle->GetAnimation()->OnOver = [particle]() {
			particle->Delete();
		};
		m_particles.pop_back();
	}
}

Snowman::ContactHandler::ContactHandler(Snowman* snowman) : m_snowman(snowman) {

}

void Snowman::ContactHandler::handle(b2Contact* contact, const b2ContactImpulse* impulse) {
	engine::Node* a = static_cast<engine::Node*> (contact->GetFixtureA()->GetBody()->GetUserData());
	engine::Node* b = static_cast<engine::Node*> (contact->GetFixtureB()->GetBody()->GetUserData());

	Snowman::BodyPart* snowman = nullptr;
	engine::Node* other = nullptr;
	if (a && 
			a->GetParent() == m_snowman &&
			b->GetParent()->GetType() != NT_SNOWMAN) {
		snowman = static_cast<Snowman::BodyPart*> (a);
		other = b;
	} else if (b && b->GetParent() == m_snowman && b->GetParent()->GetType() != NT_SNOWMAN) {
		snowman = static_cast<Snowman::BodyPart*> (b);
		other = a;
	}
	if (snowman && other->GetIdentifier() != "particle") {
		float force = 0;
		for (uint32_t i = 0; i < impulse->count; i++) {
			force += impulse->normalImpulses[i];
		}
		if (abs(force) > 0.05) {
			b2WorldManifold worldManifold;
			contact->GetWorldManifold(&worldManifold);
			for (uint32_t i = 0; i < contact->GetManifold()->pointCount; i++) {
				for (uint32_t o = 0; o < (std::min(10*contact->GetManifold()->pointCount,50)/contact->GetManifold()->pointCount); o++) {
					snowman->AddParticle(worldManifold.points[i], force);
				}
			}
			snowman->Damage(abs(force));
		}
	}
}

engine::Node* Snowman::manufacture(Json::Value& root, engine::Node* parent) {
	Snowman* s = new Snowman(parent->GetScene());
	if (root["position"].isArray()) {
		s->Initialize(root["position"].get(0u, 0).asFloat(), root["position"].get(1u, 0).asFloat());
	} else if (root["position"].isObject()) {
		s->Initialize(root["position"].get("x", 0).asFloat(), root["position"].get("y", 0).asFloat());
	}
	if (parent->GetType() == NT_LEVELSCENE) {
		LevelScene* level = static_cast<LevelScene*> (parent);
	} else {
		std::cerr << "Creating snowman with non-levelscene as a parent. Stuff might not work correctly." << std::endl;
	}
	return s;
}

Snowman::Snowman(engine::Scene* scene) : Node(scene), m_hat(nullptr), m_head(nullptr), m_middle(nullptr), m_bottom(nullptr), m_initialized(false), m_contactHandler(this) {
	m_scene->OnContactPostSolve.AddHandler(&m_contactHandler);
}

Snowman::~Snowman() {
	m_scene->OnContactPostSolve.RemoveHandler(&m_contactHandler);
}

void Snowman::Initialize(float x, float y) {
	if (m_initialized) {
		std::cerr << "Warning: Initialized snowman multiple times" << std::endl;
	}
	m_initialized = true;
	m_bottom = static_cast<BodyPart*> (engine::Factory::CreateChildFromFile("assets/script/snowman_bottom.json", this));
	m_middle = static_cast<BodyPart*> (engine::Factory::CreateChildFromFile("assets/script/snowman_middle.json", this));
	m_head = static_cast<BodyPart*> (engine::Factory::CreateChildFromFile("assets/script/snowman_head.json", this));
	m_hat = static_cast<BodyPart*> (engine::Factory::CreateChildFromFile("assets/script/snowman_hat.json", this));
	m_bottom->SetPosition(x + m_bottom->GetSize().x / 2, y - m_bottom->GetSize().y / 2);
	y -= m_bottom->GetSize().y;
	m_middle->SetPosition(x + m_bottom->GetSize().x / 2, y - m_middle->GetSize().y / 2);
	y -= m_middle->GetSize().y;
	m_head->SetPosition(x + m_bottom->GetSize().x / 2, y - m_head->GetSize().y / 2);
	y -= m_head->GetSize().y;
	m_hat->SetPosition(x + m_bottom->GetSize().x / 2, y - m_hat->GetSize().y / 2);
	static_cast<LevelScene*>(m_scene)->AddTarget(4);
}

void Snowman::OnRemoveNode(Node* node) {
	if (node == m_hat)
		m_hat = nullptr;
	if (node == m_head)
		m_head = nullptr;
	if (node == m_middle)
		m_middle = nullptr;
	if (node == m_bottom)
		m_bottom = nullptr;
	if (m_scene->GetType() == NT_LEVELSCENE) {
		static_cast<LevelScene*>(m_scene)->AddScore(10);
		static_cast<LevelScene*>(m_scene)->AddTarget(-1);
	}

	if (!m_hat && !m_head && !m_middle && !m_bottom) {
		Delete();
	}
}

