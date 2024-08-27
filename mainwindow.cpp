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

    // 获取ASCII转换器单例对象
    perry::AsciiConverter& asciiConverter = perry::AsciiConverter::getInstance();

    // ascii 输入框改变事件
    connect(ui->edit_ascii, &QTextEdit::textChanged, this, [&](){
        qDebug() << "edit_ascii changed";
        asciiConverter.setAsciiStr(ui->edit_ascii->toPlainText().toStdString());
        setEditTextNoEvent(ui->edit_hex, QString::fromStdString(asciiConverter.getHexStr()));
        setEditTextNoEvent(ui->edit_dec, QString::fromStdString(asciiConverter.getDecStr()));
    });

    // hex 输入框改变事件
    connect(ui->edit_hex, &QTextEdit::textChanged, this, [&](){
        qDebug() << "edit_hex changed";
        asciiConverter.setHexStr(ui->edit_hex->toPlainText().toStdString());
        setEditTextNoEvent(ui->edit_ascii, QString::fromStdString(asciiConverter.getAsciiStr()));
        setEditTextNoEvent(ui->edit_dec, QString::fromStdString(asciiConverter.getDecStr()));
    });

    // dec 输入框改变事件
    connect(ui->edit_dec, &QTextEdit::textChanged, this, [&](){
        qDebug() << "edit_dec changed";
        asciiConverter.setDecStr(ui->edit_dec->toPlainText().toStdString());
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
