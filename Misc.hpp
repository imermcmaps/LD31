/* 
 * File:   Misc.hpp
 * Author: iMer
 *
 * Created on 8. Dezember 2014, 00:59
 */

#ifndef LD31_MISC_HPP
#define	LD31_MISC_HPP
#include <SFML/System.hpp>
#include <Engine/Scene.hpp>
engine::Node* CreateExplosion(engine::Scene* scene, float force, const sf::Vector2f& pos, float sizeScale,  uint8_t numParticles = 20);
#endif	/* LD31_MISC_HPP */

