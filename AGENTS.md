# AGENTS.md

PerryBox — 基于 Qt 5.14.2 的桌面开发者工具箱（ASCII/时间戳/CRC/正则转换器）。C++17，qmake + MinGW，仅 Windows，中文界面与注释。

## 构建

- `build_run.bat` — 构建并以 debug 模式启动；`build_run.bat release` 构建 release 版本。构建前会先杀掉正在运行的 `PerryBox.exe`。
- qmake **不会**自动收集源文件。新增的 `.cpp`/`.h`/`.ui` 文件必须手动加入 `PerryBox.pro`（`SOURCES`/`HEADERS`/`FORMS`），否则不会参与编译。
- Qt 路径硬编码为 `D:\Qt\Qt5.14.2`（mingw73_64 / mingw730_64），写在 `build_run.bat` 和 `.vscode/c_cpp_properties.json` 中。Qt 不在该路径下会导致构建失败。
- `ui_*.h`、`moc_*.cpp`、`qrc_*.cpp` 由 uic/moc 生成到 `build/` 目录，已被 gitignore。切勿编辑它们；应改对应的 `.ui` 文件。
- fmt 内置于 `thirdparty/fmt-12.1.0`，由源码编译（`format.cc` 已加入 `.pro`）。不要尝试链接系统自带的 fmt。

## 验证

本项目没有测试、没有 CI、没有 lint/格式化工具。“验证”即运行 `build_run.bat`，确认能编译并启动。

## 架构

分层：`src/core/`（纯逻辑层，**不依赖 Qt**）+ `src/ui/`（界面层）。

- 入口：`src/main.cpp` -> `MainWindow`（`src/ui/mainwindow.*`）。主窗口只负责组装标签页（`addTab`）和"关于"菜单，不包含功能逻辑。
- 四个功能页面（`src/ui/*_page.{h,cpp,ui}`）：`ascii_converter_page`、`timestamp_converter_page`、`crc_calculator_page`、`regex_tester_page`。每个页面自带 `.ui`，信号处理写在各自 `.cpp`。
- `src/ui/byte_warn_highlighter.{h,cpp}`：4 个不可打印字符高亮器，公共基类 `ByteWarnHighlighter`，子类只需实现 `nextToken()` 解析一种进制 token。
- 纯逻辑层全部为 `namespace perry` 中的**无状态自由函数**（无单例）：`asciiConvert()`（入参含进制与前缀开关，返回 `AsciiConversionResult`）、`regexMatch()`/`regexReplace()`/`regexPresets()`、`crcModels()`/`crcCalc()`（参数模型表驱动）、`timestamp_converter` 的转换函数。错误契约：用户输入路径**不抛异常**，通过返回值（bool/struct 的 valid 字段/`std::string* error`）传递；用户可见文案只出现在 ui 层（内部工具函数对非法参数保留防御性 throw，正常调用不可达）。
- `perry_common.h` 定义 `BaseEnum`（BIN/OCT/DEC/HEX/ASCII）和 `baseStr2Nums()`。
- 新增功能：`src/core/` 写纯函数 -> `src/ui/` 建页面三件套 -> 注册到 `PerryBox.pro` -> `mainwindow.cpp` 中 `addTab`。

## 约定

- 命名：类/函数用 PascalCase，变量用 camelCase，常量用 UPPER_CASE，缩进为 4 空格（见 README）。
- 注释和用户可见文本均为中文；`PerryBox_zh_CN.ts` 是翻译源文件（`.pro` 中的 `TRANSLATIONS`）。
- 提交信息采用 Conventional Commits（`feat:`/`fix:`/`docs:`/`build:`/`chore:`）。
