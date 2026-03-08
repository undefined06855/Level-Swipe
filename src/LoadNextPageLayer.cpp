#include "LoadNextPageLayer.hpp"
#include "hooks/LevelBrowserLayer.hpp"

LoadNextPageLayer* LoadNextPageLayer::create(bool first, int pageNumber) {
    auto ret = new LoadNextPageLayer;
    if (ret->init(first, pageNumber)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool LoadNextPageLayer::init(bool first, int pageNumber) {
    if (!CCLayer::init()) return false;

    m_first = first;
    m_pageToLoad = first ? pageNumber - 1 : pageNumber + 1;

    auto bg = geode::createLayerBG();
    this->addChildAtPosition(bg, geode::Anchor::BottomLeft, { -5.f, -5.f });

    return true;
}

void LoadNextPageLayer::loadNextPage() {
    auto browser = HookedLevelBrowserLayer::getPreviousLevelBrowserLayer();
    if (!browser) return;

    if (!browser->m_leftArrow->isVisible() && m_first) {
        cocos2d::CCDirector::get()->popSceneWithTransition(.5f, cocos2d::kPopTransitionMoveInT);
        return;
    }

    if (!browser->m_rightArrow->isVisible() && !m_first) {
        cocos2d::CCDirector::get()->popSceneWithTransition(.5f, cocos2d::kPopTransitionMoveInT);
        return;
    }

    auto fields = browser->m_fields.self();
    fields->m_first = m_first;
    fields->m_isLoadingInBG = true;

    if (m_first) {
        browser->onPrevPage(nullptr);
    } else {
        browser->onNextPage(nullptr);
    }
}
