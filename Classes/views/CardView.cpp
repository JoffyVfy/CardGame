#include "CardView.h"

USING_NS_CC;

CardView* CardView::create(CardModel* card) {
    CardView* sprite = new CardView();
    if (sprite && sprite->init(card)) {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

bool CardView::init(CardModel* card) {
    if (!Node::init()) {
        return false;
    }

    this->card = card;

    // 布局参数
    const float margin = 15.0f;
    const float spacing = 4.0f;
    const float centerOffsetY = -20.0f;

    // 1. 创建背景卡牌精灵

    //backgroundSprite = Sprite::create("C:/Users/Joffy Vfy/Documents/CocosProgram/test3/Resources/res/card_general.png");
    backgroundSprite = Sprite::create("res/card_general.png");

    if (!backgroundSprite) {
        CCLOG("无法加载图片: card_general.png");
        return false;
    }
    backgroundSprite->setAnchorPoint(Vec2(0, 0));
    this->addChild(backgroundSprite, 0);

    auto bgSize = backgroundSprite->getContentSize();

    // 2. 创建中心大数字精灵
    std::string bigNumberImageName = getBigNumberImageName();
    auto centerNumberSprite = Sprite::create(bigNumberImageName);
    if (centerNumberSprite) {
        centerNumberSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
        centerNumberSprite->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.5f + centerOffsetY));
        backgroundSprite->addChild(centerNumberSprite, 1);
    }

    // 3. 创建左上角小数字精灵
    std::string smallNumberImageName = getSmallNumberImageName();
    auto topLeftNumberSprite = Sprite::create(smallNumberImageName);
    if (topLeftNumberSprite) {
        topLeftNumberSprite->setAnchorPoint(Vec2(0.0f, 1.0f));
        topLeftNumberSprite->setPosition(Vec2(margin, bgSize.height - margin));
        backgroundSprite->addChild(topLeftNumberSprite, 1);
    }

    // 4. 创建右上角花色精灵
    std::string suitImageName = getSuitImageName();
    auto topRightSuitSprite = Sprite::create(suitImageName);
    if (topRightSuitSprite) {
        topRightSuitSprite->setAnchorPoint(Vec2(1.0f, 1.0f));
        topRightSuitSprite->setPosition(Vec2(bgSize.width - margin, bgSize.height - margin));
        backgroundSprite->addChild(topRightSuitSprite, 1);
    }

    // 设置节点大小
    this->setContentSize(bgSize);

    return true;
}

bool CardView::isRedSuit() const {
    return (card->getSuit() == CardSuit::HEARTS || card->getSuit() == CardSuit::DIAMONDS);
}

std::string CardView::getSmallNumberImageName() const {
    std::string color;
    switch (card->getSuit()) {
    case CardSuit::HEARTS:
    case CardSuit::DIAMONDS:
        color = "red";
        break;
    case CardSuit::CLUBS:
    case CardSuit::SPADES:
        color = "black";
        break;
    default:
        return "";
    }

    //std::string prefix = "C:/Users/Joffy Vfy/Documents/CocosProgram/test3/Resources/res/number/small_";
    std::string prefix = "res/number/small_";
    int number = card->getFace();

    switch (number) {
    case 1:  return prefix + color + "_A.png";
    case 11: return prefix + color + "_J.png";
    case 12: return prefix + color + "_Q.png";
    case 13: return prefix + color + "_K.png";
    default: return prefix + color + "_" + std::to_string(number) + ".png";
    }
}

std::string CardView::getBigNumberImageName() const {
    std::string color;
    switch (card->getSuit()) {
    case CardSuit::HEARTS:
    case CardSuit::DIAMONDS:
        color = "red";
        break;
    case CardSuit::CLUBS:
    case CardSuit::SPADES:
        color = "black";
        break;
    default:
        return "";
    }

    //std::string prefix = "C:/Users/Joffy Vfy/Documents/CocosProgram/test3/Resources/res/number/big_";
    std::string prefix = "res/number/big_";
    int number = card->getFace();

    switch (number) {
    case 1:  return prefix + color + "_A.png";
    case 11: return prefix + color + "_J.png";
    case 12: return prefix + color + "_Q.png";
    case 13: return prefix + color + "_K.png";
    default: return prefix + color + "_" + std::to_string(number) + ".png";
    }
}

std::string CardView::getSuitImageName() const {
    //std::string prefix = "C:/Users/Joffy Vfy/Documents/CocosProgram/test3/Resources/res/suits/";
    std::string prefix = "res/suits/";

    switch (card->getSuit()) {
    case CardSuit::HEARTS: return prefix + "heart.png";
    case CardSuit::DIAMONDS: return prefix + "diamond.png";
    case CardSuit::CLUBS: return prefix + "club.png";
    case CardSuit::SPADES: return prefix + "spade.png";
    default: return "";
    }
}
