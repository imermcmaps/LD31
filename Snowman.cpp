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

Snowman::BodyPart::BodyPart(engine::Scene* scene) : Damageable(scene), m_dead(false) {
	m_health = 1.5;
}

void Snowman::BodyPart::Damage(float damage, Node* by, const b2WorldManifold& manifold) {
	if (m_dead || (by && by->GetType() == NT_SNOWMANPART)) {
		return;
	}
	std::cout << damage << std::endl;
	for (uint32_t o = 0; o < 5; o++) {
		AddParticle(manifold.points[0], damage);
	}
	m_health -= damage;
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

engine::Node* Snowman::manufacture(Json::Value& root, engine::Node* parent) {
	Snowman* s = new Snowman(parent->GetScene());
	if (root["position"].isArray()) {
		s->Initialize(root["position"].get(0u, 0).asFloat(), root["position"].get(1u, 0).asFloat());
	} else if (root["position"].isObject()) {
		s->Initialize(root["position"].get("x", 0).asFloat(), root["position"].get("y", 0).asFloat());
	}
	if (root["sprite"]["flipped"].isBool()){
		s->SetFlipped(root["sprite"].get("flipped", false).asBool());
	}
	return s;
}

Snowman::Snowman(engine::Scene* scene) : Node(scene), m_hat(nullptr), m_head(nullptr), m_middle(nullptr), m_bottom(nullptr), m_initialized(false) {
}

Snowman::~Snowman() {
	
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

