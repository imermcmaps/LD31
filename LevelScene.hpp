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

class LD31;
class LevelScene: public engine::Scene{
public:
    LevelScene(LD31* game);
    virtual ~LevelScene();
    virtual void OnUpdate(sf::Time interval);
    virtual uint8_t GetType() const;

    void SetSnowman(Snowman* snowman) {
        m_snowman = snowman;
    }

    Snowman* GetSnowman() const {
        return m_snowman;
    }

private:
    Snowman* m_snowman;
};

#endif	/* LD31_LEVELSCENE_HPP */

