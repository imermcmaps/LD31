/* 
 * File:   LoadingScene.hpp
 * Author: iMer
 *
 * Created on 7. Dezember 2014, 05:05
 */

#ifndef LOADINGSCENE_HPP
#define	LOADINGSCENE_HPP

#include <Scene.hpp>
#include <util/Event.hpp>
class LoadingScene : public engine::Scene {
public:

    class DeleteHandler : public engine::util::EventHandler<Node*> {
        LoadingScene* m_scene;
    public:
        explicit DeleteHandler(LoadingScene* scene);
        virtual void handle(Node*);
    };
protected:
    Scene* m_old;
    std::atomic<bool> m_delete;
    bool m_updating;
    DeleteHandler m_deleteHandler;
    std::string m_target;
public:
    LoadingScene(engine::Game* game);
    void Load(Scene* old, std::string target);
    virtual ~LoadingScene();
    virtual void OnUpdate(sf::Time interval);
    virtual void OnDraw(sf::RenderTarget& target, sf::RenderStates states, float delta);

};

#endif	/* LOADINGSCENE_HPP */

