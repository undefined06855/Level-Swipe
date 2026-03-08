#pragma once

class LoadNextPageLayer : public cocos2d::CCLayer {
public:
    static LoadNextPageLayer* create(bool first, int pageNumber);
    bool init(bool first, int pageNumber);

    bool m_first;
    int m_pageToLoad;

    void loadNextPage();
};
