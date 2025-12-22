#pragma once

enum class CardSuit {
    HEARTS = 0,
    DIAMONDS = 1,
    CLUBS = 2,
    SPADES = 3
};

class CardModel
{
public:
    // 构造函数，增加 id 参数
    CardModel(int id, CardSuit suit, int face);

    // 位置数据 (逻辑坐标或初始物理坐标)
    void setPosition(float x, float y) { _x = x; _y = y; }
    float getX() const { return _x; }
    float getY() const { return _y; }

    // 添加获取 ID 的方法
    int getId() const { return _id; }

    CardSuit getSuit() const { return _suit; }
    int getFace() const { return _face; }

    // 翻转卡牌
    void flip();
    void setFaceUp(bool faceUp);
    bool isFaceUp() const { return _isFaceUp; }

    // 判断花色颜色
    bool isRed() const;
    bool isBlack() const;

private:
    int _id;  // 唯一ID
    CardSuit _suit;
    int _face;
    bool _isFaceUp;
    float _x = 0;
    float _y = 0;
};