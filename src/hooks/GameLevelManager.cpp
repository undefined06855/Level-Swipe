#include "GameLevelManager.hpp"

HookedGameLevelManager::Fields::Fields()
    : m_allowDownloadingLevel(true) {}

void HookedGameLevelManager::downloadLevel(int id, bool gauntletLevel, int dailyID) {
    if (!m_fields->m_allowDownloadingLevel) return;

    GameLevelManager::downloadLevel(id, gauntletLevel, dailyID);
}

void HookedGameLevelManager::updateLevel(GJGameLevel* level) {
    if (!m_fields->m_allowDownloadingLevel) return;

    GameLevelManager::updateLevel(level);
}