/* 
 * File:   Snowman.hpp
 * Author: iMer
 *
 * Created on 6. Dezember 2014, 03:50
 */

#ifndef SNOWMAN_HPP
#define	SNOWMAN_HPP
#include "engine/src/SpriteNode.hpp"
#include "Constants.hpp"

class Snowman : public engine::Node {
protected:

    engine::SpriteNode* m_hat;
    engine::SpriteNode* m_head;
    engine::SpriteNode* m_middle;
    engine::SpriteNode* m_bottom;
public:
    static engine::Node* manufacture(Json::Value& root, engine::Node* parent);

    explicit Snowman(engine::Scene* scene);
    virtual ~Snowman();
protected:
    void Initialize(float x, float y);
    virtual void OnRemoveNode(Node* node);
    bool m_initialized;
    
    virtual uint8_t GetType() const{
        return NT_SNOWMAN;
    }


};

#endif	/* SNOWMAN_HPP */

