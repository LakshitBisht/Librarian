#ifndef ISSUEDBOOKS_H
#define ISSUEDBOOKS_H

#include <QWidget>
#include "database.h"

namespace Ui {
class IssuedBooks;
}

class IssuedBooks : public QWidget
{
    Q_OBJECT

public:
    explicit IssuedBooks(QWidget *parent = nullptr);
    ~IssuedBooks();

    bool deleteData(QString bookid);

private slots:
    void on_pushButton_returnBook_clicked();

    void on_pushButton_requests_clicked();

    void on_toolButton_back_clicked();

    void on_pushButton_accept_clicked();

    void on_pushButton_decline_clicked();

    void on_comboBox_search_currentIndexChanged(int index);

    void on_comboBox_search_2_currentIndexChanged(int index);

    void on_lineEdit_searchBar_textChanged(const QString &arg1);

    void on_lineEdit_searchBar_2_textChanged(const QString &arg1);

private:
    Ui::IssuedBooks *ui;

    QSqlQueryModel *model;
    QSortFilterProxyModel *filter_proxy_model;
    QSortFilterProxyModel *filter_proxy_model_2;
};

#endif // ISSUEDBOOKS_H
