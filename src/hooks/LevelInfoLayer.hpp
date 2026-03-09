#pragma once
#include <Geode/modify/LevelInfoLayer.hpp>

class $modify(HookedLevelInfoLayer, LevelInfoLayer) {
    struct Fields {
        bool m_allowDownloadLevel;

        Fields();
    };

    static void onModify(geode::modifier::ModifyBase<geode::modifier::ModifyDerive<HookedLevelInfoLayer, LevelInfoLayer>>& self);

    void onBack(cocos2d::CCObject* sender);
    void downloadLevel();
    virtual void keyBackClicked();
};
