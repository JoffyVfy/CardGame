#pragma once

#include "cocos2d.h"

struct CardData
{
    int face = 0;
    int suit = 0;
    cocos2d::Vec2 physicalPosition;
};

class LevelConfig
{
public:
    LevelConfig();
    ~LevelConfig();

    void addPlayfieldCard(const CardData& cardData);
    void addReserveCard(const CardData& cardData);
    void addStackCard(const CardData& cardData);

    // 返回常量引用: const 调用者不能修改返回的数据; & 返回引用，避免拷贝整个 vector
    const std::vector<CardData>& getPlayfieldCards() const { return _playfieldCards; }
    const std::vector<CardData>& getReserveCards() const { return _reserveCards; }
    const std::vector<CardData>& getStackCards() const { return _stackCards; }

private:
    std::vector<CardData> _playfieldCards;
    std::vector<CardData> _reserveCards;
    std::vector<CardData> _stackCards;
};