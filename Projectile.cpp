/* 
 * File:   Projectile.cpp
 * Author: iMer
 * 
 * Created on 6. Dezember 2014, 09:09
 */

#include "Projectile.hpp"
#include "Factory.hpp"
#include "util/math.hpp"
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
		if (projectile->IsExplode()){
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
		engine::SpriteNode* explosion = static_cast<engine::SpriteNode*> (engine::Factory::CreateChildFromFile("assets/script/explosion.json", GetScene()));
		explosion->setScale(GetSize().x * 20 / explosion->GetSize().x, GetSize().x * 20 / explosion->GetSize().x);
		explosion->setPosition(gpos);
		explosion->GetAnimation()->OnOver = [explosion]() {
			explosion->Delete();
		};
		Json::Value ep;
		if (engine::Factory::LoadJson("assets/script/shockwave_particle.json", ep)){
			const float power = 5;
			for (uint32_t i=0; i<20; i++){
				float angle = (i/20.0) * 360.0 * engine::util::fPI/180.0;
				b2Vec2 rayDir( sinf(angle), cosf(angle) );
				Node* particle = engine::Factory::CreateChild(ep, explosion);
				explosion->AddNode(particle);
				particle->SetPosition(gpos.x, gpos.y);
				particle->GetBody()->SetLinearVelocity(power*rayDir);
				
			}
		}
		GetBody()->SetActive(false);
		Delete();
	}

				
}