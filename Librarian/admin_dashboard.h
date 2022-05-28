#ifndef ADMIN_DASHBOARD_H
#define ADMIN_DASHBOARD_H

#include <QMainWindow>
#include "admin_accountdetails.h"
#include "memberadmindetails.h"
#include "bookdetails.h"
#include "issuedbooks.h"
#include "admin_messages.h"
#include "adminapplication.h"
#include "registrationlog.h"


namespace Ui {
class Admin_Dashboard;
}

class Admin_Dashboard : public QMainWindow
{
    Q_OBJECT

public:
    explicit Admin_Dashboard(QString user, QWidget *parent = nullptr);
    ~Admin_Dashboard();

private slots:
    void on_pushButton_logout_clicked();

    void on_pushButton_accountDetails_clicked();

    void on_pushButton_memberAdminDetails_clicked();

    void on_pushButton_bookDetails_clicked();

    void on_pushButton_isuueDetails_clicked();

    void on_pushButton_mssg_clicked();

    void on_toolButton_application_clicked();

    void on_toolButton_log_clicked();

private:
    Ui::Admin_Dashboard *ui;

    //creating widgets for stacked widget
    Admin_AccountDetails *accountDetailsWidget;
    MemberAdminDetails *memberAdminDetailsWidget;
    BookDetails *bookDetailsWidget;
    IssuedBooks *issuedBooksWidget;
    Admin_Messages *adminMessagesWidget;
    AdminApplication *adminApplicationWidget;
    RegistrationLog *registrationLogWidget;

    QSqlQueryModel *model;
    QString user;
};

#endif // ADMIN_DASHBOARD_H
