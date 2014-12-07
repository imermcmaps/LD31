/* 
 * File:   LD31.hpp
 * Author: iMer
 *
 * Created on 6. Dezember 2014, 03:07
 */

#ifndef LD31_LD31_HPP
#define	LD31_LD31_HPP
#include "Game.hpp"
#include "LoadingScene.hpp"
class LD31: public engine::Game {
protected:
    uint32_t m_score;
    LoadingScene m_load;
public:
    LD31();
    virtual ~LD31();

    void SetScore(uint32_t score) {
        m_score = score;
    }
    void Load(std::string target){
        m_load.Load(m_scene, target);
    }
    uint32_t GetScore() const {
        return m_score;
    }

};

#endif	/* LD31_LD31_HPP */

