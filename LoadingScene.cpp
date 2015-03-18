/* 
 * File:   LoadingScene.cpp
 * Author: iMer
 * 
 * Created on 7. Dezember 2014, 05:05
 */

#include "LoadingScene.hpp"
#include "Cannon.hpp"
#include "Engine/Factory.hpp"
#include "LevelScene.hpp"
#include <Engine/Game.hpp>

LoadingScene::LoadingScene(engine::Game* game) : Scene(game), m_delete(false){
}

LoadingScene::~LoadingScene() {
}

void LoadingScene::Load(Scene* old, std::string target) {
	m_old=old;
	m_target=target;
	m_delete=false;
	m_game->SetScene(this);
}

void LoadingScene::OnUpdate(sf::Time interval) {
	m_delete=true;
}
void LoadingScene::OnDraw(sf::RenderTarget& target, sf::RenderStates states, float delta){
	if (m_delete){
		delete m_old;
		m_old=nullptr;
		m_game->SetScene(engine::Factory::create<LevelScene>(m_target, m_game));
	}
}