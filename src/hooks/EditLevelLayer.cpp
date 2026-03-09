#include "EditLevelLayer.hpp"
#include "../LevelInfoLayerLayer.hpp"

void HookedEditLevelLayer::onModify(geode::modifier::ModifyBase<geode::modifier::ModifyDerive<HookedEditLevelLayer, EditLevelLayer>>& self) {
    (void)self.setHookPriorityPost("EditLevelLayer::onBack", geode::Priority::VeryLate);
}

void HookedEditLevelLayer::onBack(cocos2d::CCObject* sender) {
    auto layer = LevelInfoLayerLayer::get();
    if (layer) {
        layer->onBack();
        return;
    }

    EditLevelLayer::onBack(sender);
}
