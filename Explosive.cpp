/* 
 * File:   Explosive.cpp
 * Author: iMer
 * 
 * Created on 8. Dezember 2014, 00:57
 */

#include "Explosive.hpp"
#include "Misc.hpp"

Explosive::Explosive(engine::Scene* scene): Damageable(scene) {
	m_health=0.5;
}

Explosive::~Explosive() {
}

void Explosive::Damage(float damage, Node* by, const b2WorldManifold& manifold){
	m_health -= damage;
}

void Explosive::OnUpdate(sf::Time interval) {
if (m_health < 0.4){
		CreateExplosion(GetScene(), 10, GetGlobalPosition(), 7, 20);
		Delete();
	}
}
