/* 
 * File:   Explosive.hpp
 * Author: iMer
 *
 * Created on 7. Dezember 2014, 20:58
 */

#ifndef LD31_EXPLOSIVE_HPP
#define	LD31_EXPLOSIVE_HPP

#include <SpriteNode.hpp>

class Damageable : public engine::SpriteNode {
public:

    class ContactHandler : public engine::util::EventHandler<b2Contact*, const b2ContactImpulse*> {
    protected:
        Damageable* m_damageable;
    public:
        ContactHandler(Damageable* damageable);
        virtual void handle(b2Contact* contact, const b2ContactImpulse* impulse);
    };
protected:
    float m_health;
    ContactHandler m_contactHandler;
public:
    Damageable(engine::Scene* scene);
    virtual ~Damageable();
    virtual void Damage(float damage, Node* by, const b2WorldManifold& manifold);
private:

};

#endif	/* EXPLOSIVE_HPP */

