#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "timestamp_converter.h"
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QTextEdit>

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

void MainWindow::asciiConverterPage()
{
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/Image/BirdIcon4.png"));

    // 版本信息
    connect(ui->actionUpdateInfo, &QAction::triggered, this, [&](){
        QMessageBox::information(nullptr, "版本信息", "当前版本：" APP_VERSION "，欢迎使用");
    });

    // Ascii 转换器页面
    asciiConverterPage();

    // Timestamp 转换器页面
    timestampConverterPage();
}

MainWindow::~MainWindow()
{
    delete ui;
}
