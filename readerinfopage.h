#ifndef READERINFOPAGE_H
#define READERINFOPAGE_H

#include <QWidget>

namespace Ui {
class ReaderInfoPage;
}

class ReaderInfoPage : public QWidget
{
    Q_OBJECT

public:
    explicit ReaderInfoPage(QWidget *parent = 0);
    ~ReaderInfoPage();

private:
    Ui::ReaderInfoPage *ui;
};

#endif // READERINFOPAGE_H
