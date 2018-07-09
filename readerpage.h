#ifndef READERPAGE_H
#define READERPAGE_H

#include <QWidget>

namespace Ui {
class ReaderPage;
}

class ReaderPage : public QWidget
{
    Q_OBJECT

public:
    explicit ReaderPage(QWidget *parent = 0);
    ~ReaderPage();

private:
    Ui::ReaderPage *ui;
};

#endif // READERPAGE_H
