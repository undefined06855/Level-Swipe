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

    m_spinner = geode::LoadingSpinner::create(40.f);
    m_spinner->setOpacity(0);
    this->addChildAtPosition(m_spinner, geode::Anchor::Center);

    return true;
}

// returns true on success
bool LoadNextPageLayer::loadNextPage() {
    auto browser = HookedLevelBrowserLayer::getPreviousLevelBrowserLayer();
    if (!browser) return false;

    // if we're loading, don't do anything
    if (browser->m_circle->isVisible()) {
        return false;
    }
    
    if (!browser->m_leftArrow->isVisible() && m_first) {
        browser->m_fields->m_isLoadingInBG = false;
        cocos2d::CCDirector::get()->popSceneWithTransition(.5f, cocos2d::kPopTransitionMoveInT);
        return true;
    }

    if (!browser->m_rightArrow->isVisible() && !m_first) {
        browser->m_fields->m_isLoadingInBG = false;
        cocos2d::CCDirector::get()->popSceneWithTransition(.5f, cocos2d::kPopTransitionMoveInT);
        return true;
    }

    auto fields = browser->m_fields.self();
    fields->m_first = m_first;
    fields->m_isLoadingInBG = true;

    if (m_first) {
        browser->onPrevPage(nullptr);
    } else {
        browser->onNextPage(nullptr);
    }

    m_spinner->runAction(cocos2d::CCSequence::createWithTwoActions(
        cocos2d::CCDelayTime::create(1.f),
        cocos2d::CCFadeIn::create(2.f)
    ));

    return true;
}
