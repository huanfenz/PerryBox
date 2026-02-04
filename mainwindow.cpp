#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ascii_converter.h"
#include "timestamp_converter.h"
#include "crc_calculator.h"
#include <sstream>
#include <iomanip>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QTextEdit>
#include <QSyntaxHighlighter>

using namespace perry;

void MainWindow::setEditTextNoEvent(QTextEdit* const edit, const QString& text)
{
    edit->blockSignals(true);
    edit->setPlainText(text);
    edit->blockSignals(false);
}

void MainWindow::handleEditChanged(BaseEnum base)
{
    AsciiConverter& asciiConverter = AsciiConverter::getInstance();
    std::map<BaseEnum, QTextEdit*> updateMap = {
        { BaseEnum::ASCII,  ui->edit_ascii },
        { BaseEnum::HEX,    ui->edit_hex },
        { BaseEnum::DEC,    ui->edit_dec },
    };

    if (updateMap.find(base) == updateMap.end()) {
        return;
    }
    QTextEdit* currEdit = updateMap[base];

    // 从edit获取str
    std::string str = currEdit->toPlainText().toStdString();

    // 设置字符串，转换器会自动转换，返回false表示校验失败
    bool vaild = asciiConverter.setStrByType(str, base);
    if (!vaild) {
        ui->label_info_ascii->setText("格式错误，请检查");
        return;
    } else {
        ui->label_info_ascii->setText("");
    }

    // 设置除了自己的其他edit
    for (const auto& pair : updateMap) {
        if (pair.first == base) {
            continue;
        }
        setEditTextNoEvent(pair.second, TO_QSTR(asciiConverter.getStrByType(pair.first)));
    }

    // 设置字符长度text
    ui->edit_char_size->setText(QString::number(asciiConverter.getCharSize()));
}

class EditHexHighlighter : public QSyntaxHighlighter
{
public:
    explicit EditHexHighlighter(QTextDocument* doc)
        : QSyntaxHighlighter(doc) {}

protected:
    static bool isHexChar(QChar c)
    {
        return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
    }

    static int hexVal(QChar c)
    {
        if (c >= '0' && c <= '9') return c.unicode() - '0';
        if (c >= 'a' && c <= 'f') return c.unicode() - 'a' + 10;
        if (c >= 'A' && c <= 'F') return c.unicode() - 'A' + 10;
        return -1;
    }

    static bool isAsciiPrintable(uint8_t v)
    {
        return v >= 0x20 && v <= 0x7E;
    }

    void highlightBlock(const QString& text) override
    {
        QTextCharFormat warnFmt;
//        warnFmt.setForeground(QColor("#E67E22")); // 橙色
        warnFmt.setBackground(QColor(255, 220, 180)); // 浅橙
        warnFmt.setForeground(Qt::black);             // 字体仍然可读

        for (int i = 0; i < text.size(); )
        {
            // 0xAB
            if (i + 3 < text.size() && text[i] == '0' && text[i + 1] == 'x' && isHexChar(text[i + 2]) && isHexChar(text[i + 3]))
            {
                int h = hexVal(text[i + 2]);
                int l = hexVal(text[i + 3]);
                uint8_t v = (h << 4) | l;

                if (!isAsciiPrintable(v))
                    setFormat(i, 4, warnFmt);

                i += 4;
                continue;
            }

            // AB
            if (i + 1 < text.size() && isHexChar(text[i]) && isHexChar(text[i + 1]))
            {
                int h = hexVal(text[i]);
                int l = hexVal(text[i + 1]);
                uint8_t v = (h << 4) | l;

                if (!isAsciiPrintable(v))
                    setFormat(i, 2, warnFmt);

                i += 2;
                continue;
            }

            ++i;
        }
    }
};

class EditDecHighlighter : public QSyntaxHighlighter
{
public:
    explicit EditDecHighlighter(QTextDocument* doc)
        : QSyntaxHighlighter(doc) {}

protected:
    static bool isDecChar(QChar c)
    {
        return (c >= '0' && c <= '9');
    }

    static int decVal(QChar c)
    {
        if (c >= '0' && c <= '9') {
            return c.unicode() - '0';
        }
        return -1;
    }

    static bool isAsciiPrintable(uint8_t v)
    {
        return v >= 0x20 && v <= 0x7E;
    }

    void highlightBlock(const QString& text) override
    {
        QTextCharFormat warnFmt;
//        warnFmt.setForeground(QColor("#E67E22")); // 橙色
        warnFmt.setBackground(QColor(255, 220, 180)); // 浅橙
        warnFmt.setForeground(Qt::black);             // 字体仍然可读

        for (int i = 0; i < text.size(); )
        {
            // xxx
            if (i + 2 < text.size() && isDecChar(text[i]) && isDecChar(text[i + 1]) && isDecChar(text[i + 2])) {
                int a = decVal(text[i]);
                int b = decVal(text[i + 1]);
                int c = decVal(text[i + 2]);
                int value = a * 100 + b * 10 + c;
                if (value < 0 || value > 255) {
                    i += 3;
                    continue;
                }
                uint8_t v = value;
                if (!isAsciiPrintable(v)) {
                    setFormat(i, 3, warnFmt);
                }
                i += 3;
                continue;
            }

            // xx
            if (i + 1 < text.size() && isDecChar(text[i]) && isDecChar(text[i + 1])) {
                int a = decVal(text[i]);
                int b = decVal(text[i + 1]);
                int value = a * 10 + b;
                if (value < 0) {
                    i += 2;
                    continue;
                }
                uint8_t v = value;
                if (!isAsciiPrintable(v)) {
                    setFormat(i, 2, warnFmt);
                }
                i += 2;
                continue;
            }

            // x
            if (isDecChar(text[i])) {
                int value = decVal(text[i]);
                if (value < 0) {
                    ++i;
                    continue;
                }
                uint8_t v = value;
                if (!isAsciiPrintable(v))
                    setFormat(i, 1, warnFmt);
                ++i;
                continue;
            }

            ++i;
        }
    }
};

class EditAsciiHighlighter : public QSyntaxHighlighter
{
public:
    explicit EditAsciiHighlighter(QTextDocument* doc)
        : QSyntaxHighlighter(doc) {}

protected:
    static bool isAsciiPrintable(QChar c)
    {
        ushort u = c.unicode();
        return (u >= 0x20 && u <= 0x7E);
    }

    void highlightBlock(const QString& text) override
    {
        QTextCharFormat warnFmt;
        warnFmt.setBackground(QColor(255, 220, 180)); // 浅橙
        warnFmt.setForeground(Qt::black);             // 字体仍然可读

        for (int i = 0; i < text.size(); ++i)
        {
            QChar c = text[i];

            // 非 ASCII 或不可打印
            if (!isAsciiPrintable(c))
            {
                setFormat(i, 1, warnFmt);
            }
        }
    }
};

void MainWindow::asciiConverterPage()
{
    new EditHexHighlighter(ui->edit_hex->document());
    new EditDecHighlighter(ui->edit_dec->document());
    new EditAsciiHighlighter(ui->edit_ascii->document());

    // ascii 输入框改变事件
    connect(ui->edit_ascii, &QTextEdit::textChanged, this, [&](){
        handleEditChanged(BaseEnum::ASCII);
    });

    // hex 输入框改变事件
    connect(ui->edit_hex, &QTextEdit::textChanged, this, [&](){
        handleEditChanged(BaseEnum::HEX);
    });

    // dec 输入框改变事件
    connect(ui->edit_dec, &QTextEdit::textChanged, this, [&](){
        handleEditChanged(BaseEnum::DEC);
    });

    // 重置按钮事件
    connect(ui->btn_reset_ascii, &QPushButton::clicked, this, [&](){
        ui->edit_ascii->setPlainText("");
        ui->edit_hex->setPlainText("");
        ui->edit_dec->setPlainText("");
        ui->label_info_ascii->setText("");
    });

    // 切换十六进制前缀
    connect(ui->cbox_hex_prefix, &QCheckBox::toggled, this, [&](bool checked){
        AsciiConverter& asciiConverter = AsciiConverter::getInstance();
        asciiConverter.togglePrefix(checked);
        handleEditChanged(BaseEnum::ASCII);
    });
}

void MainWindow::timestampConverterPage()
{
    // 初始化显示当前时间
    std::time_t now = std::time(nullptr);
    ui->edit_timestamp->setText(TO_QSTR(std::to_string(now)));
    std::string localtimeStr = timestamp2LocaltimeStr(now);
    ui->edit_localtime->setText(TO_QSTR(localtimeStr));
    std::string utctimeStr = timestamp2UtctimeStr(now);
    ui->edit_utctime->setText(TO_QSTR(utctimeStr));

    // 更新当前时间事件
    connect(ui->btn_update_time, &QPushButton::clicked, this, [&](){
        ui->label_info_timestamp->setText("");
        std::time_t now = std::time(nullptr);
        ui->edit_timestamp->setText(TO_QSTR(std::to_string(now)));
        std::string timeStr = timestamp2LocaltimeStr(now);
        ui->edit_localtime->setText(TO_QSTR(timeStr));
        std::string utctimeStr = timestamp2UtctimeStr(now);
        ui->edit_utctime->setText(TO_QSTR(utctimeStr));
    });

    // 时间戳转换
    connect(ui->btn_timestamp, &QPushButton::clicked, this, [&](){
        std::string reqStr = ui->edit_timestamp->text().toStdString();
        if (!isValidTimestamp(reqStr)) {
            ui->label_info_timestamp->setText("输入格式不合法或超出范围");
            return;
        }
        ui->label_info_timestamp->setText("");
        // 获取时间戳
        std::time_t timestamp = static_cast<std::time_t>(std::stoll(reqStr));
        // 转换成本地时间
        std::string timeStr = timestamp2LocaltimeStr(timestamp);
        ui->edit_localtime->setText(TO_QSTR(timeStr));
        // 转换成UTC时间
        std::string utcTimeStr = timestamp2UtctimeStr(timestamp);
        ui->edit_utctime->setText(TO_QSTR(utcTimeStr));
    });

    // 本地时间转换
    connect(ui->btn_localtime, &QPushButton::clicked, this, [&](){
        std::string reqStr = ui->edit_localtime->text().toStdString();
        if (!isValidTimeStr(reqStr)) {
            ui->label_info_timestamp->setText("输入格式不合法或超出范围");
            return;
        }
        // 获取时间戳
        std::time_t timestamp = localtime2Timestamp(reqStr);
        if (timestamp < 0) {
            ui->label_info_timestamp->setText("输入超出范围");
            return;
        }
        ui->label_info_timestamp->setText("");
        // 直接显示
        ui->edit_timestamp->setText(TO_QSTR(std::to_string(timestamp)));
        // 转换成UTC时间
        std::string utcTimeStr = timestamp2UtctimeStr(timestamp);
        ui->edit_utctime->setText(TO_QSTR(utcTimeStr));
    });

    // UTC时间转换
    connect(ui->btn_utctime, &QPushButton::clicked, this, [&](){
        std::string reqStr = ui->edit_utctime->text().toStdString();
        if (!isValidTimeStr(reqStr)) {
            ui->label_info_timestamp->setText("输入格式不合法或超出范围");
            return;
        }
        // 获取时间戳
        std::time_t timestamp = utctime2Timestamp(reqStr);
        if (timestamp < 0) {
            ui->label_info_timestamp->setText("输入超出范围");
            return;
        }
        ui->label_info_timestamp->setText("");
        // 直接显示
        ui->edit_timestamp->setText(TO_QSTR(std::to_string(timestamp)));
        // 转换成本地时间
        std::string timeStr = timestamp2LocaltimeStr(timestamp);
        ui->edit_localtime->setText(TO_QSTR(timeStr));
    });

    // 重置按钮事件
    connect(ui->btn_reset_timestamp, &QPushButton::clicked, this, [&](){
        ui->edit_timestamp->setText("");
        ui->edit_localtime->setText("");
        ui->edit_utctime->setText("");
    });
}

void MainWindow::crcCalculatorPage()
{
    // 时间戳转换
    connect(ui->btn_crc_calc, &QPushButton::clicked, this, [&](){
        // 获取输入
        std::string inputStr = ui->pedit_crc_input->toPlainText().toStdString();
        std::vector<uint8_t> inputNums = baseStr2Nums(inputStr, BaseEnum::HEX);

        for (uint8_t num : inputNums) {
            qDebug() << num;
        }
        // 获取crc模型
        int crcModelIndex = ui->cbox_crc_model->currentIndex();

        std::string res;
        switch (crcModelIndex)
        {
            case 0: {// crc8
                uint8_t value = calcCRC8(inputNums);

                std::stringstream ss;
                ss << std::hex << std::uppercase
                   << std::setw(2) << std::setfill('0')
                   << static_cast<int>(value);
                res = ss.str();   // "AF"
                break;
            }
            case 1: {// crc32
                uint32_t value = calcCRC8(inputNums);

                std::stringstream ss;
                ss << std::hex << std::uppercase
                   << std::setw(8) << std::setfill('0')
                   << static_cast<int>(value);
                res = ss.str();   // "AAAF"
                break;
            }
            default:
                QMessageBox::critical(this, "错误", "参数模型超出范围");
                break;
        }

        ui->ledit_crc_result->setText(TO_QSTR(res));
    });
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/Image/BirdIcon4.png"));

    // 版本信息
    connect(ui->actionUpdateInfo, &QAction::triggered, this, [&](){
        QMessageBox::information(nullptr, "版本信息", "当前版本：" APP_VERSION "，Created By Perry.\n"
                                 "0.6版本更新日志：\n"
                                 "1. crc计算器基本实现\n"
                                 "0.5版本更新日志：\n"
                                 "1. Fix Some Bugs\n"
                                 "2. ASCII转换器：不可显示字符添加橙色背景显示\n"
                                 "0.4版本更新日志：\n"
                                 "1. 添加图标, 优化不同分辨率缩放的显示\n"
                                 "2. ASCII转换器：不可显示字符添加方框显示\n"
                                 "3. 时间戳转换器：基本完善");
    });

    // Ascii 转换器页面
    asciiConverterPage();

    // Timestamp 转换器页面
    timestampConverterPage();

    // CRC 计算器页面
    crcCalculatorPage();
}

MainWindow::~MainWindow()
{
    delete ui;
}
