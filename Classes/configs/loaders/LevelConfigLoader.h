#ifndef __LEVEL_CONFIG_LOADER_H__
#define __LEVEL_CONFIG_LOADER_H__

#include "cocos2d.h"
#include "json/document.h"
#include "configs/models/LevelConfig.h"
#include "models/CardModel.h"
#include "json/filereadstream.h"
#include <cstdio>

class LevelConfigLoader
{
public:
    // 从 JSON 文件加载关卡配置
    static LevelConfig* loadFromJson(const std::string& filename);

    // 别名方法，方便调用
    static LevelConfig* loadLevelConfig(const std::string& filename) {
        return loadFromJson(filename);
    }

private:
    // 解析单张卡牌的数据
    static CardData parseCardData(const rapidjson::Value& cardJson);
};

#endif // __LEVEL_CONFIG_LOADER_H__