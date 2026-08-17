#ifndef TIMESTAMP_CONVERTER_PAGE_H
#define TIMESTAMP_CONVERTER_PAGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class TimestampConverterPage; }
QT_END_NAMESPACE

class TimestampConverterPage : public QWidget
{
    Q_OBJECT

public:
    explicit TimestampConverterPage(QWidget *parent = nullptr);
    ~TimestampConverterPage();

private:
    Ui::TimestampConverterPage *ui;
};

#endif // TIMESTAMP_CONVERTER_PAGE_H
