#include "LevelInfoLayerLayer.hpp"
#include "LoadNextPageLayer.hpp"
#include "hooks/LevelInfoLayer.hpp"
#include "hooks/LevelBrowserLayer.hpp"

LevelInfoLayerLayer* LevelInfoLayerLayer::create(LevelBrowserLayer* layer, int index) {
    auto ret = new LevelInfoLayerLayer;
    if (ret->init(layer, index)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

LevelInfoLayerLayer* LevelInfoLayerLayer::get() {
    auto scene = cocos2d::CCScene::get();
    if (!scene) return nullptr;
    return scene->getChildByType<LevelInfoLayerLayer*>(0);
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
    auto pages = cocos2d::CCArray::createWithCapacity(levels.size());

    int page = layer->m_searchObject->m_page + 1;

    pages->addObject(LoadNextPageLayer::create(/* first */ true, page));

    for (int i = 0; i < levels.size(); i++) {
        auto level = levels[i];

        // basically recreate GameLevelManager::gotoLevelPage but with our stuff
        static_assert(GEODE_COMP_GD_VERSION == 22081, "Check GameLevelManager::gotoLevelPage hasn't changed?");

        if (level->m_levelType == GJLevelType::Editor) {
            auto page = EditLevelLayer::create(level);
            this->hideBGForLayer(page);
            m_layers.push_back(nullptr);
            pages->addObject(page);
            continue;
        }

        if (level->m_levelType == GJLevelType::Saved) {
            auto page = LevelInfoLayer::create(level, false);
            this->hideBGForLayer(page);
            m_layers.push_back(page);
            pages->addObject(page);
            continue;
        }

        if (level->m_levelType == GJLevelType::SearchResult) {
            auto glm = GameLevelManager::get();
            GJGameLevel* savedLevel = nullptr;

            if (level->m_dailyID < 1) {
                if (!level->m_gauntletLevel) {
                    savedLevel = glm->getSavedLevel(level->m_levelID);
                } else {
                    savedLevel = glm->getSavedGauntletLevel(level->m_levelID);
                }
            } else {
                savedLevel = glm->getSavedDailyLevel(level->m_dailyID);
            }

            if (savedLevel) {
                if (!savedLevel->m_levelNotDownloaded) {
                    if (level->m_levelVersion <= savedLevel->m_levelVersion) {
                        savedLevel->m_songID = level->m_songID;
                        level = savedLevel;
                    }
                } else {
                    glm->saveLevel(level);
                }
            }

            auto page = LevelInfoLayer::create(level, false);
            this->hideBGForLayer(page);
            m_layers.push_back(page);
            pages->addObject(page);
            continue;
        }

        m_layers.push_back(nullptr);
        auto message = fmt::format("No layer for this level!\nm_levelID = {}, m_levelType={}", level->m_levelID, (int)level->m_levelType);
        pages->addObject(cocos2d::CCLabelBMFont::create(message.c_str(), "bigFont.fnt"));
    }

    pages->addObject(LoadNextPageLayer::create(/* last */ false, page));

    int offset = index + 1; // go to page index+1 (add one because of loadnextpagelayer)
    m_scrollLayer = BoomScrollLayer::create(pages, offset, true);

    m_dots = m_scrollLayer->getChildByType<cocos2d::CCSpriteBatchNode>(0);
    m_dots->setPositionY(255.f);
    // hide first and last dot (loadnextpagelayers)
    static_cast<cocos2d::CCSprite*>(m_dots->getChildByIndex(0))->setOpacity(0);
    static_cast<cocos2d::CCSprite*>(m_dots->getChildByIndex(m_dots->getChildrenCount() - 1))->setOpacity(0);

    m_scrollLayer->m_delegate = this;
    m_scrollLayer->setDotScale(.8f);
    m_scrollLayer->updateDots(0.f);
    this->addChildAtPosition(m_scrollLayer, geode::Anchor::BottomLeft);

    m_scrollLayer->instantMoveToPage(offset);
    m_scrollLayer->m_page = offset;
    this->scrollLayerWillScrollToPage(m_scrollLayer, offset); // doesn't get called

    this->setKeyboardEnabled(true);

    return true;
}

void LevelInfoLayerLayer::hideBGForLayer(cocos2d::CCLayer* layer) {
    auto bg = layer->getChildByType<cocos2d::CCSprite>(0);
    bg->setVisible(false);
}

void LevelInfoLayerLayer::changePage(int page) {
    // taken from levelselectlayer
    if (m_scrollLayer->m_pageMoving) {
        m_scrollLayer->m_pageMoving = false;
        m_scrollLayer->m_extendedLayer->stopActionByTag(2);
        m_scrollLayer->m_extendedLayer->setPosition(m_scrollLayer->m_position);
        m_scrollLayer->moveToPageEnded();
    }

    m_scrollLayer->moveToPage(page);
}

void LevelInfoLayerLayer::keyDown(cocos2d::enumKeyCodes key, double timestamp) {
    if (key == cocos2d::enumKeyCodes::KEY_Left) {
        this->changePage(m_scrollLayer->m_page - 1);
        return;
    }

    if (key == cocos2d::enumKeyCodes::KEY_Right) {
        this->changePage(m_scrollLayer->m_page + 1);
        return;
    }

    // sometimes keyBackClicked breaks so this would normally be in keyBackClicked
    // TODO: maybe fix?
    if (key == cocos2d::enumKeyCodes::KEY_Escape) {
        for (auto page : m_layers) {
            if (page && page->m_isBusy) return;
        }

        auto browser = HookedLevelBrowserLayer::getPreviousLevelBrowserLayer();
        if (browser) {
            browser->m_fields->m_isLoadingInBG = false;
        }

        // idk rob uses it just using it for mod compatibility
        GameManager::get()->safePopScene();
        return;
    }
}

void LevelInfoLayerLayer::scrollLayerScrollingStarted(BoomScrollLayer* layer) {
    this->stopActionByTag(1); // stop waiting to download this level
}

void LevelInfoLayerLayer::scrollLayerWillScrollToPage(BoomScrollLayer* layer, int page) {
    auto pages = m_scrollLayer->m_pages;

    // load next page if needed
    if (page == 0 || page == pages->count() - 1) {
        bool ret = static_cast<LoadNextPageLayer*>(pages->objectAtIndex(page))->loadNextPage();
        if (!ret) {
            if (page == 0) m_scrollLayer->instantMoveToPage(1);
            else m_scrollLayer->instantMoveToPage(pages->count() - 2);
        }
        return;
    }

    if (page < 0) {
        m_scrollLayer->instantMoveToPage(0);
        return;
    }

    if (page > pages->count() - 1) {
        m_scrollLayer->instantMoveToPage(pages->count() - 1);
        return;
    }

    // wait .25s before downloading to buffer in case the user switches quickly to avoid rate limit
    auto action = cocos2d::CCSequence::createWithTwoActions(
        cocos2d::CCDelayTime::create(.25f),
        geode::cocos::CallFuncExt::create([this, page] { this->downloadLevel(page); })
    );

    action->setTag(1);
    this->stopActionByTag(1);
    this->runAction(action);
}

void LevelInfoLayerLayer::downloadLevel(int page) {
    this->stopActionByTag(1); // in case downloadLevel was called from somewhere else
    auto pages = m_scrollLayer->m_pages;

    auto infoLayer = geode::cast::typeinfo_cast<LevelInfoLayer*>(pages->objectAtIndex(page));
    if (!infoLayer) return;

    static_cast<HookedLevelInfoLayer*>(infoLayer)->m_fields->m_allowDownloadLevel = true;
    infoLayer->onEnterTransitionDidFinish();
    if (infoLayer->shouldDownloadLevel()) infoLayer->downloadLevel();

    GameManager::get()->m_currentLevelID = infoLayer->m_level->m_levelID;
}
