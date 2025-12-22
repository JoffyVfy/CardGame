#pragma once
#include "models/GameModel.h"

class GameManager {
public:
    GameManager();
    ~GameManager();
    
    void setGameModel(GameModel* model);
    GameModel* getGameModel() const { return _model; }
    
    // 业务逻辑：纯数据操作
    
    // 移动备用牌到手牌堆
    bool moveReserveToStack(int cardId);
    
    // 移动主牌区牌到手牌堆
    bool movePlayfieldToStack(int cardId);
    
    // 回退：移动手牌堆顶到备用牌区
    // 返回被移动的卡牌ID，失败返回-1
    int moveStackToReserve();
    
private:
    GameModel* _model;
};