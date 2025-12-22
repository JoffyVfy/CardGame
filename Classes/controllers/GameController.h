#pragma once

#include "configs/loaders/LevelConfigLoader.h"
#include "managers/GameManager.h"
#include "views/CardView.h"
#include "models/CardModel.h"

class GameScene; 
class LevelConfig;

class GameController
{
public:
    static GameController* getInstance();

    // 启动游戏关卡
    void startGame(GameScene* scene, int levelId);

    // 处理备用牌区点击
    void handleReserveCardClick(int cardId);

    void moveReserveToStack(int cardId);

    // 处理主牌区点击事件
    void handlePlayfieldCardClick(int cardId);

    void movePlayfieldToStack(int cardId);

    // 撤销操作
    void undoLastMove();

    // 检查胜利条件
    void checkVictory();

private:
    GameController(); // 私有构造函数，实现在cpp文件

    LevelConfig* _config = nullptr;
    
    // 核心引用
    GameManager* _gameManager = nullptr;
    GameScene* _gameScene = nullptr;

    // 动画锁
    bool _isAnimating = false;
};