/* 
 * File:   LD31.cpp
 * Author: iMer
 * 
 * Created on 6. Dezember 2014, 03:07
 */

#include "LD31.hpp"
#include "LevelScene.hpp"
#include "Factory.hpp"
LD31::LD31() {
	m_windowTitle = "LD31 - Do you want to kill a snowman?";
	m_scene = engine::Factory::create<LevelScene>("assets/script/level_1.json", this);
}

LD31::~LD31() {
}

