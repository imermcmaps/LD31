/* 
 * File:   Cannon.hpp
 * Author: iMer
 *
 * Created on 6. Dezember 2014, 08:08
 */

#ifndef LD31_CANNON_HPP
#define	LD31_CANNON_HPP
#include <SpriteNode.hpp>
#include <util/Event.hpp>
#include <SFML/Window.hpp>
class Cannon : public engine::SpriteNode {

    class MouseHandler : public engine::util::EventHandler<const sf::Event::MouseButtonEvent&> {
    protected:
        Cannon* m_cannon;
    public:
        MouseHandler(Cannon* cannon);
        virtual void handle(const sf::Event::MouseButtonEvent&);
    };
protected:
    
    bool m_loaded;
    std::string m_cannonBall;
    uint32_t m_cannonBallCount;
    float m_shootCooldown;
    MouseHandler m_mouseHandler;
    
public:
    explicit Cannon(engine::Scene* scene);
    virtual ~Cannon();
    
    void Fire();

    bool IsLoaded() const {
        return m_loaded;
    }
    virtual bool initialize(Json::Value& root);

    void SetLoaded(bool loaded) {
        m_loaded = loaded;
    }
    engine::util::Event<Cannon*> OnFire;
protected:
    virtual void OnUpdate(sf::Time interval);

};

#endif	/* LD31_CANNON_HPP */

