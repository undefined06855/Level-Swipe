#include "LevelInfoLayer.hpp"

HookedLevelInfoLayer::Fields::Fields()
    : m_allowDownloadLevel(false) {}

void HookedLevelInfoLayer::downloadLevel() {
    if (!m_fields->m_allowDownloadLevel) return;

    LevelInfoLayer::downloadLevel();
}

void HookedLevelInfoLayer::onEnterTransitionDidFinish() {
    if (!m_fields->m_allowDownloadLevel) return;

    LevelInfoLayer::onEnterTransitionDidFinish();
}
