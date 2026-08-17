#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ascii_converter_page.h"
#include "timestamp_converter_page.h"
#include "crc_calculator_page.h"
#include "regex_tester_page.h"
#include <QAction>
#include <QIcon>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/Image/BirdIcon4.png"));

    // 各功能页面
    ui->tabWidget->addTab(new AsciiConverterPage(this), tr("ASCII转换"));
    ui->tabWidget->addTab(new TimestampConverterPage(this), tr("时间戳转换"));
    ui->tabWidget->addTab(new CrcCalculatorPage(this), tr("CRC校验"));
    ui->tabWidget->addTab(new RegexTesterPage(this), tr("正则测试"));

    // 版本信息
    connect(ui->actionUpdateInfo, &QAction::triggered, this, [this](){
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
}

MainWindow::~MainWindow()
{
    delete ui;
}
