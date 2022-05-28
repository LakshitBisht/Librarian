#ifndef FORGOT_PASSWORD_H
#define FORGOT_PASSWORD_H

#include <QDialog>
#include "database.h"

namespace Ui {
class Forgot_password;
}

class Forgot_password : public QDialog
{
    Q_OBJECT

public:
    explicit Forgot_password(QWidget *parent = nullptr);
    ~Forgot_password();

private slots:
    void on_pushButton_Submit_clicked();

    void on_checkBox_admin_stateChanged(int arg1);

    void on_pushButton_ChangePass_clicked();

    void on_lineEdit_username_textChanged(const QString &arg1);

    void on_lineEdit_NewPassword_textChanged(const QString &arg1);

    void on_lineEdit_ConfirmPassword_textChanged(const QString &arg1);

    void on_comboBox_question_currentIndexChanged(int index);

    void on_lineEdit_answer_textChanged(const QString &arg1);

private:
    Ui::Forgot_password *ui;

    //creating a datamember to store the checkbox value
        bool admin_check;
};

#endif // FORGOT_PASSWORD_H
