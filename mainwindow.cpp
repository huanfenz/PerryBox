#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ascii_converter.h"
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
    });

    // 重置按钮事件
    connect(ui->btn_reset, &QPushButton::clicked, this, [=](){
        ui->edit_ascii->setPlainText("");
        ui->edit_hex->setPlainText("");
        ui->edit_dec->setPlainText("");
        ui->label_info_ascii->setText("");
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
}

MainWindow::~MainWindow()
{
    delete ui;
}
