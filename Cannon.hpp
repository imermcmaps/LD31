/* 
 * File:   Cannon.hpp
 * Author: iMer
 *
 * Created on 6. Dezember 2014, 08:08
 */

#ifndef LD31_CANNON_HPP
#define	LD31_CANNON_HPP
#include <Engine/SpriteNode.hpp>
#include <Engine/util/Event.hpp>
#include <SFML/Window.hpp>
class Cannon : public engine::SpriteNode {

    class KeyHandler : public engine::util::EventHandler<const sf::Event::KeyEvent&> {
    protected:
        Cannon* m_cannon;
    public:
        KeyHandler(Cannon* cannon);
        virtual void handle(const sf::Event::KeyEvent&);
    };
protected:
    
    bool m_loaded;
    std::string m_cannonBall;
    uint32_t m_cannonBallCount;
    float m_shootCooldown;
    KeyHandler m_keyHandler;
    
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

    void SetCannonBall(std::string cannonBall) {
        m_cannonBall = cannonBall;
    }

    std::string GetCannonBall() const {
        return m_cannonBall;
    }
protected:
    virtual void OnUpdate(sf::Time interval);

};

#endif	/* LD31_CANNON_HPP */

