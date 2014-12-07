/* 
 * File:   LevelScene.hpp
 * Author: iMer
 *
 * Created on 6. Dezember 2014, 03:13
 */

#ifndef LD31_LEVELSCENE_HPP
#define	LD31_LEVELSCENE_HPP
#include "Scene.hpp"
#include "Snowman.hpp"
#include "Ui.hpp"

class Cannon;
class LD31;

class LevelScene : public engine::Scene {
protected:
    std::string m_next;
    Cannon* m_cannon;
    uint32_t m_score;
public:
    explicit LevelScene(engine::Game* game);
    virtual ~LevelScene();

    virtual uint8_t GetType() const;

    void SetCannon(Cannon* cannon) {
        m_cannon = cannon;
    }

    Cannon* GetCannon() const {
        return m_cannon;
    }
    virtual bool initialize(Json::Value& root);

    void AddScore(uint32_t score);
    void Next();
    void Restart();
protected:
    virtual void OnUpdate(sf::Time interval);

};

#endif	/* LD31_LEVELSCENE_HPP */

