#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "perry_utils.h"
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ascii 输入框改变事件
    connect(ui->edit_ascii, &QTextEdit::textChanged, this, [=](){
        // 得到数值数组
        QString asciiText = ui->edit_ascii->toPlainText();
        std::vector<uint8_t> numVct = perry::ascii2Nums(asciiText.toStdString());
        // 转成对应的进制的字符串塞进edit text
        QString strHex = QString::fromStdString(perry::nums2StrByBase(numVct, perry::BaseEnum::BASE_HEX));
        ui->edit_hex->setPlainText(strHex);

        QString strDec = QString::fromStdString(perry::nums2StrByBase(numVct, perry::BaseEnum::BASE_DEC));
        ui->edit_dec->setPlainText(strDec);

        QString strBin = QString::fromStdString(perry::nums2StrByBase(numVct, perry::BaseEnum::BASE_BIN));
        ui->edit_bin->setPlainText(strBin);
    });

    // 重置按钮事件
    connect(ui->btn_reset, &QPushButton::clicked, this, [=](){
        ui->edit_ascii->setPlainText("");
        ui->edit_hex->setPlainText("");
        ui->edit_dec->setPlainText("");
        ui->edit_bin->setPlainText("");
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
