#include "GameManager.hpp"
#include "LevelBrowserLayer.hpp"
#include "../LevelInfoLayerLayer.hpp"

void HookedGameManager::returnToLastScene(GJGameLevel* level) {
    if (m_sceneEnum != 3) {
        GameManager::returnToLastScene(level);
        return;
    }

    auto levelBrowserLayer = HookedLevelBrowserLayer::getPreviousLevelBrowserLayer();
    if (!levelBrowserLayer) {
        GameManager::returnToLastScene(level);
        return;
    }
    
    int index = levelBrowserLayer->getIndexFromLevelID(level->m_levelID);
    auto scene = LevelInfoLayerLayer::scene(levelBrowserLayer, index);
    cocos2d::CCDirector::get()->replaceScene(cocos2d::CCTransitionFade::create(.5f, scene));
}
