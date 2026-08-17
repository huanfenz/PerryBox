#ifndef REGEX_TESTER_PAGE_H
#define REGEX_TESTER_PAGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class RegexTesterPage; }
QT_END_NAMESPACE

class RegexTesterPage : public QWidget
{
    Q_OBJECT

public:
    explicit RegexTesterPage(QWidget *parent = nullptr);
    ~RegexTesterPage();

private:
    Ui::RegexTesterPage *ui;
};

#endif // REGEX_TESTER_PAGE_H
