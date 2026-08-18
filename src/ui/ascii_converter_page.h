#ifndef ASCII_CONVERTER_PAGE_H
#define ASCII_CONVERTER_PAGE_H

#include <QWidget>
#include <QTextEdit>
#include <perry_common.h>

QT_BEGIN_NAMESPACE
namespace Ui { class AsciiConverterPage; }
QT_END_NAMESPACE

class AsciiConverterPage : public QWidget
{
    Q_OBJECT

public:
    explicit AsciiConverterPage(QWidget *parent = nullptr);
    ~AsciiConverterPage();

private:
    static void setEditTextNoEvent(QTextEdit* const edit, const QString& text);

    void handleEditChanged(BaseEnum base);
    Ui::AsciiConverterPage *ui;
};

#endif // ASCII_CONVERTER_PAGE_H
