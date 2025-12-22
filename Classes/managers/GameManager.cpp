#include "GameManager.h"

GameManager::GameManager() : _model(nullptr) {}

GameManager::~GameManager() {
    if (_model) {
        delete _model;
        _model = nullptr;
    }
}

void GameManager::setGameModel(GameModel* model) {
    if (_model) {
        delete _model;
    }
    _model = model;
}

bool GameManager::moveReserveToStack(int cardId) {
    if (!_model) return false;
    
    auto& reserve = _model->reserveCards;
    for (auto it = reserve.begin(); it != reserve.end(); ++it) {
        if ((*it)->getId() == cardId) {
            CardModel* card = *it;
            reserve.erase(it);
            _model->stackCards.push_back(card);
            return true;
        }
    }
    return false;
}

bool GameManager::movePlayfieldToStack(int cardId) {
    if (!_model) return false;
    
    auto& playfield = _model->playfieldCards;
    for (auto it = playfield.begin(); it != playfield.end(); ++it) {
        if ((*it)->getId() == cardId) {
            CardModel* card = *it;
            playfield.erase(it);
            _model->stackCards.push_back(card);
            return true;
        }
    }
    return false;
}

int GameManager::moveStackToReserve() {
    if (!_model) return -1;
    
    auto& stack = _model->stackCards;
    if (stack.empty()) return -1;
    
    // 规则：必须保留最后一张底牌
    if (stack.size() <= 1) return -1;
    
    CardModel* card = stack.back();
    stack.pop_back();
    _model->reserveCards.push_back(card);
    
    return card->getId();
}