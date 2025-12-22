#include "CardModel.h"

// USING_NS_CC; // CardModel 不再继承 cocos2d::Node，不需要这个命名空间

CardModel::CardModel(int id, CardSuit suit, int face)
    : _id(id), _suit(suit), _face(face), _isFaceUp(false)
{
}

void CardModel::flip()
{
    _isFaceUp = !_isFaceUp;
}

void CardModel::setFaceUp(bool faceUp)
{
    _isFaceUp = faceUp;
}

bool CardModel::isRed() const
{
    return _suit == CardSuit::HEARTS || _suit == CardSuit::DIAMONDS;
}

bool CardModel::isBlack() const
{
    return _suit == CardSuit::CLUBS || _suit == CardSuit::SPADES;
}