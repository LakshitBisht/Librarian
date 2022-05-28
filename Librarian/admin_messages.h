#ifndef ADMIN_MESSAGES_H
#define ADMIN_MESSAGES_H

#include <QWidget>
#include <database.h>

namespace Ui {
class Admin_Messages;
}

class Admin_Messages : public QWidget
{
    Q_OBJECT

public:
    explicit Admin_Messages(QString user, QWidget *parent = nullptr);
    ~Admin_Messages();

    void updateTable();

private slots:
    void on_pushButton_messageReset_clicked();

    void on_toolButton_sentMessg_clicked();

    void on_toolButton_back_clicked();

    void on_toolButton_sendMssg_clicked();

    void on_pushButton_messageSend_clicked();

    void on_pushButton_delete_clicked();

    void on_pushButton_view_clicked();

    void on_comboBox_search_currentIndexChanged(int index);

    void on_lineEdit_messageTo_textChanged(const QString &arg1);

    void on_lineEdit_messageSubject_textChanged(const QString &arg1);

    void on_lineEdit_searchBar_textChanged(const QString &arg1);

private:
    Ui::Admin_Messages *ui;

    QSqlQueryModel *model;
    QSortFilterProxyModel *filter_proxy_model;
    QString user;
};

#endif // ADMIN_MESSAGES_H
