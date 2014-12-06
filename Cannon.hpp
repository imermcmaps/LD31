/* 
 * File:   Cannon.hpp
 * Author: iMer
 *
 * Created on 6. Dezember 2014, 08:08
 */

#ifndef LD31_CANNON_HPP
#define	LD31_CANNON_HPP
#include <SpriteNode.hpp>

class Cannon: public engine::SpriteNode {
public:
    explicit Cannon(engine::Scene* scene);
    virtual ~Cannon();
    virtual void OnUpdate(sf::Time interval);
    bool m_loaded;
    std::string  m_cannonBall;
    uint32_t m_cannonBallCount;
};

#endif	/* LD31_CANNON_HPP */

