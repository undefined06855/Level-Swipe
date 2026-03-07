#pragma once

class LevelInfoLayerLayer : public cocos2d::CCLayer, public BoomScrollLayerDelegate {
public:
    static LevelInfoLayerLayer* create(LevelBrowserLayer* layer, int index);
    static cocos2d::CCScene* scene(LevelBrowserLayer* layer, int index);
    bool init(LevelBrowserLayer* layer, int index);

    cocos2d::CCSpriteBatchNode* m_dots;
    geode::Ref<cocos2d::CCArray> m_pages;

    virtual void scrollLayerScrollingStarted(BoomScrollLayer* layer) override;
    virtual void scrollLayerWillScrollToPage(BoomScrollLayer* layer, int page) override;
};
