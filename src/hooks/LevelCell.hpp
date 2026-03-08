#pragma once
#include <Geode/modify/LevelCell.hpp>

class $modify(HookedLevelCell, LevelCell) {
    static void onModify(geode::modifier::ModifyBase<geode::modifier::ModifyDerive<HookedLevelCell, LevelCell>>& self);
    void onClick(cocos2d::CCObject* sender);
};
