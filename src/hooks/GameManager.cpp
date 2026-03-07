#include "GameManager.hpp"
#include "../LevelInfoLayerLayer.hpp"

void HookedGameManager::returnToLastScene(GJGameLevel* level) {
    if (m_sceneEnum != 3) {
        GameManager::returnToLastScene(level);
        return;
    }

    auto director = cocos2d::CCDirector::get();

    // current scene is playlayer or whatever
    // scene before should be levelbrowserlayer
    auto sceneBefore = director->m_pobScenesStack->objectAtIndex(director->m_pobScenesStack->count() - 2);
    auto levelBrowserLayer = geode::cast::typeinfo_cast<LevelBrowserLayer*>(sceneBefore);
    if (!levelBrowserLayer) {
        // oh well
        GameManager::returnToLastScene(level);
        return;
    }

    auto scene = LevelInfoLayerLayer::scene(levelBrowserLayer, level->m_levelIndex);
    director->replaceScene(cocos2d::CCTransitionFade::create(.5f, scene));
}
