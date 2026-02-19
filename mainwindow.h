#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <perry_common.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define APP_VERSION "0.0.4"
#define TO_QSTR(str) QString::fromStdString(str)
#define NUM_TO_QSTR(num) QString::number(num)

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    static void setEditTextNoEvent(QTextEdit* const edit, const QString& text);

    void handleEditChanged(BaseEnum base);
    void asciiConverterPage();
    void timestampConverterPage();
    void crcCalculatorPage();
    void regexTesterPage();
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
