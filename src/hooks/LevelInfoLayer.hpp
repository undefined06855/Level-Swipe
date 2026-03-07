#pragma once
#include <Geode/modify/LevelInfoLayer.hpp>

class $modify(HookedLevelInfoLayer, LevelInfoLayer) {
    struct Fields {
        bool m_allowDownloadLevel;

        Fields();
    };

    void downloadLevel();
    virtual void onEnterTransitionDidFinish();
};
