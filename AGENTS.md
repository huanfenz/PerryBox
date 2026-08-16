# AGENTS.md

PerryBox — 基于 Qt 5.14.2 的桌面开发者工具箱（ASCII/时间戳/CRC/正则转换器）。C++17，qmake + MinGW，仅 Windows，中文界面与注释。

## 构建

- `build_run.bat` — 构建并以 debug 模式启动；`build_run.bat release` 构建 release 版本。构建前会先杀掉正在运行的 `PerryBox.exe`。
- qmake **不会**自动收集源文件。新增的 `.cpp`/`.h`/`.ui` 文件必须手动加入 `PerryBox.pro`（`SOURCES`/`HEADERS`/`FORMS`），否则不会参与编译。
- Qt 路径硬编码为 `D:\Qt\Qt5.14.2`（mingw73_64 / mingw730_64），写在 `build_run.bat` 和 `.vscode/c_cpp_properties.json` 中。Qt 不在该路径下会导致构建失败。
- `ui_mainwindow.h`、`moc_*.cpp`、`qrc_*.cpp` 由 uic/moc 生成到 `build/` 目录，已被 gitignore。切勿编辑它们；应改 `mainwindow.ui`。
- fmt 内置于 `thirdparty/fmt-12.1.0`，由源码编译（`format.cc` 已加入 `.pro`）。不要尝试链接系统自带的 fmt。

## 验证

本项目没有测试、没有 CI、没有 lint/格式化工具。“验证”即运行 `build_run.bat`，确认能编译并启动。

## 架构

- 入口：`main.cpp` -> `MainWindow`（`mainwindow.h/.cpp`，界面在 `mainwindow.ui`）。
- 四个功能模块，均为 `.h`/`.cpp` 成对：`ascii_converter`、`timestamp_converter`、`crc_calculator`、`regex_tester`。
- 纯逻辑位于 `namespace perry`，与 UI 无关。`AsciiConverter` 和 `RegexTester` 是单例（`getInstance()`）；`crc_calculator`/`timestamp_converter` 暴露自由函数。
- `perry_common.h` 定义 `BaseEnum`（BIN/OCT/DEC/HEX/ASCII）和 `baseStr2Nums()`。
- `mainwindow.cpp` 负责把模块接入 UI；信号处理函数在此调用模块 API，而非反向调用。
- 新增功能：新建 `.h/.cpp`，注册到 `PerryBox.pro`，在 `mainwindow.ui` 添加标签页，在 `mainwindow.cpp` 中编写接入逻辑。

## 约定

- 命名：类/函数用 PascalCase，变量用 camelCase，常量用 UPPER_CASE，缩进为 4 空格（见 README）。
- 注释和用户可见文本均为中文；`PerryBox_zh_CN.ts` 是翻译源文件（`.pro` 中的 `TRANSLATIONS`）。
- 提交信息采用 Conventional Commits（`feat:`/`fix:`/`docs:`/`build:`/`chore:`）。
