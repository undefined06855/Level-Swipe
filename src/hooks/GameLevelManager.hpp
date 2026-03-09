#pragma once
#include <Geode/modify/GameLevelManager.hpp>

class $modify(HookedGameLevelManager, GameLevelManager) {
    struct Fields {
        bool m_allowDownloadingLevel;

        Fields();
    };

    void downloadLevel(int id, bool gauntletLevel, int dailyID);
    void updateLevel(GJGameLevel* level);
};
