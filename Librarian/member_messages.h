#ifndef MEMBER_MESSAGES_H
#define MEMBER_MESSAGES_H

#include <QWidget>
#include "database.h"

namespace Ui {
class Member_Messages;
}

class Member_Messages : public QWidget
{
    Q_OBJECT

public:
    explicit Member_Messages(QString user, QToolButton *iconbutton = nullptr, QWidget *parent = nullptr);
    ~Member_Messages();

    void updateTable();

private slots:
    void on_pushButton_view_clicked();

    void on_pushButton_delete_clicked();

    void on_toolButton_back_clicked();

    void on_comboBox_search_currentIndexChanged(int index);

    void on_lineEdit_searchBar_textChanged(const QString &arg1);

private:
    Ui::Member_Messages *ui;

    QSqlQueryModel *model;
    QSortFilterProxyModel *filter_proxy_model;

    QToolButton *iconbutton;

    QString user;
};

#endif // MEMBER_MESSAGES_H
