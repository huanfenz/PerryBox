#ifndef BYTE_WARN_HIGHLIGHTER_H
#define BYTE_WARN_HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

/* 字节 token：高亮器扫描文本的基本单位 */
struct ByteToken
{
    int start = 0;         // token 起始位置
    int length = 0;        // token 长度
    int value = 0;         // token 表示的数值（ASCII 场景为完整 unicode 码点）
    bool checkable = true; // false 表示跳过可打印性检查（如超出 0~255 的十进制组）
};

/* 公共基类：逐 token 扫描文本，对不可打印的值加浅橙色背景 */
class ByteWarnHighlighter : public QSyntaxHighlighter
{
public:
    explicit ByteWarnHighlighter(QTextDocument* doc);

protected:
    void highlightBlock(const QString& text) override;

    /* 在 pos 处解析下一个 token：
       - 解析成功返回 true，pos 前进到 token 末尾之后，token 填充完毕
       - 解析失败返回 false，pos 保持不变（基类将前进一步） */
    virtual bool nextToken(const QString& text, int& pos, ByteToken& token) = 0;

    static bool isPrintable(int value);
    static bool isHexChar(QChar c);
    static int hexVal(QChar c);
    static bool isDecChar(QChar c);
    static int decVal(QChar c);
    static bool isBinChar(QChar c);
};

/* 十六进制输入框高亮器：识别 0xAB / AB */
class HexWarnHighlighter : public ByteWarnHighlighter
{
public:
    explicit HexWarnHighlighter(QTextDocument* doc) : ByteWarnHighlighter(doc) {}

protected:
    bool nextToken(const QString& text, int& pos, ByteToken& token) override;
};

/* 十进制输入框高亮器：识别 1~3 位数字组 */
class DecWarnHighlighter : public ByteWarnHighlighter
{
public:
    explicit DecWarnHighlighter(QTextDocument* doc) : ByteWarnHighlighter(doc) {}

protected:
    bool nextToken(const QString& text, int& pos, ByteToken& token) override;
};

/* ASCII 输入框高亮器：逐字符检查 */
class AsciiWarnHighlighter : public ByteWarnHighlighter
{
public:
    explicit AsciiWarnHighlighter(QTextDocument* doc) : ByteWarnHighlighter(doc) {}

protected:
    bool nextToken(const QString& text, int& pos, ByteToken& token) override;
};

/* 二进制输入框高亮器：识别 0b01000001 / 01000001 */
class BinWarnHighlighter : public ByteWarnHighlighter
{
public:
    explicit BinWarnHighlighter(QTextDocument* doc) : ByteWarnHighlighter(doc) {}

protected:
    bool nextToken(const QString& text, int& pos, ByteToken& token) override;
};

#endif // BYTE_WARN_HIGHLIGHTER_H
