#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include "models/CardModel.h"


class CardView : public cocos2d::Node {
public:
    static CardView* create(CardModel* card);
    virtual bool init(CardModel* card);

    CardModel* getCard() const { return card; }

private:
    CardModel* card;

    // 子精灵sprite
    cocos2d::Sprite* backgroundSprite;  // 背景卡牌
    cocos2d::Sprite* numberSprite;      // 数字
    cocos2d::Sprite* suitSprite;        // 花色

    // 辅助函数
    std::string getBigNumberImageName() const;    // 获取大数字图片路径
    std::string getSmallNumberImageName() const;  // 获取左上角小数字图片路径
    std::string getSuitImageName() const;
    bool isRedSuit() const;
};

#endif // __CARD_VIEW_H__