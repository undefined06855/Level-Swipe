#pragma once
#include <Geode/modify/LevelInfoLayer.hpp>

class $modify(HookedLevelInfoLayer, LevelInfoLayer) {
    static void onModify(geode::modifier::ModifyBase<geode::modifier::ModifyDerive<HookedLevelInfoLayer, LevelInfoLayer>>& self);

    void onBack(cocos2d::CCObject* sender);
    virtual void levelDownloadFailed(int response);
    virtual void keyBackClicked();
};
