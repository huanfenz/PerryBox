#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define APP_VERSION "0.0.2"
#define TO_QSTR(str) QString::fromStdString(str)

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    static void setEditTextNoEvent(QTextEdit* const edit, const QString& text);

    void asciiConverterPage();
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
