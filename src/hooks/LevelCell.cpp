#include "LevelCell.hpp"
#include "../LevelInfoLayerLayer.hpp"

void HookedLevelCell::onModify(geode::modifier::ModifyBase<geode::modifier::ModifyDerive<HookedLevelCell, LevelCell>>& self) {
    (void)self.setHookPriorityPost("LevelCell::onClick", geode::Priority::VeryLate);
}

void HookedLevelCell::onClick(cocos2d::CCObject* sender) {
    // some check in levelcell onClick that means gotolevelpage doesnt get called
    if (m_listType == BoomListType::Level4 && sender->getTag() != 0) {
        LevelCell::onClick(sender);
        return;
    }

    // outdated levels
    static_assert(GEODE_COMP_GD_VERSION == 22081, "Need to check version for outdatedness\nSee LevelCell::onClick");
    if (m_level->m_gameVersion > 22) {
        LevelCell::onClick(sender);
        return;
    }

    auto browserLayer = cocos2d::CCScene::get()->getChildByType<LevelBrowserLayer>(0);
    if (!browserLayer) {
        GameLevelManager::get()->gotoLevelPage(m_level);
        return;
    }

    // if we have one level and no pages, just create normal levelinfolayer
    if (browserLayer->m_levels->count() == 1 && !browserLayer->m_leftArrow->isVisible() && !browserLayer->m_rightArrow->isVisible()) {
        GameLevelManager::get()->gotoLevelPage(m_level);
        return;
    }

    auto index = getParent()->getChildren()->indexOfObject(this);
    auto scene = LevelInfoLayerLayer::scene(browserLayer, index);
    cocos2d::CCDirector::get()->pushScene(cocos2d::CCTransitionFade::create(.5f, scene));
}
