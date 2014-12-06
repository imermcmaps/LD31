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
    explicit LevelScene(LD31* game);
    virtual ~LevelScene();
    
    virtual uint8_t GetType() const;

    void SetSnowman(Snowman* snowman) {
        m_snowman = snowman;
    }

    Snowman* GetSnowman() const {
        return m_snowman;
    }
protected:
    virtual void OnUpdate(sf::Time interval);
private:
    Snowman* m_snowman;
};

#endif	/* LD31_LEVELSCENE_HPP */

