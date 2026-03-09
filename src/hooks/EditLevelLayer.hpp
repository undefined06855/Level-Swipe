#pragma once
#include <Geode/modify/EditLevelLayer.hpp>

class $modify(HookedEditLevelLayer, EditLevelLayer) {
    static void onModify(geode::modifier::ModifyBase<geode::modifier::ModifyDerive<HookedEditLevelLayer, EditLevelLayer>>& self);

    void onBack(cocos2d::CCObject* sender);
};
