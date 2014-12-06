/* 
 * File:   Projectile.hpp
 * Author: iMer
 *
 * Created on 6. Dezember 2014, 09:09
 */

#ifndef LD31_PROJECTILE_HPP
#define	LD31_PROJECTILE_HPP

#include <SpriteNode.hpp>


class Projectile: public engine::SpriteNode {
public:
    Projectile(engine::Scene* scene);
    virtual ~Projectile();
protected:
};

#endif	/* LD31_PROJECTILE_HPP */

