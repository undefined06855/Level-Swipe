#include "LevelInfoLayer.hpp"
#include "../LevelInfoLayerLayer.hpp"

void HookedLevelInfoLayer::onModify(geode::modifier::ModifyBase<geode::modifier::ModifyDerive<HookedLevelInfoLayer, LevelInfoLayer>>& self) {
    (void)self.setHookPriorityPost("LevelInfoLayer::onBack", geode::Priority::VeryLate);
    (void)self.setHookPriorityPost("LevelInfoLayer::levelDownloadFailed", geode::Priority::Replace);
}

void HookedLevelInfoLayer::onBack(cocos2d::CCObject* sender) {
    auto layer = LevelInfoLayerLayer::get();
    if (layer) {
        layer->onBack();
        return;
    }

    LevelInfoLayer::onBack(sender);
}

void HookedLevelInfoLayer::levelDownloadFailed(int response) {
    m_circle->setVisible(false);

    auto popup = FLAlertLayer::create(
        "Error",
        "Level download failed, please try again later.",
        "OK" // capital OK :(
    );
    popup->m_scene = this;
    popup->show();
}

void HookedLevelInfoLayer::keyBackClicked() {
    if (LevelInfoLayerLayer::get()) return;

    LevelInfoLayer::keyBackClicked();
}
