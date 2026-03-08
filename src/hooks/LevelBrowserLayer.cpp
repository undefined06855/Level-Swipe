#include "LevelBrowserLayer.hpp"
#include "../LevelInfoLayerLayer.hpp"

HookedLevelBrowserLayer::Fields::Fields()
    : m_first(false)
    , m_isLoadingInBG(false) {}

HookedLevelBrowserLayer* HookedLevelBrowserLayer::getPreviousLevelBrowserLayer() {
    auto director = cocos2d::CCDirector::get();

    if (director->m_pobScenesStack->count() < 2) return nullptr;

    // current scene is playlayer or levelinfolayer or whatever
    // scene before should be levelbrowserlayer
    auto sceneBefore = director->m_pobScenesStack->objectAtIndex(director->m_pobScenesStack->count() - 2);
    auto levelBrowserLayer = static_cast<cocos2d::CCScene*>(sceneBefore)->getChildByType<LevelBrowserLayer*>(0);
    auto cast = static_cast<HookedLevelBrowserLayer*>(levelBrowserLayer);
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
    // if there isnt a levelinfolayerlayer or we're not the previous layer in scene stack, return
    if (!HookedLevelBrowserLayer::getPreviousLevelBrowserLayer() || !LevelInfoLayerLayer::get()) {
        LevelBrowserLayer::loadLevelsFinished(levels, key, type);
        return;
    }

    auto fields = m_fields.self();
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
    auto winSize = director->getWinSize();
    auto label = cocos2d::CCLabelBMFont::create(fmt::format("Page {}", m_searchObject->m_page + 1).c_str(), "bigFont.fnt");
    label->setScale(4.f);
    label->setOpacity(0);
    label->setPosition({
        winSize.width / 2.f + (fields->m_first ? -40.f : 40.f),
        winSize.height / 2.f
    });
    label->runAction(cocos2d::CCSequence::create(
        cocos2d::CCDelayTime::create(.1f), // wait for initial lag spike
        cocos2d::CCFadeTo::create(0.f, 200),

        cocos2d::CCSpawn::createWithTwoActions(
            cocos2d::CCEaseExponentialOut::create(cocos2d::CCMoveBy::create(1.f, { fields->m_first ? 100.f : -100.f, 0.f })),
            cocos2d::CCEaseExponentialOut::create(cocos2d::CCFadeTo::create(.75f, 0))
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
