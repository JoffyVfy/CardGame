#include "GameModel.h"

GameModel::~GameModel() {
    // 释放所有卡牌内存
    // 注意：这里假设 GameModel 拥有 Card 的所有权
    for(auto c : reserveCards) delete c;
    for(auto c : stackCards) delete c;
    for(auto c : playfieldCards) delete c;
    
    reserveCards.clear();
    stackCards.clear();
    playfieldCards.clear();
}

CardModel* GameModel::getCardById(int id) {
    for(auto c : reserveCards) if(c->getId() == id) return c;
    for(auto c : stackCards) if(c->getId() == id) return c;
    for(auto c : playfieldCards) if(c->getId() == id) return c;
    return nullptr;
}