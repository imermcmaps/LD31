/* 
 * File:   Cannon.cpp
 * Author: iMer
 * 
 * Created on 6. Dezember 2014, 08:08
 */

#include "Cannon.hpp"
#include "Scene.hpp"
#include "Game.hpp"
#include "util/math.hpp"
#include "util/misc.hpp"
#include "Factory.hpp"
#include <iostream>
#include "Projectile.hpp"

Cannon::MouseHandler::MouseHandler(Cannon* cannon) : m_cannon(cannon) {

}

void Cannon::MouseHandler::handle(const sf::Event::MouseButtonEvent& e) {
	if (e.button == sf::Mouse::Left && m_cannon->IsLoaded()) {
		m_cannon->Fire();
	}
}

Cannon::Cannon(engine::Scene* scene) : SpriteNode(scene), m_mouseHandler(this) {
	scene->GetGame()->OnMouseClick.AddHandler(&m_mouseHandler);
}

Cannon::~Cannon() {
	m_scene->GetGame()->OnMouseClick.RemoveHandler(&m_mouseHandler);
}

void Cannon::Fire() {
	SpriteNode* barrel = static_cast<SpriteNode*> (m_children.front());
	barrel->PlayAnimation("fire");
	auto delta = m_scene->GetGame()->GetMousePosition() - barrel->GetGlobalPosition();
	float angle = engine::util::minmax<float>(-80*engine::util::fPI/180.0,atan2(delta.y, delta.x), 5*engine::util::fPI/180.0);
	auto p = static_cast<Projectile*> (engine::Factory::CreateChildFromFile(m_cannonBall, this->GetParent()));
	if (!p->GetBody()) {
		std::cerr << "Cannon ball '" << m_cannonBall << "' does not have a body" << std::endl;
		p->Delete();
		return;
	}
	p->GetBody()->SetTransform(b2Vec2(barrel->GetGlobalPosition().x/m_scene->GetPixelMeterRatio(), barrel->GetGlobalPosition().y/m_scene->GetPixelMeterRatio()), angle);
	p->GetBody()->SetLinearVelocity(b2Vec2(p->GetBody()->GetLinearVelocity().x * cosf(angle), p->GetBody()->GetLinearVelocity().x * sinf(angle)));
	m_loaded=false;
	OnFire.Fire(this);
}

void Cannon::OnUpdate(sf::Time interval) {
	engine::SpriteNode::OnUpdate(interval);
	Node* barrel = m_children.front();
	auto delta = m_scene->GetGame()->GetMousePosition() - barrel->GetGlobalPosition();
	float angle = atan2(delta.y, delta.x);
	angle *= 180 / engine::util::fPI;
	angle += 90;
	barrel->setRotation(engine::util::minmax<float>(15, angle, 100));
}

bool Cannon::initialize(Json::Value& root) {
	if (!engine::SpriteNode::initialize(root)) {
		return false;
	}
	m_cannonBall = root.get("cannonBall", "assets/script/cannon_ball.json").asString();
	return true;
}