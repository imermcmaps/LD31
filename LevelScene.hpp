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

class Cannon;
class LD31;
class LevelScene: public engine::Scene{
public:
    explicit LevelScene(LD31* game);
    virtual ~LevelScene();
    
    virtual uint8_t GetType() const;

    void SetSnowman(Snowman* snowman) {
        m_snowman = snowman;
    }

    Snowman* GetSnowman() const {
        return m_snowman;
    }

    void SetCannon(Cannon* cannon) {
        m_cannon = cannon;
    }

    Cannon* GetCannon() const {
        return m_cannon;
    }
    virtual bool initialize(Json::Value& root);

protected:
    virtual void OnUpdate(sf::Time interval);
private:
    Snowman* m_snowman;
    Cannon* m_cannon;
};

#endif	/* LD31_LEVELSCENE_HPP */

