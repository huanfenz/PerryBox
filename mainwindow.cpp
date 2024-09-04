#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ascii_converter.h"
#include "timestamp_converter.h"
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QTextEdit>

void MainWindow::setEditTextNoEvent(QTextEdit* const edit, const QString& text)
{
    edit->blockSignals(true);
    edit->setPlainText(text);
    edit->blockSignals(false);
}

void MainWindow::asciiConverterPage()
{
    // 获取ASCII转换器单例对象
    perry::AsciiConverter& asciiConverter = perry::AsciiConverter::getInstance();

    // ascii 输入框改变事件
    connect(ui->edit_ascii, &QTextEdit::textChanged, this, [&](){
        qDebug() << "edit_ascii changed";
        ui->label_info_ascii->setText("");
        std::string str = ui->edit_ascii->toPlainText().toStdString();
        asciiConverter.setAsciiStr(str);
        setEditTextNoEvent(ui->edit_hex, TO_QSTR(asciiConverter.getHexStr()));
        setEditTextNoEvent(ui->edit_dec, TO_QSTR(asciiConverter.getDecStr()));
        ui->edit_char_size->setText(QString::number(asciiConverter.getCharSize()));
    });

    // hex 输入框改变事件
    connect(ui->edit_hex, &QTextEdit::textChanged, this, [&](){
        qDebug() << "edit_hex changed";
        std::string str = ui->edit_hex->toPlainText().toStdString();
        if (!perry::AsciiConverter::checkHexStrValid(str)) {
            ui->label_info_ascii->setText("格式错误，请检查");
            return;
        } else {
            ui->label_info_ascii->setText("");
        }
        asciiConverter.setHexStr(ui->edit_hex->toPlainText().toStdString());
        setEditTextNoEvent(ui->edit_ascii, TO_QSTR(asciiConverter.getAsciiStr()));
        setEditTextNoEvent(ui->edit_dec, TO_QSTR(asciiConverter.getDecStr()));
        ui->edit_char_size->setText(QString::number(asciiConverter.getCharSize()));
    });

    // dec 输入框改变事件
    connect(ui->edit_dec, &QTextEdit::textChanged, this, [&](){
        qDebug() << "edit_dec changed";
        std::string str = ui->edit_dec->toPlainText().toStdString();
        if (!perry::AsciiConverter::checkDecStrValid(str)) {
            ui->label_info_ascii->setText("格式错误，请检查");
            return;
        } else {
            ui->label_info_ascii->setText("");
        }
        asciiConverter.setDecStr(str);
        setEditTextNoEvent(ui->edit_ascii, TO_QSTR(asciiConverter.getAsciiStr()));
        setEditTextNoEvent(ui->edit_hex, TO_QSTR(asciiConverter.getHexStr()));
        ui->edit_char_size->setText(QString::number(asciiConverter.getCharSize()));
    });

    // 重置按钮事件
    connect(ui->btn_reset_ascii, &QPushButton::clicked, this, [&](){
        ui->edit_ascii->setPlainText("");
        ui->edit_hex->setPlainText("");
        ui->edit_dec->setPlainText("");
        ui->label_info_ascii->setText("");
    });
}

void MainWindow::timestampConverterPage()
{
    // 初始化显示当前时间
    std::time_t now = std::time(nullptr);
    ui->edit_cur_timestamp->setText(TO_QSTR(std::to_string(now)));
    std::string localtimeStr = perry::timestamp2LocaltimeStr(now);
    ui->edit_cur_localtime->setText(TO_QSTR(localtimeStr));
    std::string utctimeStr = perry::timestamp2UtctimeStr(now);
    ui->edit_cur_utctime->setText(TO_QSTR(utctimeStr));

    // 更新当前时间事件
    connect(ui->btn_update_cur_time, &QPushButton::clicked, this, [&](){
        ui->label_info_timestamp->setText("");
        std::time_t now = std::time(nullptr);
        ui->edit_cur_timestamp->setText(TO_QSTR(std::to_string(now)));
        std::string timeStr = perry::timestamp2LocaltimeStr(now);
        ui->edit_cur_localtime->setText(TO_QSTR(timeStr));
        std::string utctimeStr = perry::timestamp2UtctimeStr(now);
        ui->edit_cur_utctime->setText(TO_QSTR(utctimeStr));
    });    

    // 时间戳转换
    connect(ui->btn_timestamp, &QPushButton::clicked, this, [&](){
        std::string reqStr = ui->edit_timestamp->text().toStdString();
        if (!perry::isValidTimestamp(reqStr)) {
            ui->label_info_timestamp->setText("输入格式不合法或超出范围");
            return;
        }
        ui->label_info_timestamp->setText("");
        // 获取时间戳
        std::time_t timestamp = static_cast<std::time_t>(std::stoll(reqStr));
        // 转换成本地时间
        std::string timeStr = perry::timestamp2LocaltimeStr(timestamp);
        ui->edit_localtime->setText(TO_QSTR(timeStr));
        // 转换成UTC时间
        std::string utcTimeStr = perry::timestamp2UtctimeStr(timestamp);
        ui->edit_utctime->setText(TO_QSTR(utcTimeStr));
    });

    // 本地时间转换
    connect(ui->btn_localtime, &QPushButton::clicked, this, [&](){
        std::string reqStr = ui->edit_localtime->text().toStdString();
        if (!perry::isValidTimeStr(reqStr)) {
            ui->label_info_timestamp->setText("输入格式不合法或超出范围");
            return;
        }
        // 获取时间戳
        std::time_t timestamp = perry::localtime2Timestamp(reqStr);
        if (timestamp < 0) {
            ui->label_info_timestamp->setText("输入超出范围");
            return;
        }
        ui->label_info_timestamp->setText("");
        // 直接显示
        ui->edit_timestamp->setText(TO_QSTR(std::to_string(timestamp)));
        // 转换成UTC时间
        std::string utcTimeStr = perry::timestamp2UtctimeStr(timestamp);
        ui->edit_utctime->setText(TO_QSTR(utcTimeStr));
    });

    // UTC时间转换
    connect(ui->btn_utctime, &QPushButton::clicked, this, [&](){
        std::string reqStr = ui->edit_utctime->text().toStdString();
        if (!perry::isValidTimeStr(reqStr)) {
            ui->label_info_timestamp->setText("输入格式不合法或超出范围");
            return;
        }
        // 获取时间戳
        std::time_t timestamp = perry::utctime2Timestamp(reqStr);
        if (timestamp < 0) {
            ui->label_info_timestamp->setText("输入超出范围");
            return;
        }
        ui->label_info_timestamp->setText("");
        // 直接显示
        ui->edit_timestamp->setText(TO_QSTR(std::to_string(timestamp)));
        // 转换成本地时间
        std::string timeStr = perry::timestamp2LocaltimeStr(timestamp);
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
