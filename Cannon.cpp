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
#include <iostream>

Cannon::Cannon(engine::Scene* scene) : SpriteNode(scene) {

}

Cannon::~Cannon() {
}

void Cannon::OnUpdate(sf::Time interval) {
	engine::SpriteNode::OnUpdate(interval);
	Node* barrel = m_children.front();
	auto delta =  m_scene->GetGame()->GetMousePosition() - barrel->GetGlobalPosition();
	float angle = atan2(delta.y, delta.x);
	angle *= 180 / engine::util::fPI;
	angle += 90;
	barrel->setRotation(engine::util::minmax<float>(15, angle, 100));
}

