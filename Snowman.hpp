/* 
 * File:   Snowman.hpp
 * Author: iMer
 *
 * Created on 6. Dezember 2014, 03:50
 */

#ifndef SNOWMAN_HPP
#define	SNOWMAN_HPP
#include <SpriteNode.hpp>
#include "Constants.hpp"
#include <util/Event.hpp>

class Snowman : public engine::Node {
public:

    class BodyPart : public engine::SpriteNode {
    public:
        struct ParticleDef{
            b2Vec2 point;
            float force;
        };
    protected:
        float m_health;
        bool m_dead;
        std::vector<ParticleDef> m_particles;
    public:
        BodyPart(engine::Scene* scene);
        void Damage(float impact);
        virtual void OnUpdate(sf::Time interval);
        void AddParticle(const b2Vec2& point, float force);

    };

    class ContactHandler : public engine::util::EventHandler<b2Contact*, const b2ContactImpulse*> {
    protected:
        Snowman* m_snowman;
    public:
        ContactHandler(Snowman* snowman);
        virtual void handle(b2Contact* contact, const b2ContactImpulse* impulse);
    };
protected:
    friend ContactHandler;
    BodyPart* m_hat;
    BodyPart* m_head;
    BodyPart* m_middle;
    BodyPart* m_bottom;
    ContactHandler m_contactHandler;
public:
    static engine::Node* manufacture(Json::Value& root, engine::Node* parent);

    explicit Snowman(engine::Scene* scene);
    virtual ~Snowman();
protected:
    void Initialize(float x, float y);
    virtual void OnRemoveNode(Node* node);
    bool m_initialized;

    virtual uint8_t GetType() const {
        return NT_SNOWMAN;
    }


};

#endif	/* SNOWMAN_HPP */

