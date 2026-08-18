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
        QMessageBox::information(nullptr, "版本信息",
            QString("当前版本：%1，Created By Perry.<br>"
                    "GitHub: <a href=\"https://github.com/huanfenz/PerryBox\">https://github.com/huanfenz/PerryBox</a>")
                .arg(APP_VERSION));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
