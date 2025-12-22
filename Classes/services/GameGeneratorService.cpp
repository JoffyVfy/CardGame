#include "GameGeneratorService.h"

GameModel* GameGeneratorService::generateGameModel(LevelConfig* config) {
    if (!config) return nullptr;

    GameModel* model = new GameModel();
    
    // 用于生成唯一ID
    int cardIdCounter = 0;
    
    // 1. 生成备用牌区卡牌
    for(const auto& data : config->getReserveCards()) {
        CardModel* card = new CardModel(++cardIdCounter, (CardSuit)data.suit, data.face);
        card->setPosition(data.physicalPosition.x, data.physicalPosition.y);
        model->reserveCards.push_back(card);
    }
    
    // 2. 生成手牌堆卡牌
    for(const auto& data : config->getStackCards()) {
        CardModel* card = new CardModel(++cardIdCounter, (CardSuit)data.suit, data.face);
        card->setPosition(data.physicalPosition.x, data.physicalPosition.y);
        model->stackCards.push_back(card);
    }
    
    // 3. 生成主牌区卡牌
    for(const auto& data : config->getPlayfieldCards()) {
        CardModel* card = new CardModel(++cardIdCounter, (CardSuit)data.suit, data.face);
        card->setPosition(data.physicalPosition.x, data.physicalPosition.y);
        model->playfieldCards.push_back(card);
    }
    
    return model;
}