#include "LevelCell.hpp"
#include "../LevelInfoLayerLayer.hpp"

void HookedLevelCell::onClick(cocos2d::CCObject* sender) {
    // TODO: change hook prio

    auto browserLayer = cocos2d::CCScene::get()->getChildByType<LevelBrowserLayer>(0);
    if (!browserLayer) {
        LevelCell::onClick(sender);
        return;
    }

    auto index = getParent()->getChildren()->indexOfObject(this);
    auto scene = LevelInfoLayerLayer::scene(browserLayer, index);
    cocos2d::CCDirector::get()->pushScene(cocos2d::CCTransitionFade::create(.5f, scene));
}
