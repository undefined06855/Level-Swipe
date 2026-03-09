#pragma once

class LevelInfoLayerLayer : public cocos2d::CCLayer, public BoomScrollLayerDelegate {
public:
    static LevelInfoLayerLayer* create(LevelBrowserLayer* layer, int index);
    static LevelInfoLayerLayer* get();
    static cocos2d::CCScene* scene(LevelBrowserLayer* layer, int index);
    bool init(LevelBrowserLayer* layer, int index);

    cocos2d::CCSpriteBatchNode* m_dots;
    BoomScrollLayer* m_scrollLayer;
    std::vector<LevelInfoLayer*> m_layers;

    void hideBGForLayer(cocos2d::CCLayer* layer);
    void changePage(int page);
    void downloadLevel(int page);

    void onBack();

    virtual void keyDown(cocos2d::enumKeyCodes key, double timestamp) override;
    virtual void scrollLayerScrollingStarted(BoomScrollLayer* layer) override;
    virtual void scrollLayerWillScrollToPage(BoomScrollLayer* layer, int page) override;
};
