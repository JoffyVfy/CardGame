#pragma once
#include "configs/models/LevelConfig.h"
#include "models/GameModel.h"

class GameGeneratorService {
public:
    // 静态服务：根据配置，生成运行时游戏模型
    static GameModel* generateGameModel(LevelConfig* config);
};