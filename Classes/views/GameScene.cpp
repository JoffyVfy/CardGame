#include "GameScene.h"
#include "views/CardView.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::init())
        return false;

    setupLayout();  // 初始化布局元素

    // 设置触摸事件监听
    setupTouchListener();

    return true;
}

void GameScene::setupLayout()
{
    // 获取屏幕尺寸
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 1. 主牌区 (PlayField) - 深棕色背景
    _playFieldArea = LayerColor::create(Color4B(139, 69, 19, 255), 1080, 1500);
    _playFieldArea->setPosition(Vec2(origin.x, origin.y + 580));
    this->addChild(_playFieldArea);

    // 2. 备用牌区 (Reserve) - 紫色半透明 (左下角)
    _reserveArea = LayerColor::create(Color4B(100, 0, 150, 255), 540, 580);
    _reserveArea->setPosition(Vec2(origin.x, origin.y));
    _reserveArea->setTag(100); // 设置 Tag 方便 Controller 获取
    this->addChild(_reserveArea);

    // 3. 手牌堆 (Stack) - 紫色半透明 (右下角)
    _stackArea = LayerColor::create(Color4B(100, 0, 150, 255), 540, 580);
    _stackArea->setPosition(Vec2(origin.x + 540, origin.y));
    _stackArea->setTag(101); // 设置 Tag
    this->addChild(_stackArea);

    // 4. 回退按钮
    auto undoLabel = Label::createWithSystemFont("\xE5\x9B\x9E\xE9\x80\x80", "Arial", 48); 
    auto undoItem = MenuItemLabel::create(undoLabel, [this](Ref* sender) {
        if (_onUndoClick) {
            _onUndoClick();
        }
    });
    
    undoItem->setPosition(Vec2(1000, 250));

    auto menu = Menu::create(undoItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1000); // ZOrder 1000，确保在最上层
}

void GameScene::initGameView(GameModel* model)
{
    if (!model) return;

    _playFieldArea->removeAllChildren();
    _reserveArea->removeAllChildren();
    _stackArea->removeAllChildren();

    // 1. 主牌区
    for (auto card : model->playfieldCards) {
        auto sprite = CardView::create(card);
        // 使用卡牌中的位置
        sprite->setPosition(Vec2(card->getX(), card->getY()));
        _playFieldArea->addChild(sprite);
    }

    // 2. 备用牌区
    Vec2 reserveCenterPos(179, 119); 
    float xInterval = 30.0f;
    int totalReserve = model->reserveCards.size();

    for (int i = 0; i < totalReserve; ++i) {
        auto card = model->reserveCards[i];
        auto sprite = CardView::create(card);
        _reserveArea->addChild(sprite);
        
        float offsetX = (i - (totalReserve - 1) / 2.0f) * xInterval;
        sprite->setPosition(Vec2(reserveCenterPos.x + offsetX, reserveCenterPos.y));
    }

    // 3. 手牌堆
    Vec2 stackPos = getStackCardPosition(); 
    for (auto card : model->stackCards) {
        auto sprite = CardView::create(card);
        _stackArea->addChild(sprite);
        sprite->setPosition(stackPos);
    }
}


void GameScene::setupTouchListener()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true); // 吞噬触摸，防止穿透

    listener->onTouchBegan = [this](Touch* touch, Event* event) {
        Vec2 touchLoc = touch->getLocation();
        
        CCLOG("Click: %f, %f", touchLoc.x, touchLoc.y);

        // 1. 检测 Reserve 区 (从顶层往下)
        auto& reserveChildren = _reserveArea->getChildren();
        for (ssize_t i = reserveChildren.size() - 1; i >= 0; --i) {
            auto child = reserveChildren.at(i);
            CardView* sprite = dynamic_cast<CardView*>(child);
            if (!sprite) continue;

            Vec2 localPos = sprite->getParent()->convertToNodeSpace(touchLoc);
            if (sprite->getBoundingBox().containsPoint(localPos)) {
                if (_onReserveCardClick) {
                    _onReserveCardClick(sprite->getCard()->getId());
                }
                return true;
            }
        }

        // 2. 检测 Playfield 区 (从顶层往下)
        auto& playfieldChildren = _playFieldArea->getChildren();
        for (ssize_t i = playfieldChildren.size() - 1; i >= 0; --i) {
            auto child = playfieldChildren.at(i);
            CardView* sprite = dynamic_cast<CardView*>(child);
            if (!sprite) continue;

            Vec2 localPos = sprite->getParent()->convertToNodeSpace(touchLoc);
            if (sprite->getBoundingBox().containsPoint(localPos)) {
                if (_onPlayfieldCardClick) {
                    _onPlayfieldCardClick(sprite->getCard()->getId());
                }
                return true;
            }
        }

        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

CardView* GameScene::getCardSpriteById(int id)
{
    // 1. 搜索 Playfield
    for (auto child : _playFieldArea->getChildren()) {
        auto sprite = dynamic_cast<CardView*>(child);
        if (sprite && sprite->getCard()->getId() == id) return sprite;
    }
    // 2. 搜索 Reserve
    for (auto child : _reserveArea->getChildren()) {
        auto sprite = dynamic_cast<CardView*>(child);
        if (sprite && sprite->getCard()->getId() == id) return sprite;
    }
    // 3. 搜索 Stack
    for (auto child : _stackArea->getChildren()) {
        auto sprite = dynamic_cast<CardView*>(child);
        if (sprite && sprite->getCard()->getId() == id) return sprite;
    }
    // 4. 搜索 Scene 直接子节点 (动画中的卡牌)
    for (auto child : this->getChildren()) {
        auto sprite = dynamic_cast<CardView*>(child);
        if (sprite && sprite->getCard()->getId() == id) return sprite;
    }
    return nullptr;
}

Vec2 GameScene::getStackCardPosition() const
{
    // 返回手牌堆中卡牌的位置
    return Vec2(160, 119);
}

void GameScene::updateReserveLayout(GameModel* model)
{
    if (!model) return;

    Vec2 reserveCenterPos(179, 119); 
    float xInterval = 30.0f;
    int totalReserve = model->reserveCards.size();

    for (int i = 0; i < totalReserve; ++i) {
        int id = model->reserveCards[i]->getId();
        
        CardView* sprite = getCardSpriteById(id);
        if (sprite && sprite->getParent() == _reserveArea) {
            float offsetX = (i - (totalReserve - 1) / 2.0f) * xInterval;
            Vec2 targetPos = Vec2(reserveCenterPos.x + offsetX, reserveCenterPos.y);
            
            // 带动画移动到新位置，增加效果
            sprite->stopAllActions();
            sprite->runAction(MoveTo::create(0.2f, targetPos));
        }
    }
}

void GameScene::showVictoryUI()
{
    // 1. 禁用触摸
    _eventDispatcher->removeEventListenersForTarget(this);

    // 2. 显示通关文字
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    auto label = Label::createWithSystemFont("\xE6\x81\xAD\xE5\x96\x9C\xE4\xBD\xA0\xE9\x80\x9A\xE5\x85\xB3", "SimHei", 96);
    if (label) {
        label->setColor(Color3B::WHITE);
        label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        this->addChild(label, 9999);
        
        // 弹出动画效果
        label->setScale(0.0f);
        label->runAction(EaseBackOut::create(ScaleTo::create(0.5f, 1.0f)));
    }
}