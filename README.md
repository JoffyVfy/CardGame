# 纸牌消除游戏

一个使用 Cocos2d-x 3.17.2 引擎开发的跨平台卡牌消除游戏。

## 项目简介

这是一个经典的纸牌消除游戏，玩家需要通过策略性地移动卡牌来完成关卡挑战。游戏采用 MVC（Model-View-Controller）架构设计，代码结构清晰，易于维护和扩展。

## 功能特性

- 🎮 经典纸牌消除玩法
- 📱 跨平台支持（Android、iOS、macOS、Windows、Linux）
- 🔄 撤销操作支持
- 🎯 多关卡配置系统
- 🎨 流畅的动画效果

## 技术栈

- **游戏引擎**: Cocos2d-x 3.17.2
- **开发语言**: C++
- **构建工具**: CMake
- **配置格式**: JSON

## 项目结构

```
test3/
├── Classes/                    # 源代码目录
│   ├── AppDelegate.cpp/h       # 应用程序入口
│   ├── configs/                # 配置模块
│   │   ├── loaders/            # 配置加载器
│   │   └── models/             # 配置数据模型
│   ├── controllers/            # 控制器层
│   │   └── GameController      # 游戏主控制器
│   ├── managers/               # 管理器
│   │   └── GameManager         # 游戏状态管理
│   ├── models/                 # 数据模型层
│   │   ├── CardModel           # 卡牌数据模型
│   │   └── GameModel           # 游戏数据模型
│   ├── services/               # 服务层
│   │   └── GameGeneratorService # 游戏生成服务
│   └── views/                  # 视图层
│       ├── CardView            # 卡牌视图组件
│       └── GameScene           # 游戏主场景
├── Resources/                  # 资源文件目录
│   ├── fonts/                  # 字体资源
│   └── res/                    # 图片资源
├── proj.android/               # Android 平台工程
├── proj.ios_mac/               # iOS/macOS 平台工程
├── proj.linux/                 # Linux 平台工程
├── proj.win32/                 # Windows 平台工程
└── CMakeLists.txt              # CMake 构建配置
```

## 架构设计

项目采用 **MVC** 架构模式：

| 层级 | 组件 | 职责 |
|------|------|------|
| **Model** | CardModel, GameModel | 管理卡牌数据和游戏状态 |
| **View** | CardView, GameScene | 负责 UI 渲染和用户交互展示 |
| **Controller** | GameController | 处理用户输入和游戏逻辑协调 |

## 环境要求

- CMake 3.6 或更高版本
- Cocos2d-x 3.17.2
- 平台对应的开发环境：
  - **Android**: Android Studio + NDK
  - **iOS/macOS**: Xcode
  - **Windows**: Visual Studio
  - **Linux**: GCC/G++

## 构建说明

### 通用步骤

1. 克隆仓库：
   ```bash
   git clone https://github.com/JoffyVfy/test3.git
   cd test3
   ```

2. 确保 Cocos2d-x 引擎已正确安装并配置环境变量。

### Android 平台

```bash
cd proj.android
./gradlew assembleDebug
```

### iOS/macOS 平台

使用 Xcode 打开 `proj.ios_mac` 目录下的工程文件进行编译。

### Windows 平台

```bash
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
```

### Linux 平台

```bash
mkdir build
cd build
cmake ..
make
```

## 游戏玩法

1. 游戏场景分为三个区域：
   - **牌场区域** (Playfield): 显示可供选择的卡牌
   - **储备区域** (Reserve): 储备的卡牌堆
   - **牌堆区域** (Stack): 已消除的卡牌

2. 点击卡牌将其移动到牌堆中

3. 通过策略性地选择卡牌顺序来完成关卡

4. 支持撤销操作，可以回退上一步移动

## 许可证

本项目基于 MIT 许可证开源。
