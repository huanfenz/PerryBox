# PerryBox - 多功能开发者工具箱

PerryBox 是一个基于 Qt 框架开发的多功能桌面应用程序，集成了多种常用的开发工具，提供简单直观的用户界面，帮助开发者快速完成常见的编码、转换和测试任务。

## 功能特性

### 1. ASCII 转换器
- 支持二进制、十进制、十六进制之间的相互转换
- 支持前缀显示切换（如 0x、0b 前缀）
- 实时更新转换结果
- 支持批量字符处理

### 2. 时间戳转换器
- 时间戳与本地时间/UTC 时间互转
- 支持多种时间格式验证
- 精确到秒的时间计算
- 用户友好的时间输入界面

### 3. CRC 校验计算器
- 支持 CRC8 和 CRC32 校验算法
- 字节级数据处理
- 支持多种输入格式
- 快速计算校验值

### 4. 正则表达式测试器
- 正则表达式验证和匹配
- 支持多种正则表达式标志
- 提供预定义正则表达式模板
- 支持正则替换功能
- 结果格式化输出
- 匹配位置和长度显示

## 技术规格

- **开发语言**: C++17
- **框架**: Qt 5.14.2
- **编译器**: MinGW 7.3.0 (32位)
- **构建系统**: qmake + Makefile
- **第三方库**: fmt-12.1.0 (格式化库)
- **操作系统**: Windows 10/11 (支持高分屏适配)

## 快速开始

### 构建和运行

#### 方法 1: 使用批处理脚本（推荐）

```bash
# 调试模式（默认）
build_run.bat

# 发布模式
build_run.bat release
```

#### 方法 2: 手动构建

```bash
# 1. 配置 Qt 环境变量
set QtPath=D:\Qt\Qt5.14.2\5.14.2\mingw73_64
set MingwPath=D:\Qt\Qt5.14.2\Tools\mingw730_64
set PATH=%MingwPath%\bin;%QtPath%\bin;%PATH%

# 2. 创建构建目录
mkdir build
cd build

# 3. 运行 qmake
qmake ..\PerryBox.pro -spec win32-g++ "CONFIG+=debug"

# 4. 编译项目
mingw32-make -j4

# 5. 运行程序
debug\PerryBox.exe
```

## 项目结构

```
PerryBox/
├── src/
│   ├── main.cpp                    # 程序入口
│   ├── core/                       # 纯逻辑层（不依赖 Qt）
│   │   ├── ascii_converter.h/.cpp      # ASCII 转换（无状态转换函数）
│   │   ├── timestamp_converter.h/.cpp  # 时间戳转换
│   │   ├── crc_calculator.h/.cpp       # CRC 计算（参数模型表驱动）
│   │   ├── regex_tester.h/.cpp         # 正则匹配/替换/预设表
│   │   └── perry_common.h/.cpp         # 公用工具函数
│   └── ui/                         # 界面层
│       ├── mainwindow.h/.cpp/.ui        # 主窗口：组装各标签页
│       ├── ascii_converter_page.*       # ASCII 转换页
│       ├── timestamp_converter_page.*   # 时间戳转换页
│       ├── crc_calculator_page.*        # CRC 校验页
│       ├── regex_tester_page.*          # 正则测试页
│       └── byte_warn_highlighter.h/.cpp # 不可打印字符高亮器
├── 资源
│   ├── res.qrc                     # Qt 资源文件
│   └── BirdIcon4.ico               # 应用图标
├── 构建脚本
│   └── build_run.bat               # 自动构建脚本
├── 第三方库
│   └── thirdparty/fmt-12.1.0/      # fmt 格式化库
└── 配置
    ├── .vscode/                    # VSCode 配置
    └── PerryBox.pro                # Qt 项目配置
```

## 使用说明

### 启动应用

运行 `PerryBox.exe` 启动应用程序，主界面包含四个功能标签页，点击标签页即可切换功能。

### 常见问题

1. **程序无法启动**
   - 确保已正确配置 Qt 环境
   - 检查是否缺少 Qt 运行时库

2. **高分屏显示问题**
   - 应用已优化高分屏适配
   - 如遇显示异常，尝试调整系统缩放比例

3. **构建失败**
   - 检查 Qt 路径是否正确
   - 确认 MinGW 编译器是否正常
   - 尝试删除 build 目录后重新构建

## 开发说明

### 添加新功能

1. 纯逻辑放入 `src/core/`（新建 `new_feature.h/.cpp`，提供无状态转换函数）
2. 界面放入 `src/ui/`（新建 `new_feature_page.h/.cpp/.ui`，参照现有页面）
3. 在 `PerryBox.pro` 中注册新增的源文件和 UI 文件
4. 在 `mainwindow.cpp` 中 `addTab(new NewFeaturePage(this), ...)` 挂载新标签页
5. 连接信号和槽函数

### 代码风格

- 遵循 Qt 编程规范
- 使用 4 个空格缩进
- 类名使用 PascalCase
- 变量名使用 camelCase
- 常量使用 UPPER_CASE
- 函数名使用 PascalCase

## 更新日志

### v1.0.0 (2026-08-18)
- 首个正式版，新增 Inno Setup 单文件安装包（简中安装界面）
- 新增一键打包脚本 `make_installer.bat`（构建 + 部署运行库 + 打包）
- 发版版本号双写同步：`APP_VERSION` 与 `MyAppVersion`

### v0.6.0 (2026-08-17)
- 架构重构：拆分 core（纯逻辑层）/ ui（界面层），逻辑层纯函数化
- 各功能页输入框统一添加占位提示
- 修复带前缀进制字符串解析结果为 0 的问题
- 移除正则测试器无实际作用的全局匹配复选框
- 添加人工测试用例文档

### v0.0.4 (2025-02-19)
- 添加正则表达式测试器模块
- 优化 ASCII 转换器，支持二进制转换
- 改进构建脚本，防止程序重复运行
- 配置 VSCode 开发环境

### v0.0.3 (2025-02-15)
- 添加时间戳转换器功能
- 添加 CRC 校验计算器功能
- 完善项目配置和构建脚本

### v0.0.1 (2025-06-08)
- 项目初始化
- 实现基础的 ASCII 转换器
- 配置 Qt 开发环境

## 许可证

MIT License

## 联系方式

如有问题或建议，欢迎通过以下方式联系：

- 作者: Perry
- 项目主页: [GitHub Repository]
- 邮箱: huanfenz@qq.com

---

**注**: 这是一个开源项目，欢迎提交 Issue 和 Pull Request。
