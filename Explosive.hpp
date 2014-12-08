/* 
 * File:   Explosive.hpp
 * Author: iMer
 *
 * Created on 8. Dezember 2014, 00:57
 */

#ifndef LD31_EXPLOSIVE_HPP
#define	LD31_EXPLOSIVE_HPP
#include "Damageable.hpp"

class Explosive: public Damageable {
public:
    Explosive(engine::Scene* scene);
    virtual ~Explosive();

    virtual void Damage(float damage, Node* by, const b2WorldManifold& manifold);

    virtual void OnUpdate(sf::Time interval);

private:

};

#endif	/* LD31_EXPLOSIVE_HPP */

