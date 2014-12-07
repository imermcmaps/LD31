/* 
 * File:   LoadingScene.cpp
 * Author: iMer
 * 
 * Created on 7. Dezember 2014, 05:05
 */

#include "LoadingScene.hpp"
#include "Cannon.hpp"
#include "Factory.hpp"
#include "LevelScene.hpp"
#include <Game.hpp>

LoadingScene::DeleteHandler::DeleteHandler(LoadingScene* scene) : m_scene(scene) {

}

void LoadingScene::DeleteHandler::handle(Node*) {
	m_scene->m_delete = true;
}

LoadingScene::LoadingScene(engine::Game* game) : Scene(game), m_delete(false), m_deleteHandler(this) {
}

LoadingScene::~LoadingScene() {
}

void LoadingScene::Load(Scene* old, std::string target) {
	m_old=old;
	m_old->OnDelete.AddHandler(&m_deleteHandler);
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