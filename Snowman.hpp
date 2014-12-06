/* 
 * File:   Snowman.hpp
 * Author: iMer
 *
 * Created on 6. Dezember 2014, 03:50
 */

#ifndef SNOWMAN_HPP
#define	SNOWMAN_HPP
#include "engine/src/SpriteNode.hpp"

class Snowman: public engine::Node {
public:
    static engine::Node* manufacture(Json::Value& root, engine::Node* parent) ;
    
    Snowman(engine::Scene* scene);
    virtual ~Snowman();
private:
    void Initialize(float x, float y);
    virtual void OnRemoveNode(Node* node);
    bool m_initialized;
    engine::SpriteNode* m_hat;
    engine::SpriteNode* m_head;
    engine::SpriteNode* m_middle;
    engine::SpriteNode* m_bottom;
};

#endif	/* SNOWMAN_HPP */

