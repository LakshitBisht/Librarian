#ifndef MEMBERADMINDETAILS_H
#define MEMBERADMINDETAILS_H

#include <QWidget>
#include "database.h"

namespace Ui {
class MemberAdminDetails;
}

class MemberAdminDetails : public QWidget
{
    Q_OBJECT

public:
    explicit MemberAdminDetails(QWidget *parent = nullptr);
    ~MemberAdminDetails();

    void connectTableDatabase(QString tableName);

private slots:
    void on_pushButton_removeMem_clicked();

    void on_comboBox_search_currentIndexChanged(int index);

    void on_comboBox_search_2_currentIndexChanged(int index);

    void on_lineEdit_searchBar_textChanged(const QString &arg1);

    void on_lineEdit_searchBar_2_textChanged(const QString &arg1);

private:
    Ui::MemberAdminDetails *ui;

    QSqlQueryModel * model;
    QSortFilterProxyModel *filter_proxy_model;
    QSortFilterProxyModel *filter_proxy_model_2;
};

#endif // MEMBERADMINDETAILS_H
