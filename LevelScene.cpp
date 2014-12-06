/* 
 * File:   LevelScene.cpp
 * Author: iMer
 * 
 * Created on 6. Dezember 2014, 03:13
 */

#include "LevelScene.hpp"
#include "LD31.hpp"
#include <iostream>
#include "Constants.hpp"
#include <Factory.hpp>
LevelScene::LevelScene(LD31* game): Scene(game) {
}

LevelScene::~LevelScene() {
}

void LevelScene::OnUpdate(sf::Time interval){
	//std::cout << m_game->GetMousePosition().x << "," << m_game->GetMousePosition().y  << std::endl;
}

uint8_t LevelScene::GetType() const{
	return NT_LEVELSCENE;
}