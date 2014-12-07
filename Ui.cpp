/* 
 * File:   Ui.cpp
 * Author: iMer
 * 
 * Created on 7. Dezember 2014, 00:05
 */

#include "Ui.hpp"
#include "Scene.hpp"
#include "Constants.hpp"
#include "Factory.hpp"
#include "Cannon.hpp"
#include "Text.hpp"
#include "LevelScene.hpp"

Ui::Slot::Slot(engine::Scene* scene) : SpriteNode(scene), m_selected(false), m_icon(nullptr), m_count(0){
}

Ui::Slot::~Slot() {

}

Ui::Ui(engine::Scene* scene) : Node(scene), m_currentSlot(0), m_score(nullptr), m_next(nullptr)  {
	if (scene->GetType() != NT_LEVELSCENE) {
		std::cerr << "Ui Scene type isnt level. Stuff will not work properly" << std::endl;
	}
}

Ui::~Ui() {
}

bool Ui::initialize(Json::Value& root) {
	if (!engine::Node::initialize(root)){
		return false;
	}
	auto& slots = root["slots"];
	if (slots.isArray()) {
		for (size_t i = 0; i < slots.size(); i++) {
			auto slot = slots[i];
			if (!slot.isObject()) {
				std::cerr << "Invalid slot def " << i << std::endl;
				continue;
			}
			Slot* s = engine::Factory::create<Slot>("assets/script/slot.json", m_scene);
			if (!s) {
				return false;
			}
			this->AddNode(s);
			s->SetPosition((s->GetSize().x + 5) * (i + 1), s->GetSize().y / 2 + 10);
			s->SetCount(slot.get("count", 1).asInt());
			static_cast<LevelScene*>(m_scene)->AddAmmo(s->GetCount());
			s->SetProjectile(slot.get("projectile", "").asString());
			s->SetIcon(slot.get("icon", "").asString());
			Node* text = s->GetChildByID("slot");
			if (text && text->GetType() == engine::NT_TEXT) {
				std::ostringstream ss;
				ss << i + 1;
				static_cast<engine::Text*> (text)->SetText(ss.str());
				
			}
			m_slots.insert(std::make_pair(i, s));
			if (i == m_currentSlot) {
			}
			s->UpdateAnimation();
		}
	}
	auto& score = root["score"];
	if (score.isObject()){
		m_score = static_cast<engine::Text*>(engine::Factory::CreateChild(score, this));
		AddNode(m_score);
	}
	auto& next = root["next"];
	if (next.isObject()){
		m_next = static_cast<engine::Button*>(engine::Factory::CreateChild(next, this));
		AddNode(m_next);
		m_next->OnClick = [this](engine::Button*, sf::Vector2f){
			static_cast<LevelScene*>(m_scene)->Next();
		};
	}
	m_init=true;
	return true;
}

Ui::Slot* Ui::GetCurrentSlot() {
	return m_slots[m_currentSlot];
}

void Ui::OnUpdate(sf::Time interval) {
#define P sf::Keyboard::isKeyPressed
	bool m_updateSelect = false;
	if (m_currentSlot != 0 && m_slots.size() > 0 && P(sf::Keyboard::Num1)) {
		GetCurrentSlot()->SetSelected(false);
		m_currentSlot = 0;
		m_updateSelect = true;
	} else if (m_currentSlot != 1 && m_slots.size() > 1 && P(sf::Keyboard::Num2)) {
		GetCurrentSlot()->SetSelected(false);
		m_currentSlot = 1;
		m_updateSelect = true;
	} else if (m_currentSlot != 2 && m_slots.size() > 2 && P(sf::Keyboard::Num3)) {
		GetCurrentSlot()->SetSelected(false);
		m_currentSlot = 2;
		m_updateSelect = true;
	}
	if (m_updateSelect) {
		Select();
		m_updateSelect=false;
	}
	if (m_init){
		m_init=Select();
		static_cast<LevelScene*>(m_scene)->AddScore(0);
	}
#undef P
}
bool Ui::Select(){
	auto s = GetCurrentSlot();
	s->SetSelected(true);
	if (m_scene->GetType() == NT_LEVELSCENE) {
		auto cannon = static_cast<LevelScene*> (m_scene)->GetCannon();
		if (!cannon){
			return false;
		}
		cannon->SetCannonBall(s->GetProjectile());
		if (s->GetCount() > 0) {
			cannon->SetLoaded(true);
		} else {
			cannon->SetLoaded(false);
		}
	}
	return true;
}