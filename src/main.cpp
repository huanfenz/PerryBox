#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    /* 高分屏适配 */
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    // 2. 适配非整数倍缩放
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    // 3. 适配字体渲染 hinting
    QFont font;
    font.setStyleStrategy(QFont::PreferAntialias);
    font.setHintingPreference(QFont::PreferFullHinting);
    QApplication::setFont(font);

    /* Origin */
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
