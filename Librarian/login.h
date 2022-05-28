#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include "signup_member.h"
#include "signup_admin.h"
#include "forgot_password.h"
#include "admin_dashboard.h"
#include "member_dashboard.h"
#include "database.h"


namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(QLabel *statusLabel = nullptr, QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_checkBox_admin_stateChanged(int arg1);

    void on_pushButton_forgot_clicked();

    void on_pushButton_signup_clicked();

    void on_pushButton_login_clicked();

    void on_lineEdit_username_textChanged(const QString &arg1);

    void on_lineEdit_password_textChanged(const QString &arg1);

private:
    Ui::Login *ui;

    //creating pointers to objects of the follwing windows
        SignUp_Member *member_window;
        SignUp_Admin *admin_window;
        Admin_Dashboard *adminDashboard_window;
        Member_Dashboard *memberDashboard_window;
};

#endif // LOGIN_H
