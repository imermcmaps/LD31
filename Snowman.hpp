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
#include "Damageable.hpp"
class Snowman : public engine::Node {
public:

    class BodyPart : public Damageable {
    public:
        struct ParticleDef{
            b2Vec2 point;
            float force;
        };
    protected:
        bool m_dead;
        std::vector<ParticleDef> m_particles;
    public:
        BodyPart(engine::Scene* scene);
        virtual void Damage(float damage, Node* by, const b2WorldManifold& manifold);
        virtual void OnUpdate(sf::Time interval);
        void AddParticle(const b2Vec2& point, float force);
        virtual uint8_t GetType() const{
            return NT_SNOWMANPART;
        }


    };
protected:
    BodyPart* m_hat;
    BodyPart* m_head;
    BodyPart* m_middle;
    BodyPart* m_bottom;
    bool m_flipped;
public:
    static engine::Node* manufacture(Json::Value& root, engine::Node* parent);

    explicit Snowman(engine::Scene* scene);
    virtual ~Snowman();

    void SetFlipped(bool flipped) {
        if (m_hat){
            m_hat->SetFlipped(flipped);
        }
        if (m_head){
            m_head->SetFlipped(flipped);
            m_head->setOrigin(18, 18);
        }
        if (m_middle){
            m_middle->SetFlipped(flipped);
        }
        if (m_bottom){
            m_bottom->SetFlipped(flipped);
        }
        m_flipped = flipped;
    }

    bool IsFlipped() const {
        return m_flipped;
    }
protected:
    void Initialize(float x, float y);
    virtual void OnRemoveNode(Node* node);
    bool m_initialized;

    virtual uint8_t GetType() const {
        return NT_SNOWMAN;
    }


};

#endif	/* SNOWMAN_HPP */

