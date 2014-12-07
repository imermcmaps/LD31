/* 
 * File:   Ui.hpp
 * Author: iMer
 *
 * Created on 7. Dezember 2014, 00:05
 */

#ifndef LD31_UI_HPP
#define	LD31_UI_HPP
#include <unordered_map>
#include <Node.hpp>
#include <Text.hpp>
#include <Factory.hpp>
#include <sstream>
#include "Cannon.hpp"
#include <Button.hpp>

class Ui : public engine::Node {
public:

    class Slot : public engine::SpriteNode {
    protected:
        bool m_selected;
        uint32_t m_count;
        std::string m_projectile;
        engine::Node* m_icon;
    public:
        Slot(engine::Scene* scene);
        virtual ~Slot();

        void SetCount(uint32_t count) {
            Node* text = GetChildByID("count");
            if (text && text->GetType() == engine::NT_TEXT) {
                std::ostringstream ss;
                ss << count;
                static_cast<engine::Text*> (text)->SetText(ss.str());
            }
            m_count = count;
            UpdateAnimation();
        }

        void SetIcon(std::string file) {
            if (m_icon) {
                m_icon->Delete();
            }
            m_icon = engine::Factory::CreateChildFromFile(file, this);
            // ensure it's rendered before the text
            m_children.remove(m_icon);
            m_children.push_front(m_icon);
        }

        uint32_t GetCount() const {
            return m_count;
        }

        void SetProjectile(std::string projectile) {
            m_projectile = projectile;
        }

        std::string GetProjectile() const {
            return m_projectile;
        }

        void SetSelected(bool selected) {
            m_selected = selected;
            UpdateAnimation();
        }

        bool IsSelected() const {
            return m_selected;
        }
        void UpdateAnimation() {
            if (m_selected){
                if (m_count > 0){
                    PlayAnimation("selected");
                } else {
                    PlayAnimation("selected_empty");
                }
            }else{
                if (m_count > 0){
                    PlayAnimation("default");
                } else {
                    PlayAnimation("empty");
                }
            }
        }
    };
protected:
    std::unordered_map<uint8_t, Slot*> m_slots;
    uint8_t m_currentSlot;
    engine::Text* m_score;
    engine::Button* m_next;
    bool m_updateSelect;
    bool m_init;
public:
    Ui(engine::Scene* scene);
    virtual ~Ui();
    virtual bool initialize(Json::Value& root);
    Slot* GetCurrentSlot();

    engine::Text* GetScore() const {
        return m_score;
    }
protected:
    virtual void OnUpdate(sf::Time interval);
    void Select();


};

#endif	/* LD31_UI_HPP */

