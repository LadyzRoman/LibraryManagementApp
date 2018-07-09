#ifndef BOOKPAGE_H
#define BOOKPAGE_H

#include <QWidget>
#include <QCompleter>
#include <QDataWidgetMapper>
#include "database.h"
#include "booktablemodel.h"
#include "bookproxymodel.h"
#include "readertablemodel.h"
#include "readerproxymodel.h"

namespace Ui {
class BookPage;
}

class BookPage : public QWidget
{
    Q_OBJECT

public:
    BookPage(DataBase * db, BookTableModel * bookTableModel, ReaderTableModel * readerTableModel, QWidget * parent = 0);
    ~BookPage();


private:
    void initModel();
    void initConnection();
    void initUI();
    void setReaderPrepared(bool prepared);

public slots:
    void newBook();

    void editBook();

    void deleteBook();

    void currentReaderChanged(int id);

    void returnBook(int readerId, int bookId);

signals:
    void tableSelectionStatusChanged(bool status);
    void readerChanged();

private slots:
    void on_findButton_clicked();

    void on_resetButton_clicked();

    void on_borrowBookButton_clicked();

    void on_returnBookButton_clicked();

    void on_clearFilterButton_clicked();

    void on_idFindLineEdit_textEdited(const QString &id);

    void on_lastNameFindLineEdit_textEdited(const QString &lastName);

    void on_firstNameFindLineEdit_textEdited(const QString &firstName);

    void bookTableSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected);

    void firstNameCompleterActivated(QString firstName);

    void lastNameCompleterActivated(QString lastName);


private:
    Ui::BookPage *ui;

    DataBase * db;

    BookTableModel *bookTableModel;
    BookProxyModel * filteredBookModel;

    ReaderTableModel *readerTableModel;

    ReaderProxyModel * currentReaderModel;
    QDataWidgetMapper * currentReaderMapper;
    QCompleter * lastNameCompleter;
    QCompleter * firstNameCompleter;

    bool borrowed;
    bool readerPrepared;
};

#endif // BOOKPAGE_H
