/* 
 * File:   Cannon.cpp
 * Author: iMer
 * 
 * Created on 6. Dezember 2014, 08:08
 */

#include "Cannon.hpp"
#include "Engine/Scene.hpp"
#include "Engine/Game.hpp"
#include "Engine/util/math.hpp"
#include "Engine/util/misc.hpp"
#include "Engine/Factory.hpp"
#include <iostream>
#include "Projectile.hpp"
#include "Ui.hpp"
#include "Constants.hpp"
#include "LevelScene.hpp"
#include "Engine/util/Random.hpp"

Cannon::KeyHandler::KeyHandler(Cannon* cannon) : m_cannon(cannon) {

}

void Cannon::KeyHandler::handle(const sf::Event::KeyEvent& e) {
	if (e.code == sf::Keyboard::Space && m_cannon->IsLoaded()) {
		m_cannon->Fire();
	}
}

Cannon::Cannon(engine::Scene* scene) : SpriteNode(scene), m_keyHandler(this) {
	scene->GetGame()->OnKeyDown.AddHandler(&m_keyHandler);
	if (scene->GetType() == NT_LEVELSCENE) {
		static_cast<LevelScene*> (m_scene)->SetCannon(this);
	}
}

Cannon::~Cannon() {
	m_scene->GetGame()->OnKeyDown.RemoveHandler(&m_keyHandler);
	if (m_scene->GetType() == NT_LEVELSCENE) {
		auto s = static_cast<LevelScene*> (m_scene);
		if (s->GetCannon() == this) s->SetCannon(nullptr);
	}
}

void Cannon::Fire() {
	SpriteNode* barrel = static_cast<SpriteNode*> (m_children.front());
	barrel->PlayAnimation("fire");
	auto delta = m_scene->GetGame()->GetMousePosition() - barrel->GetGlobalPosition();
	float angle = atan2(delta.y, delta.x);
	angle += 90;
	if (angle > 180) {
		angle -= 360;
	}
	angle = engine::util::minmax<float>(20, abs(angle), 100)*(angle < 0 ? -1 : 1) - 90;
	auto p = static_cast<Projectile*> (engine::Factory::CreateChildFromFile(m_cannonBall, this));
	if (!p->GetBody()) {
		auto& c = p->GetChildren();
		bool foundBody = false;
		auto rand2 = engine::util::RandomFloat(0.5, 2);
		for (auto it = c.begin(); it != c.end(); it++) {
			if ((*it)->GetBody()) {
				(*it)->GetBody()->SetTransform(b2Vec2(barrel->GetGlobalPosition().x / m_scene->GetPixelMeterRatio(), barrel->GetGlobalPosition().y / m_scene->GetPixelMeterRatio()), angle);
				(*it)->GetBody()->SetLinearVelocity(b2Vec2((*it)->GetBody()->GetLinearVelocity().x * cosf(angle) * rand2(), (*it)->GetBody()->GetLinearVelocity().x * sinf(angle) * rand2()));
				foundBody=true;
			}
		}
		if (!foundBody) {
			std::cerr << "Cannon ball '" << m_cannonBall << "' does not have a body" << std::endl;
			p->Delete();
			return;
		}
	} else {
		p->GetBody()->SetTransform(b2Vec2(barrel->GetGlobalPosition().x / m_scene->GetPixelMeterRatio(), barrel->GetGlobalPosition().y / m_scene->GetPixelMeterRatio()), angle);
		p->GetBody()->SetLinearVelocity(b2Vec2(p->GetBody()->GetLinearVelocity().x * cosf(angle), p->GetBody()->GetLinearVelocity().x * sinf(angle)));
	}
	Ui* ui = static_cast<Ui*> (m_scene->GetUi());
	Ui::Slot* s = ui->GetCurrentSlot();
	s->SetCount(s->GetCount() - 1);
	static_cast<LevelScene*> (m_scene)->AddAmmo(-1);
	if (s->GetCount() <= 0) {
		m_loaded = false;
	}
}

void Cannon::OnUpdate(sf::Time interval) {
	engine::SpriteNode::OnUpdate(interval);
	Node* barrel = m_children.front();
	auto delta = m_scene->GetGame()->GetMousePosition() - barrel->GetGlobalPosition();
	float angle = atan2(delta.y, delta.x);
	angle *= 180 / engine::util::fPI;
	angle += 90;
	if (angle > 180) {
		angle -= 360;
	}
	barrel->setRotation(engine::util::minmax<float>(20, abs(angle), 100)*(angle < 0 ? -1 : 1));
	if (angle < 0) {
		static_cast<engine::SpriteNode*> (barrel)->setOrigin(12, 50);
	} else {
		static_cast<engine::SpriteNode*> (barrel)->setOrigin(21, 50);
	}
}

bool Cannon::initialize(Json::Value& root) {
	if (!engine::SpriteNode::initialize(root)) {
		return false;
	}
	m_cannonBall = root.get("cannonBall", "assets/script/cannon_ball.json").asString();
	return true;
}