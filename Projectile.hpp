/* 
 * File:   Projectile.hpp
 * Author: iMer
 *
 * Created on 6. Dezember 2014, 09:09
 */

#ifndef LD31_PROJECTILE_HPP
#define	LD31_PROJECTILE_HPP

#include <Engine/SpriteNode.hpp>

class Projectile : public engine::SpriteNode {
public:

    class ContactHandler : public engine::util::EventHandler<b2Contact*, bool> {
    protected:
        Projectile* m_projectile;
    public:
        ContactHandler(Projectile* snowman);
        virtual void handle(b2Contact* contact, bool);
    };
protected:
    bool m_doesExplode;
    bool m_explode;
    ContactHandler m_contactHandler;
public:
    Projectile(engine::Scene* scene);
    virtual ~Projectile();
    virtual bool initialize(Json::Value& root);

    void SetExplode(bool explode) {
        m_explode = explode;
    }

    bool IsExplode() const {
        return m_doesExplode;
    }
    virtual void OnUpdate(sf::Time interval);


protected:
};

#endif	/* LD31_PROJECTILE_HPP */

