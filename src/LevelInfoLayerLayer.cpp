#include "LevelInfoLayerLayer.hpp"
#include "hooks/LevelInfoLayer.hpp"

LevelInfoLayerLayer* LevelInfoLayerLayer::create(LevelBrowserLayer* layer, int index) {
    auto ret = new LevelInfoLayerLayer;
    if (ret->init(layer, index)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

cocos2d::CCScene* LevelInfoLayerLayer::scene(LevelBrowserLayer* layer, int index) {
    auto scene = cocos2d::CCScene::create();
    auto node = LevelInfoLayerLayer::create(layer, index);
    node->setPosition(scene->getContentSize() / 2.f);
    scene->addChild(node);
    return scene;
}

bool LevelInfoLayerLayer::init(LevelBrowserLayer* layer, int index) {
    if (!CCLayer::init()) return false;

    auto bg = geode::createLayerBG();
    this->addChildAtPosition(bg, geode::Anchor::BottomLeft, { -5.f, -5.f });

    auto levels = geode::cocos::CCArrayExt<GJGameLevel*>(layer->m_list->m_listView->m_entries);
    m_pages = cocos2d::CCArray::createWithCapacity(levels.size());

    for (auto level : levels) {
        cocos2d::CCLayer* page;

        if (level->m_levelType == GJLevelType::Editor) {
            page = EditLevelLayer::create(level);
        } else {
            page = LevelInfoLayer::create(level, false);
        }

        m_pages->addObject(page);
    }

    auto scroll = BoomScrollLayer::create(m_pages, 0, true);

    m_dots = scroll->getChildByType<cocos2d::CCSpriteBatchNode>(0);
    m_dots->setPositionY(270.f);

    scroll->m_delegate = this;
    scroll->instantMoveToPage(index);
    scroll->updateDots(0.f);
    this->addChildAtPosition(scroll, geode::Anchor::BottomLeft);

    return true;
}

void LevelInfoLayerLayer::scrollLayerScrollingStarted(BoomScrollLayer* layer) {
    m_dots->runAction(cocos2d::CCEaseExponentialOut::create(cocos2d::CCMoveBy::create(.3f, { 0.f, -20.f })));
}

void LevelInfoLayerLayer::scrollLayerWillScrollToPage(BoomScrollLayer* layer, int page) {
    m_dots->runAction(cocos2d::CCEaseExponentialIn::create(cocos2d::CCMoveBy::create(.3f, { 0.f, 20.f })));

    int max = m_pages->count();
    page = ((page % max) + max) % max;

    auto infoLayer = geode::cast::typeinfo_cast<LevelInfoLayer*>(m_pages->objectAtIndex(page));
    if (!infoLayer) return;

    reinterpret_cast<HookedLevelInfoLayer*>(infoLayer)->m_fields->m_allowDownloadLevel = true;
    infoLayer->onEnterTransitionDidFinish();
    if (infoLayer->shouldDownloadLevel()) infoLayer->downloadLevel();

    GameManager::get()->m_currentLevelID = infoLayer->m_level->m_levelID;
}
