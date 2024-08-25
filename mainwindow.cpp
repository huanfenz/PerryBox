#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ascii_converter.h"
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QTextEdit>

void MainWindow::setEditTextNoEvent(QTextEdit* edit, const QString& text)
{
    edit->blockSignals(true);
    edit->setPlainText(text);
    edit->blockSignals(false);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ascii 输入框改变事件
    connect(ui->edit_ascii, &QTextEdit::textChanged, this, [&](){
        qDebug() << "edit_ascii changed";
        perry::AsciiConverter asciiConverter(ui->edit_ascii->toPlainText().toStdString(), perry::InputTypeEnum::ASCII);
        setEditTextNoEvent(ui->edit_hex, QString::fromStdString(asciiConverter.getHexStr()));
        setEditTextNoEvent(ui->edit_dec, QString::fromStdString(asciiConverter.getDecStr()));
    });

    // hex 输入框改变事件
    connect(ui->edit_hex, &QTextEdit::textChanged, this, [&](){
        qDebug() << "edit_hex changed";
        perry::AsciiConverter asciiConverter(ui->edit_hex->toPlainText().toStdString(), perry::InputTypeEnum::HEX);
        setEditTextNoEvent(ui->edit_ascii, QString::fromStdString(asciiConverter.getAsciiStr()));
        setEditTextNoEvent(ui->edit_dec, QString::fromStdString(asciiConverter.getDecStr()));
    });

    // dec 输入框改变事件
    connect(ui->edit_dec, &QTextEdit::textChanged, this, [&](){
        qDebug() << "edit_dec changed";
        perry::AsciiConverter asciiConverter(ui->edit_dec->toPlainText().toStdString(), perry::InputTypeEnum::DEC);
        setEditTextNoEvent(ui->edit_ascii, QString::fromStdString(asciiConverter.getAsciiStr()));
        setEditTextNoEvent(ui->edit_hex, QString::fromStdString(asciiConverter.getHexStr()));
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
