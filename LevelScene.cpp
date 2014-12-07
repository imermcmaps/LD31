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

LevelScene::LevelScene(engine::Game* game) : Scene(game), m_cannon(nullptr), m_score(0), m_ammoLeft(0), m_targetsLeft(0), m_restartTimer(0) {
}

LevelScene::~LevelScene() {
}

void LevelScene::OnUpdate(sf::Time interval) {
	engine::Scene::OnUpdate(interval);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
		m_restartTimer += interval.asSeconds();
		if (m_restartTimer > 1.5) {
			Restart();
		}
	} else {
		m_restartTimer = 0;
	}
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

void LevelScene::Next() {
	if (m_next != "") {
		auto game = static_cast<LD31*> (m_game);
		game->SetScore(game->GetScore() + m_score + m_ammoLeft*5);
		static_cast<LD31*> (m_game)->Load(m_next);
	}
}

void LevelScene::Restart() {
	static_cast<LD31*> (m_game)->Load(m_filename);
}

void LevelScene::UpdateNext() {
	if (!m_ui) {
		return;
	}
	std::ostringstream ss;
	ss << "+" << m_ammoLeft * 5;
	Node* bonus = m_ui->GetChildByID("bonusScore");
	if (bonus->GetType() == engine::NT_TEXT) {
		static_cast<engine::Text*>(bonus)->SetText(ss.str());
	}

	if (!m_ammoLeft || !m_targetsLeft) {
		static_cast<Ui*> (m_ui)->GetNext()->SetActive(true);
	} else {
		static_cast<Ui*> (m_ui)->GetNext()->SetActive(false);
	}
}