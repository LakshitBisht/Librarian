#ifndef MEMBER_DASHBOARD_H
#define MEMBER_DASHBOARD_H

#include <QMainWindow>
#include "database.h"
#include "admin_accountdetails.h"
#include "bookdetails.h"
#include "registrationlog.h"
#include "member_messages.h"


namespace Ui {
class Member_Dashboard;
}

class Member_Dashboard : public QMainWindow
{
    Q_OBJECT

public:
    explicit Member_Dashboard(QString user, QWidget *parent = nullptr);
    ~Member_Dashboard();

private slots:
    void on_pushButton_Logout_clicked();

    void on_pushButton_AccountDetails_clicked();

    void on_pushButton_allBooks_clicked();

    void on_pushButton_currentlyIssued_clicked();

    void on_toolButton_messages_clicked();

    void on_toolButton_issueLog_clicked();

private:
    Ui::Member_Dashboard *ui;

    Admin_AccountDetails *accountDetailsWidget;
    BookDetails *bookDetailsWidget;
    RegistrationLog *registrationLogWidget;
    Member_Messages *memberMessagesWidget;

    QSqlQueryModel *model;
    QString user;
};

#endif // MEMBER_DASHBOARD_H
