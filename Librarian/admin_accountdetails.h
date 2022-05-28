#ifndef ADMIN_ACCOUNTDETAILS_H
#define ADMIN_ACCOUNTDETAILS_H

#include <QWidget>
#include "database.h"

namespace Ui {
class Admin_AccountDetails;
}

class Admin_AccountDetails : public QWidget
{
    Q_OBJECT

public:
    explicit Admin_AccountDetails(QString account, QString user, QLabel* dashlabelName = nullptr, QLabel* dashlabelUser = nullptr,  QWidget *parent = nullptr);
    ~Admin_AccountDetails();

private slots:
    void on_pushButton_changePass_clicked();

    void on_pushButton_updateDetails_clicked();

    void on_pushButton_confirmChange_clicked();

    void on_pushButton_changeCancel_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_deleteAccount_clicked();

    void on_lineEdit_fname_textChanged(const QString &arg1);

    void on_lineEdit_lname_textChanged(const QString &arg1);

    void on_lineEdit_username_textChanged(const QString &arg1);

    void on_lineEdit_contact_textChanged(const QString &arg1);

    void on_lineEdit_newPass_textChanged(const QString &arg1);

    void on_lineEdit_confirmPass_textChanged(const QString &arg1);

private:
    Ui::Admin_AccountDetails *ui;

    QLabel *dashlabelName;
    QLabel *dashlabelUser;

    QString account;
    QString user;
    QString id;
};

#endif // ADMIN_ACCOUNTDETAILS_H
