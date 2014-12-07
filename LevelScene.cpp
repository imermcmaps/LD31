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

LevelScene::LevelScene(engine::Game* game) : Scene(game), m_cannon(nullptr), m_score(0) {
}

LevelScene::~LevelScene() {
	std::cout << "DEL" << std::endl;
}

void LevelScene::OnUpdate(sf::Time interval) {
	engine::Scene::OnUpdate(interval);
	//std::cout << m_game->GetMousePosition().x << "," << m_game->GetMousePosition().y  << std::endl;
}

uint8_t LevelScene::GetType() const {
	return NT_LEVELSCENE;
}

bool LevelScene::initialize(Json::Value& root) {
	if (!engine::Scene::initialize(root)) {
		return false;
	}
	if (root.isMember("ui")) {
		Json::Value& ui = root["ui"];
		m_ui = engine::Factory::createJson<Ui, Scene*>(ui, this);
	}
	m_next = root.get("next", "").asString();
	return true;
}

void LevelScene::AddScore(uint32_t score) {
	m_score += score;
	auto ui = static_cast<Ui*> (m_ui);
	if (ui->GetScore()) {
		std::ostringstream ss;
		ss << "Score: " << static_cast<LD31*> (m_game)->GetScore() + m_score;
		ui->GetScore()->SetText(ss.str());
	}
}
void LevelScene::Next(){
	if (m_next != ""){
		auto game = static_cast<LD31*>(m_game);
		game->SetScore(game->GetScore()+m_score);
		static_cast<LD31*>(m_game)->Load(m_next);
	}
}
void LevelScene::Restart(){
	static_cast<LD31*>(m_game)->Load(m_filename);
}