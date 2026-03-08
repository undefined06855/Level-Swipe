#include "LevelBrowserLayer.hpp"
#include "../LevelInfoLayerLayer.hpp"

HookedLevelBrowserLayer::Fields::Fields()
    : m_first(false)
    , m_isLoadingInBG(false) {}

HookedLevelBrowserLayer* HookedLevelBrowserLayer::getPreviousLevelBrowserLayer() {
    auto director = cocos2d::CCDirector::get();

    // current scene is playlayer or levelinfolayer or whatever
    // scene before should be levelbrowserlayer
    auto sceneBefore = director->m_pobScenesStack->objectAtIndex(director->m_pobScenesStack->count() - 2);
    auto levelBrowserLayer = static_cast<cocos2d::CCScene*>(sceneBefore)->getChildByType<LevelBrowserLayer*>(0);
    auto cast = static_cast<HookedLevelBrowserLayer*>(levelBrowserLayer);
    // cast->m_fields.self(); // create fields here to avoid bugs(?)
    return cast;
}

int HookedLevelBrowserLayer::getIndexFromLevelID(int levelID) {
    for (int i = 0; i < m_levels->count(); i++) {
        auto level = static_cast<GJGameLevel*>(m_levels->objectAtIndex(i));
        if (level->m_levelID == levelID) {
            return i;
        }
    }

    geode::log::warn("Could not find level index for id {}!", levelID);
    return 0;
}

void HookedLevelBrowserLayer::loadLevelsFinished(cocos2d::CCArray* levels, const char* key, int type) {
    auto fields = m_fields.self();
    if (!fields->m_isLoadingInBG) {
        LevelBrowserLayer::loadLevelsFinished(levels, key, type);
        return;
    }

    auto director = cocos2d::CCDirector::get();

    director->popScene(); // no transition
    LevelBrowserLayer::loadLevelsFinished(levels, key, type);

    cocos2d::CCScene* scene;
    if (fields->m_first) {
        // if we are coming from the first one, go to the last
        scene = LevelInfoLayerLayer::scene(this, levels->count() - 1);
    } else {
        // if we are coming from the last one, go to the first
        scene = LevelInfoLayerLayer::scene(this, 0);
    }

    director->pushScene(cocos2d::CCTransitionCrossFade::create(.25f, scene));

    fields->m_isLoadingInBG = false;

    // show animation
    auto label = cocos2d::CCLabelBMFont::create(fmt::format("Page {}", m_searchObject->m_page + 1).c_str(), "bigFont.fnt");
    label->setScale(4.f);
    label->setOpacity(127);
    label->setPosition(director->getWinSize() / 2.f);
    label->runAction(cocos2d::CCSequence::create(
        cocos2d::CCSpawn::createWithTwoActions(
            cocos2d::CCEaseExponentialOut::create(cocos2d::CCMoveBy::create(.5f, { fields->m_first ? 100.f : -100.f, 0.f })),
            cocos2d::CCSequence::createWithTwoActions(
                cocos2d::CCDelayTime::create(.1f),
                cocos2d::CCEaseExponentialOut::create(cocos2d::CCFadeTo::create(.25f, 0))
            )
        ),

        cocos2d::CCDelayTime::create(.5f),
        cocos2d::CCRemoveSelf::create(),
        nullptr
    ));
    geode::OverlayManager::get()->addChild(label);
}

void HookedLevelBrowserLayer::loadLevelsFailed(const char* key, int type) {
    auto fields = m_fields.self();
    if (!fields->m_isLoadingInBG) {
        LevelBrowserLayer::loadLevelsFailed(key, type);
        return;
    }

    cocos2d::CCDirector::get()->popScene(); // no transition
    LevelBrowserLayer::loadLevelsFailed(key, type);

    geode::log::error("Failed to load levels!");
    fields->m_isLoadingInBG = false;
}
