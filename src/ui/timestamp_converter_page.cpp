#include "timestamp_converter_page.h"
#include "ui_timestamp_converter_page.h"
#include "timestamp_converter.h"
#include <QPushButton>

using namespace perry;

TimestampConverterPage::TimestampConverterPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TimestampConverterPage)
{
    ui->setupUi(this);

    // 初始化显示当前时间
    std::time_t now = std::time(nullptr);
    ui->edit_timestamp->setText(QString::fromStdString(std::to_string(now)));
    ui->edit_localtime->setText(QString::fromStdString(timestamp2LocaltimeStr(now)));
    ui->edit_utctime->setText(QString::fromStdString(timestamp2UtctimeStr(now)));

    // 更新当前时间事件
    connect(ui->btn_update_time, &QPushButton::clicked, this, [this](){
        ui->label_info_timestamp->setText("");
        std::time_t now = std::time(nullptr);
        ui->edit_timestamp->setText(QString::fromStdString(std::to_string(now)));
        ui->edit_localtime->setText(QString::fromStdString(timestamp2LocaltimeStr(now)));
        ui->edit_utctime->setText(QString::fromStdString(timestamp2UtctimeStr(now)));
    });

    // 时间戳转换
    connect(ui->btn_timestamp, &QPushButton::clicked, this, [this](){
        std::string reqStr = ui->edit_timestamp->text().toStdString();
        if (!isValidTimestamp(reqStr)) {
            ui->label_info_timestamp->setText("输入格式不合法或超出范围");
            return;
        }
        ui->label_info_timestamp->setText("");
        // 获取时间戳
        std::time_t timestamp = static_cast<std::time_t>(std::stoll(reqStr));
        // 转换成本地时间
        ui->edit_localtime->setText(QString::fromStdString(timestamp2LocaltimeStr(timestamp)));
        // 转换成UTC时间
        ui->edit_utctime->setText(QString::fromStdString(timestamp2UtctimeStr(timestamp)));
    });

    // 本地时间转换
    connect(ui->btn_localtime, &QPushButton::clicked, this, [this](){
        std::string reqStr = ui->edit_localtime->text().toStdString();
        if (!isValidTimeStr(reqStr)) {
            ui->label_info_timestamp->setText("输入格式不合法或超出范围");
            return;
        }
        // 获取时间戳
        std::time_t timestamp = 0;
        if (!localtime2Timestamp(reqStr, timestamp)) {
            ui->label_info_timestamp->setText("输入超出范围");
            return;
        }
        ui->label_info_timestamp->setText("");
        // 直接显示
        ui->edit_timestamp->setText(QString::fromStdString(std::to_string(timestamp)));
        // 转换成UTC时间
        ui->edit_utctime->setText(QString::fromStdString(timestamp2UtctimeStr(timestamp)));
    });

    // UTC时间转换
    connect(ui->btn_utctime, &QPushButton::clicked, this, [this](){
        std::string reqStr = ui->edit_utctime->text().toStdString();
        if (!isValidTimeStr(reqStr)) {
            ui->label_info_timestamp->setText("输入格式不合法或超出范围");
            return;
        }
        // 获取时间戳
        std::time_t timestamp = 0;
        if (!utctime2Timestamp(reqStr, timestamp)) {
            ui->label_info_timestamp->setText("输入超出范围");
            return;
        }
        ui->label_info_timestamp->setText("");
        // 直接显示
        ui->edit_timestamp->setText(QString::fromStdString(std::to_string(timestamp)));
        // 转换成本地时间
        ui->edit_localtime->setText(QString::fromStdString(timestamp2LocaltimeStr(timestamp)));
    });

    // 重置按钮事件
    connect(ui->btn_reset_timestamp, &QPushButton::clicked, this, [this](){
        ui->edit_timestamp->setText("");
        ui->edit_localtime->setText("");
        ui->edit_utctime->setText("");
    });
}

TimestampConverterPage::~TimestampConverterPage()
{
    delete ui;
}
