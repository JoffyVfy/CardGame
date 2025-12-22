#pragma once
#include <vector>
#include "CardModel.h"

class GameModel {
public:
    ~GameModel();
    
    // 数据容器
    std::vector<CardModel*> reserveCards;
    std::vector<CardModel*> stackCards;
    std::vector<CardModel*> playfieldCards;
    
    // 辅助函数：根据ID查找卡牌数据
    CardModel* getCardById(int id);
};