#include "LevelInfoLayer.hpp"
#include "../LevelInfoLayerLayer.hpp"

HookedLevelInfoLayer::Fields::Fields()
    : m_allowDownloadLevel(false) {}

void HookedLevelInfoLayer::onModify(geode::modifier::ModifyBase<geode::modifier::ModifyDerive<HookedLevelInfoLayer, LevelInfoLayer>>& self) {
    (void)self.setHookPriorityPost("LevelInfoLayer::onBack", geode::Priority::VeryLate);
}

void HookedLevelInfoLayer::onBack(cocos2d::CCObject* sender) {
    auto layer = LevelInfoLayerLayer::get();
    if (layer) {
        layer->onBack();
        return;
    }

    LevelInfoLayer::onBack(sender);
}

void HookedLevelInfoLayer::downloadLevel() {
    if (!m_fields->m_allowDownloadLevel) return;

    LevelInfoLayer::downloadLevel();
}

void HookedLevelInfoLayer::keyBackClicked() {
    if (LevelInfoLayerLayer::get()) return;

    LevelInfoLayer::keyBackClicked();
}
