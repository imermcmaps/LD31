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

Snowman::BodyPart::BodyPart(engine::Scene* scene) : SpriteNode(scene), m_health(1), m_dead(false), m_deathCounter(0) {

}

void Snowman::BodyPart::Damage(float impact) {
	m_health -= impact;
	if (m_health < 0) {
		engine::SpriteNode* explosion = static_cast<engine::SpriteNode*> (engine::Factory::CreateChildFromFile("assets/script/explosion.json", GetScene()));
		explosion->setScale(GetSize().x * 3 / explosion->GetSize().x, GetSize().x * 3 / explosion->GetSize().x);
		explosion->setPosition(GetGlobalPosition());
		explosion->GetAnimation()->OnOver = [explosion]() {
			explosion->Delete();
		};
		m_dead=true;
		m_deathCounter=0.3f;
	}
}
void Snowman::BodyPart::OnUpdate(sf::Time interval){
	if (m_dead){
		m_deathCounter-=interval.asSeconds();
		if (m_deathCounter < 0){
			Delete();
		}
	}
}

Snowman::ContactHandler::ContactHandler(Snowman* snowman) : m_snowman(snowman) {

}

void Snowman::ContactHandler::handle(b2Contact* contact, const b2ContactImpulse* impulse) {
	engine::Node* a = static_cast<engine::Node*> (contact->GetFixtureA()->GetBody()->GetUserData());
	engine::Node* b = static_cast<engine::Node*> (contact->GetFixtureB()->GetBody()->GetUserData());

	Snowman::BodyPart* snowman = nullptr;
	engine::Node* other = nullptr;
	if (a && a->GetParent() == m_snowman && b->GetParent()->GetType() != NT_SNOWMAN) {
		snowman = static_cast<Snowman::BodyPart*> (a);
		other = b;
	} else if (b && b->GetParent() == m_snowman && b->GetParent()->GetType() != NT_SNOWMAN) {
		snowman = static_cast<Snowman::BodyPart*> (b);
		other = a;
	}
	if (snowman) {
		float force = 0;
		for (uint32_t i = 0; i < impulse->count; i++) {
			force += impulse->normalImpulses[i];
		}
		if (abs(force) > 0.3) {
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
		level->SetSnowman(s);
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
	std::cout << "Init: " << x << ", " << y << std::endl;
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
	
	if (!m_hat && !m_head && !m_middle && !m_bottom){
		if (m_scene->GetType() == NT_LEVELSCENE){
			static_cast<LevelScene*>(m_scene)->SetSnowman(nullptr);
		}
		Delete();
	}
}

