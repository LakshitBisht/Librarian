#ifndef ADDBOOKS_H
#define ADDBOOKS_H

#include <QDialog>

namespace Ui {
class AddBooks;
}

class AddBooks : public QDialog
{
    Q_OBJECT

public:
    explicit AddBooks(QWidget *parent = nullptr, QString *bookDetails = nullptr);
    ~AddBooks();

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_update_clicked();

    void on_lineEdit_bookName_textChanged(const QString &arg1);

    void on_lineEdit_authorName_textChanged(const QString &arg1);

    void on_lineEdit_publication_textChanged(const QString &arg1);

    void on_lineEdit_genre_textChanged(const QString &arg1);

    void on_lineEdit_bookID_textChanged(const QString &arg1);

private:
    Ui::AddBooks *ui;
};

#endif // ADDBOOKS_H
