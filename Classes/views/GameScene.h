#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "models/GameModel.h"
#include "cocos2d.h"
#include <functional>

class CardView;

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GameScene);

    // 根据游戏模型初始化视图，创建所有卡牌精灵
    void initGameView(GameModel* model);

    // 根据卡牌数据ID查找对应精灵
    CardView* getCardSpriteById(int id);

    // 获取手牌堆中卡牌的放置位置 (即父节点 StackArea)
    cocos2d::Vec2 getStackCardPosition() const;

    // 重新排列备用牌区卡牌 (层叠显示效果)
    void updateReserveLayout(GameModel* model);

    // 显示胜利界面
    void showVictoryUI();

    // 设置回调函数
    void setOnReserveCardClick(const std::function<void(int)>& callback) { _onReserveCardClick = callback; }
    void setOnPlayfieldCardClick(const std::function<void(int)>& callback) { _onPlayfieldCardClick = callback; }
    void setOnUndoClick(const std::function<void()>& callback) { _onUndoClick = callback; }

private:
    // ========== UI 初始化 ==========
    void setupLayout();

    // 设置触摸监听器
    void setupTouchListener();

private:
    // ========== 界面元素引用 ==========
    cocos2d::LayerColor* _playFieldArea = nullptr;   // 主牌区容器（透明）
    cocos2d::LayerColor* _reserveArea = nullptr;   // 备用牌区容器（半透明）
    cocos2d::LayerColor* _stackArea = nullptr;   // 手牌堆容器（半透明）

    // ========== 回调函数 ==========
    std::function<void(int)> _onReserveCardClick;
    std::function<void(int)> _onPlayfieldCardClick;
    std::function<void()> _onUndoClick;
};

#endif // __GAME_SCENE_H__