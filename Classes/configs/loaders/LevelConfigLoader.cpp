#include "LevelConfigLoader.h"


USING_NS_CC;
using namespace rapidjson;


LevelConfig* LevelConfigLoader::loadFromJson(const std::string& filename)
{
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filename);

    FILE* fp = fopen(fullPath.c_str(), "rb");
    if (!fp) {
        CCLOG("Failed to open file: %s", fullPath.c_str());
        return nullptr;
    }

    char readBuffer[65536]; // 1. 创建读取缓冲区（64KB）
    FileReadStream is(fp, readBuffer, sizeof(readBuffer)); // 2. 创建文件读取流，关联文件和缓冲区

    Document document; // 3. 创建 JSON 文档对象
    document.ParseStream(is); // 4. 从流中解析 JSON 数据
    fclose(fp); // 5. 关闭文件

    if (document.HasParseError()) { // 6. 检查是否有解析错误
        CCLOG("JSON parse error: %d", document.GetParseError());
        return nullptr;
    }

    // 7. 创建配置对象并填充数据
    LevelConfig* config = new LevelConfig();

    // 解析 Playfield
    // 1. 检查是否有 "Playfield" 键且其值是数组
    if (document.HasMember("Playfield") && document["Playfield"].IsArray())
    {
        // 2. 遍历数组中的每张牌
        const auto& playfield = document["Playfield"];
        for (SizeType i = 0; i < playfield.Size(); i++)
        {
            CardData cardData = parseCardData(playfield[i]);
            config->addPlayfieldCard(cardData);
        }
    }

    // 解析 Reserve
    if (document.HasMember("Reserve") && document["Reserve"].IsArray())
    {
        const auto& reserve = document["Reserve"];
        for (SizeType i = 0; i < reserve.Size(); i++)
        {
            CardData cardData = parseCardData(reserve[i]);
            config->addReserveCard(cardData);
        }
    }

    // 解析 Stack
    if (document.HasMember("Stack") && document["Stack"].IsArray())
    {
        const auto& stack = document["Stack"];
        for (SizeType i = 0; i < stack.Size(); i++)
        {
            CardData cardData = parseCardData(stack[i]);
            config->addStackCard(cardData);
        }
    }

    return config;
}

CardData LevelConfigLoader::parseCardData(const rapidjson::Value& cardJson)
{
    CardData data;

    if (cardJson.HasMember("CardFace"))
        data.face = cardJson["CardFace"].GetInt();

    if (cardJson.HasMember("CardSuit"))
        data.suit = cardJson["CardSuit"].GetInt();

    if (cardJson.HasMember("Position"))
    {
        const auto& pos = cardJson["Position"];
        float x = pos.HasMember("x") ? pos["x"].GetFloat() : 0.0f;
        float y = pos.HasMember("y") ? pos["y"].GetFloat() : 0.0f;
        data.physicalPosition = Vec2(x, y);
    }

    return data;
}