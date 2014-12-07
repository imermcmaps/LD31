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

Ui::Slot::FireHandler::FireHandler(Slot* slot): m_slot(slot) {

}

Ui::Slot::FireHandler::~FireHandler() {

}

void Ui::Slot::FireHandler::handle(Cannon* cannon) {
	if (m_slot->IsSelected()){
		m_slot->SetCount(m_slot->GetCount()-1);
		if (m_slot->GetCount() > 0){
			cannon->SetLoaded(true);
		} 
	}
}

Ui::Slot::Slot(engine::Scene* scene) : SpriteNode(scene), m_icon(nullptr), m_count(0) {
	if (scene->GetType() != NT_LEVELSCENE) {
		std::cerr << "Ui Scene type isnt level. Stuff will not work properly" << std::endl;
	}
}

Ui::Slot::~Slot() {

}

Ui::Ui(engine::Scene* scene) : Node(scene), m_currentSlot(0) {
	if (scene->GetType() != NT_LEVELSCENE) {
		std::cerr << "Ui Scene type isnt level. Stuff will not work properly" << std::endl;
	}
}

Ui::~Ui() {
}

bool Ui::initialize(Json::Value& root) {
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
			s->SetProjectile(slot.get("projectile", "").asString());
			s->SetIcon(slot.get("icon", "").asString());
			Node* text = s->GetChildByID("slot");
			if (text && text->GetType() == engine::NT_TEXT) {
				std::ostringstream ss;
				ss << i + 1;
				static_cast<engine::Text*> (text)->SetText(ss.str());
			}
			m_slots.insert(std::make_pair(i, s));
		}
	}
	return true;
}

