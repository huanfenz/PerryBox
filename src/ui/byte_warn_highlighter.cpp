#include "byte_warn_highlighter.h"
#include <QColor>

ByteWarnHighlighter::ByteWarnHighlighter(QTextDocument* doc)
    : QSyntaxHighlighter(doc)
{
}

bool ByteWarnHighlighter::isPrintable(int value)
{
    return value >= 0x20 && value <= 0x7E;
}

bool ByteWarnHighlighter::isHexChar(QChar c)
{
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

int ByteWarnHighlighter::hexVal(QChar c)
{
    if (c >= '0' && c <= '9') return c.unicode() - '0';
    if (c >= 'a' && c <= 'f') return c.unicode() - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c.unicode() - 'A' + 10;
    return -1;
}

bool ByteWarnHighlighter::isDecChar(QChar c)
{
    return c >= '0' && c <= '9';
}

int ByteWarnHighlighter::decVal(QChar c)
{
    if (c >= '0' && c <= '9') return c.unicode() - '0';
    return -1;
}

bool ByteWarnHighlighter::isBinChar(QChar c)
{
    return c == '0' || c == '1';
}

void ByteWarnHighlighter::highlightBlock(const QString& text)
{
    QTextCharFormat warnFmt;
    warnFmt.setBackground(QColor(255, 220, 180)); // 浅橙背景
    warnFmt.setForeground(Qt::black);             // 字体仍然可读

    int pos = 0;
    ByteToken token;
    while (pos < text.size()) {
        if (nextToken(text, pos, token)) {
            if (token.checkable && !isPrintable(token.value)) {
                setFormat(token.start, token.length, warnFmt);
            }
        } else {
            ++pos;
        }
    }
}

/* ---------------- 十六进制 ---------------- */
bool HexWarnHighlighter::nextToken(const QString& text, int& pos, ByteToken& token)
{
    const int n = text.size();

    // 0xAB
    if (pos + 3 < n && text[pos] == '0' && text[pos + 1] == 'x'
        && isHexChar(text[pos + 2]) && isHexChar(text[pos + 3])) {
        token.start = pos;
        token.length = 4;
        token.value = (hexVal(text[pos + 2]) << 4) | hexVal(text[pos + 3]);
        pos += 4;
        return true;
    }

    // AB
    if (pos + 1 < n && isHexChar(text[pos]) && isHexChar(text[pos + 1])) {
        token.start = pos;
        token.length = 2;
        token.value = (hexVal(text[pos]) << 4) | hexVal(text[pos + 1]);
        pos += 2;
        return true;
    }

    return false;
}

/* ---------------- 十进制 ---------------- */
bool DecWarnHighlighter::nextToken(const QString& text, int& pos, ByteToken& token)
{
    const int n = text.size();

    // 最多取 3 位连续数字作为一组
    int digits = 0;
    while (pos + digits < n && digits < 3 && isDecChar(text[pos + digits])) {
        ++digits;
    }
    if (digits == 0) {
        return false;
    }

    int value = 0;
    for (int k = 0; k < digits; ++k) {
        value = value * 10 + decVal(text[pos + k]);
    }

    token.start = pos;
    token.length = digits;
    token.value = value;
    token.checkable = (value <= 255); // 超出 0~255 的组不参与可打印性检查
    pos += digits;
    return true;
}

/* ---------------- ASCII ---------------- */
bool AsciiWarnHighlighter::nextToken(const QString& text, int& pos, ByteToken& token)
{
    token.start = pos;
    token.length = 1;
    token.value = text[pos].unicode();
    token.checkable = true;
    ++pos;
    return true;
}

/* ---------------- 二进制 ---------------- */
bool BinWarnHighlighter::nextToken(const QString& text, int& pos, ByteToken& token)
{
    const int n = text.size();
    const int start = pos;
    int p = pos;

    // 0b 前缀
    if (p + 1 < n && text[p] == '0' && text[p + 1] == 'b') {
        p += 2;
    }

    // 最多 8 个二进制位
    int bitCount = 0;
    int value = 0;
    while (p < n && bitCount < 8 && isBinChar(text[p])) {
        value = (value << 1) | (text[p].unicode() - '0');
        ++bitCount;
        ++p;
    }

    token.start = start;
    token.length = p - start;
    token.value = value;
    token.checkable = (bitCount > 0);
    pos = p;
    return token.length > 0;
}
