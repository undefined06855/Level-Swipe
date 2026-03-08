#include "LevelInfoLayer.hpp"
#include "../LevelInfoLayerLayer.hpp"

HookedLevelInfoLayer::Fields::Fields()
    : m_allowDownloadLevel(false) {}

void HookedLevelInfoLayer::downloadLevel() {
    if (!m_fields->m_allowDownloadLevel) return;

    LevelInfoLayer::downloadLevel();
}

void HookedLevelInfoLayer::keyBackClicked() {
    if (LevelInfoLayerLayer::get()) return;

    LevelInfoLayer::keyBackClicked();
}
