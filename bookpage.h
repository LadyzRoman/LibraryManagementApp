#ifndef BOOKPAGE_H
#define BOOKPAGE_H

#include <QWidget>

namespace Ui {
class BookPage;
}

class BookPage : public QWidget
{
    Q_OBJECT

public:
    explicit BookPage(QWidget *parent = 0);
    ~BookPage();

private:
    Ui::BookPage *ui;
};

#endif // BOOKPAGE_H
