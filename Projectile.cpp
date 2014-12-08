/* 
 * File:   Projectile.cpp
 * Author: iMer
 * 
 * Created on 6. Dezember 2014, 09:09
 */

#include "Projectile.hpp"
#include "Factory.hpp"
#include "util/math.hpp"
#include "Misc.hpp"
Projectile::ContactHandler::ContactHandler(Projectile* projectile): m_projectile(projectile){
	
}
void Projectile::ContactHandler::handle(b2Contact* contact, bool begin) {
	if (!begin) return;
	engine::Node* a = static_cast<engine::Node*> (contact->GetFixtureA()->GetBody()->GetUserData());
	engine::Node* b = static_cast<engine::Node*> (contact->GetFixtureB()->GetBody()->GetUserData());

	Projectile* projectile = nullptr;
	engine::Node* other = nullptr;
	if (a == m_projectile) {
		projectile = static_cast<Projectile*> (a);
		other = b;
	} else if (b == m_projectile) {
		projectile = static_cast<Projectile*> (b);
		other = a;
	}
	if (projectile) {
		if (projectile->IsExplode() && other->GetIdentifier() != "shockwave"){
			projectile->SetExplode(true);
		}
	}
}
Projectile::Projectile(engine::Scene* scene): SpriteNode(scene), m_explode(false), m_doesExplode(false), m_contactHandler(this) {
	m_scene->OnContact.AddHandler(&m_contactHandler);
}

Projectile::~Projectile() {
	m_scene->OnContact.RemoveHandler(&m_contactHandler);
}

bool Projectile::initialize(Json::Value& root){
	if (!engine::SpriteNode::initialize(root)){
		return false;
	}
	m_doesExplode = root.get("explode", false).asBool();
	return true;
}
void Projectile::OnUpdate(sf::Time interval){
	if (m_explode){
		m_doesExplode = m_explode = false;
		auto gpos = GetGlobalPosition();
		CreateExplosion(GetScene(), 2, GetGlobalPosition(), 5);
		GetBody()->SetActive(false);
		Delete();
	}

				
}