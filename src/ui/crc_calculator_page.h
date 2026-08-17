#ifndef CRC_CALCULATOR_PAGE_H
#define CRC_CALCULATOR_PAGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class CrcCalculatorPage; }
QT_END_NAMESPACE

class CrcCalculatorPage : public QWidget
{
    Q_OBJECT

public:
    explicit CrcCalculatorPage(QWidget *parent = nullptr);
    ~CrcCalculatorPage();

private:
    Ui::CrcCalculatorPage *ui;
};

#endif // CRC_CALCULATOR_PAGE_H
