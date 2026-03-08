#pragma once
#include <Geode/modify/LevelBrowserLayer.hpp>

class $modify(HookedLevelBrowserLayer, LevelBrowserLayer) {
    struct Fields {
        bool m_isLoadingInBG;
        bool m_first;

        Fields();
    };

    static HookedLevelBrowserLayer* getPreviousLevelBrowserLayer();
    int getIndexFromLevelID(int levelID);

    void loadLevelsFinished(cocos2d::CCArray* levels, const char* key, int type) override;
    void loadLevelsFailed(const char* key, int type) override;
};
