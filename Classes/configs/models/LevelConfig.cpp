#include "LevelConfig.h"

LevelConfig::LevelConfig()
{
}

LevelConfig::~LevelConfig()
{
}

void LevelConfig::addPlayfieldCard(const CardData& cardData)
{
    _playfieldCards.push_back(cardData);
}

void LevelConfig::addReserveCard(const CardData& cardData)
{
    _reserveCards.push_back(cardData);
}

void LevelConfig::addStackCard(const CardData& cardData)
{
    _stackCards.push_back(cardData);
}