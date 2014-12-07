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
#include "Ui.hpp"
LevelScene::LevelScene(LD31* game): Scene(game), m_cannon(nullptr) {
}

LevelScene::~LevelScene() {
}

void LevelScene::OnUpdate(sf::Time interval){
	engine::Scene::OnUpdate(interval);
	//std::cout << m_game->GetMousePosition().x << "," << m_game->GetMousePosition().y  << std::endl;
}

uint8_t LevelScene::GetType() const{
	return NT_LEVELSCENE;
}
bool LevelScene::initialize(Json::Value& root){
	if (!engine::Scene::initialize(root)){
		return false;
	}
	if (root.isMember("ui")){
		Json::Value& ui = root["ui"];
		m_ui = engine::Factory::createJson<Ui, Scene*>(ui, this);
	}
	return true;
}