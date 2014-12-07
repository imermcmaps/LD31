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
#include "Ui.hpp"
#include "Constants.hpp"
#include "LevelScene.hpp"
Cannon::KeyHandler::KeyHandler(Cannon* cannon) : m_cannon(cannon) {

}

void Cannon::KeyHandler::handle(const sf::Event::KeyEvent& e) {
	if (e.code == sf::Keyboard::Space && m_cannon->IsLoaded()) {
		m_cannon->Fire();
	}
}

Cannon::Cannon(engine::Scene* scene) : SpriteNode(scene), m_keyHandler(this) {
	scene->GetGame()->OnKeyDown.AddHandler(&m_keyHandler);
	if (scene->GetType() == NT_LEVELSCENE){
		static_cast<LevelScene*>(m_scene)->SetCannon(this);
	}
}

Cannon::~Cannon() {
	m_scene->GetGame()->OnKeyDown.RemoveHandler(&m_keyHandler);
	if (m_scene->GetType() == NT_LEVELSCENE){
		auto s=static_cast<LevelScene*>(m_scene);
		if (s->GetCannon() == this) s->SetCannon(nullptr);
	}
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
	Ui* ui = static_cast<Ui*>(m_scene->GetUi());
	Ui::Slot* s = ui->GetCurrentSlot();
	s->SetCount(s->GetCount()-1);
	if (s->GetCount() <= 0){
		m_loaded=false;
	}
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