#ifndef BOOKDETAILS_H
#define BOOKDETAILS_H

#include <QWidget>
#include "database.h"
#include "addbooks.h"

namespace Ui {
class BookDetails;
}

class BookDetails : public QWidget
{
    Q_OBJECT

public:
    explicit BookDetails(QString account, QString user, QWidget *parent = nullptr);
    ~BookDetails();

    void updateTable();

private slots:
    void on_pushButton_addBook_clicked();

    void on_pushButton_deleteBook_clicked();

    void on_pushButton_Edit_clicked();

    void on_pushButton_request_clicked();

    void on_comboBox_search_currentIndexChanged(int index);

    void on_lineEdit_searchBar_textChanged(const QString &arg1);

private:
    Ui::BookDetails *ui;

    QSqlQueryModel * model;
    QSortFilterProxyModel *filter_proxy_model;

    QString account;
    QString user;
};

#endif // BOOKDETAILS_H
