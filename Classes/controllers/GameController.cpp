#include "GameController.h"
#include "views/GameScene.h"
#include "services/GameGeneratorService.h"

USING_NS_CC;

GameController::GameController() {
    _gameManager = new GameManager();
}

GameController* GameController::getInstance()
{
    static GameController instance;
    return &instance;
}

void GameController::startGame(GameScene* scene, int levelId) {
    _gameScene = scene;
    
    // 1. 加载关卡配置
    std::string filename = StringUtils::format("C:/Users/Joffy Vfy/Documents/CocosProgram/test3/Classes/configs/models/level_%d.json", levelId);
    _config = LevelConfigLoader::loadFromJson(filename);
    
    if (!_config) {
        CCLOG("Load level failed");
        return;
    }
    
    // 2. 生成游戏模型数据 (Service)
    GameModel* model = GameGeneratorService::generateGameModel(_config);
    
    // 3. 设置到管理器 (Manager)
    _gameManager->setGameModel(model);
    
    // 4. 初始化视图 (View)
    if(_gameScene) {
        // 绑定回调
        _gameScene->setOnReserveCardClick([this](int cardId) {
            this->handleReserveCardClick(cardId);
        });
        _gameScene->setOnPlayfieldCardClick([this](int cardId) {
            this->handlePlayfieldCardClick(cardId);
        });
        _gameScene->setOnUndoClick([this]() {
            this->undoLastMove();
        });

        _gameScene->initGameView(model);
    }
}


void GameController::handleReserveCardClick(int cardId)
{
    // 动画锁判断
    if (_isAnimating) return;

    // 获取数据从 Manager
    auto model = _gameManager->getGameModel();
    if (!model) return;
    
    auto& reserveList = model->reserveCards;

    if (reserveList.empty()) return;

    // 只允许点击最上层的卡牌（即备用牌区的最后一张，视图上应该在最上面）
    CardModel* topCard = reserveList.back();
    if (topCard->getId() != cardId) {
        return;
    }

    // 3. 执行移动逻辑
    moveReserveToStack(cardId);
}

void GameController::moveReserveToStack(int cardId)
{
    if (!_gameScene || !_gameManager) return;

    // 1. 获取 Sprite (View)
    CardView* sprite = _gameScene->getCardSpriteById(cardId);
    if (!sprite) return;

    // 2. 更新数据模型 (Model)
    if (!_gameManager->moveReserveToStack(cardId)) {
        return;
    }

    // 3. 将精灵从当前父节点移到 Scene 下
    Vec2 worldPos = sprite->getParent()->convertToWorldSpace(sprite->getPosition());
    sprite->retain();
    sprite->removeFromParent();
    
    _gameScene->addChild(sprite);
    sprite->setPosition(_gameScene->convertToNodeSpace(worldPos));
    sprite->release();

    // 重新排列备用牌区剩余卡牌的位置 (动画)
    _gameScene->updateReserveLayout(_gameManager->getGameModel());

    // 4. 计算目标位置
    Node* stackArea = _gameScene->getChildByTag(101);
    Vec2 targetLocalPos = _gameScene->getStackCardPosition();
    Vec2 targetWorldPos = Vec2(810, 290); // Default
    if (stackArea) {
        targetWorldPos = stackArea->convertToWorldSpace(targetLocalPos);
    }

    // 设置动画锁
    _isAnimating = true;

    auto moveTo = MoveTo::create(0.5f, _gameScene->convertToNodeSpace(targetWorldPos));
    
    auto callback = CallFunc::create([=]() {
        _isAnimating = false; // 解锁

        if (stackArea) {
            sprite->removeFromParent();
            stackArea->addChild(sprite);
            sprite->setPosition(targetLocalPos);
        }
        CCLOG("Card %d moved to Stack Top", cardId);
    });
    
    sprite->runAction(Sequence::create(moveTo, callback, nullptr));
}

void GameController::handlePlayfieldCardClick(int cardId)
{
    // 动画锁判断
    if (_isAnimating) return;

    auto model = _gameManager->getGameModel();
    if (!model) return;

    auto& stackList = model->stackCards;
    if (stackList.empty()) return;

    CardModel* topStackCard = stackList.back();
    int stackFace = topStackCard->getFace();

    // 检查点击的牌是否在主牌区
    CardModel* clickedCard = model->getCardById(cardId);
    if (!clickedCard) return;

    // 遮挡检测 (Occlusion Culling)
    CardView* clickedSprite = _gameScene->getCardSpriteById(cardId);
    if (!clickedSprite) return;

    Rect myRect = clickedSprite->getBoundingBox();
    auto& playfieldList = model->playfieldCards;
    
    // 查找当前索引
    int clickedIndex = -1;
    for (int i = 0; i < playfieldList.size(); ++i) {
        if (playfieldList[i]->getId() == cardId) {
            clickedIndex = i;
            break;
        }
    }

    // 检测是否被遮挡
    for (int i = clickedIndex + 1; i < playfieldList.size(); ++i) {
        CardView* otherSprite = _gameScene->getCardSpriteById(playfieldList[i]->getId());
        if (otherSprite && myRect.intersectsRect(otherSprite->getBoundingBox())) {
            CCLOG("Card %d is blocked", cardId);
            return; 
        }
    }

    int cardFace = clickedCard->getFace();
    int diff = std::abs(cardFace - stackFace);
    
    if (diff == 1) {
        movePlayfieldToStack(cardId);
    } else {
        CCLOG("Match failed: %d vs %d", cardFace, stackFace);
    }
}

void GameController::movePlayfieldToStack(int cardId)
{
    if (!_gameScene || !_gameManager) return;

    // 1. 获取 Sprite
    CardView* sprite = _gameScene->getCardSpriteById(cardId);
    if (!sprite) return;

    // 2. 更新数据模型
    if (!_gameManager->movePlayfieldToStack(cardId)) {
        return;
    }

    // 3. 移动到场景下
    Vec2 worldPos = sprite->getParent()->convertToWorldSpace(sprite->getPosition());
    sprite->retain();
    sprite->removeFromParent();
    
    _gameScene->addChild(sprite);
    sprite->setPosition(_gameScene->convertToNodeSpace(worldPos));
    sprite->release();

    // 4. 计算目标
    Node* stackArea = _gameScene->getChildByTag(101);
    Vec2 targetLocalPos = _gameScene->getStackCardPosition();
    Vec2 targetWorldPos = Vec2(810, 290); 
    if (stackArea) {
        targetWorldPos = stackArea->convertToWorldSpace(targetLocalPos);
    }
    
    // 设置动画锁
    _isAnimating = true;

    auto moveTo = MoveTo::create(0.5f, _gameScene->convertToNodeSpace(targetWorldPos));
    
    auto callback = CallFunc::create([=]() {
        _isAnimating = false; // 解锁

        if (stackArea) {
            sprite->removeFromParent();
            stackArea->addChild(sprite);
            sprite->setPosition(targetLocalPos);
        }
        CCLOG("Playfield Card %d moved to Stack", cardId);
        
        // 检查胜利
        this->checkVictory();
    });
    
    sprite->runAction(Sequence::create(moveTo, callback, nullptr));
}

void GameController::undoLastMove()
{
    if (!_gameScene || !_gameManager) return;

    // 如果正在播放动画，则不允许操作，避免状态混乱
    if (_isAnimating) {
        CCLOG("Animation in progress, undo blocked.");
        return;
    }

    // 1. 执行数据回退 (Manager)
    int cardId = _gameManager->moveStackToReserve();
    if (cardId == -1) {
        CCLOG("Cannot undo");
        return;
    }

    // 2. 获取 Sprite (View)
    CardView* sprite = _gameScene->getCardSpriteById(cardId);
    if (!sprite) return;

    // 3. 将精灵移动到 Scene 下
    Vec2 currentWorldPos = sprite->getPosition();
    if (sprite->getParent()) {
        currentWorldPos = sprite->getParent()->convertToWorldSpace(sprite->getPosition());
    }

    sprite->retain();
    sprite->removeFromParent();

    _gameScene->addChild(sprite);
    sprite->setPosition(_gameScene->convertToNodeSpace(currentWorldPos));
    sprite->release();

    // 4. 计算目标位置：移动到备用牌区 (Reserve)
    Node* reserveArea = _gameScene->getChildByTag(100);
    Vec2 targetWorldPos = Vec2(270, 290); // Default fallback
    Vec2 reserveCenterPos(179, 119); // 与 GameScene 中保持一致

    if (reserveArea) {
        targetWorldPos = reserveArea->convertToWorldSpace(reserveCenterPos);
    }

    // 设置动画锁
    _isAnimating = true;

    auto moveTo = MoveTo::create(0.5f, _gameScene->convertToNodeSpace(targetWorldPos));

    auto callback = CallFunc::create([=]() {
        _isAnimating = false; // 解锁

        // 5. 动画完成后，将其放入 ReserveArea 并重排
        if (reserveArea) {
            sprite->removeFromParent();
            reserveArea->addChild(sprite);
            
            // 第2步 Reserve 区重排位置（带动画效果）
            _gameScene->updateReserveLayout(_gameManager->getGameModel());
        } else {
             CCLOG("Warning: Could not find ReserveArea (Tag 100) for undo.");
        }

        CCLOG("Undo complete (Stateless)");
    });

    sprite->runAction(Sequence::create(moveTo, callback, nullptr));
}

void GameController::checkVictory()
{
    if (!_gameManager || !_gameScene) return;

    auto model = _gameManager->getGameModel();
    if (!model) return;

    // 当主牌区没有剩余卡牌时胜利
    if (model->playfieldCards.empty()) {
        _gameScene->showVictoryUI();
        CCLOG("Game Victory!");
    }
}