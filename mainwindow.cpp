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
        qDebug() << "1111";
        // 得到数值数组
        QString asciiText = ui->edit_ascii->toPlainText();
        std::vector<uint8_t> numVct = perry::ascii2Nums(asciiText.toStdString());

        // 转换成十六进制字符串写入十六进制输入框
        QString strHex = QString::fromStdString(perry::nums2StrByBase(numVct, perry::BaseEnum::BASE_HEX));
        ui->edit_hex->blockSignals(true);
        ui->edit_hex->setPlainText(strHex);
        ui->edit_hex->blockSignals(false);

        // 转换成十进制写入十进制输入框
        QString strDec = QString::fromStdString(perry::nums2StrByBase(numVct, perry::BaseEnum::BASE_DEC));
        ui->edit_dec->blockSignals(true);
        ui->edit_dec->setPlainText(strDec);
        ui->edit_dec->blockSignals(false);
    });

    // hex 输入框改变事件
    connect(ui->edit_hex, &QTextEdit::textChanged, this, [=](){
        qDebug() << "2222";
        // 得到数值数组
        QString hexText = ui->edit_hex->toPlainText();
        std::vector<uint8_t> numVct = perry::hexsStr2Nums(hexText.toStdString());

        // 转换成十进制写入十进制输入框
        QString strDec = QString::fromStdString(perry::nums2StrByBase(numVct, perry::BaseEnum::BASE_DEC));
        ui->edit_dec->blockSignals(true);
        ui->edit_dec->setPlainText(strDec);
        ui->edit_dec->blockSignals(false);

        // 转换成ASCII写入ASCII输入框
        QString str = QString::fromStdString(perry::nums2Ascii(numVct));
        ui->edit_ascii->blockSignals(true);
        ui->edit_ascii->setPlainText(str);
        ui->edit_ascii->blockSignals(false);
    });

    // dec 输入框改变事件
    connect(ui->edit_dec, &QTextEdit::textChanged, this, [=](){
        qDebug() << "3333";
        // 得到数值数组
        QString decText = ui->edit_dec->toPlainText();
        std::vector<uint8_t> numVct = perry::decsStr2Nums(decText.toStdString());

        // 转换成十六进制写入十进制输入框
        QString strHex = QString::fromStdString(perry::nums2StrByBase(numVct, perry::BaseEnum::BASE_HEX));
        ui->edit_hex->blockSignals(true);
        ui->edit_hex->setPlainText(strHex);
        ui->edit_hex->blockSignals(false);

        // 转换成ASCII写入ASCII输入框
        QString str = QString::fromStdString(perry::nums2Ascii(numVct));
        ui->edit_ascii->blockSignals(true);
        ui->edit_ascii->setPlainText(str);
        ui->edit_ascii->blockSignals(false);
    });

    // 重置按钮事件
    connect(ui->btn_reset, &QPushButton::clicked, this, [=](){
        ui->edit_ascii->setPlainText("");
        ui->edit_hex->setPlainText("");
        ui->edit_dec->setPlainText("");
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
