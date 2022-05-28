#ifndef SIGNUP_ADMIN_H
#define SIGNUP_ADMIN_H

#include <QMainWindow>
#include "database.h"

namespace Ui {
class SignUp_Admin;
}

class SignUp_Admin : public QMainWindow
{
    Q_OBJECT

public:
    explicit SignUp_Admin(QWidget *parent = nullptr);
    ~SignUp_Admin();

private slots:
    void on_pushButton_adLogin_clicked();

    void on_pushButton_submit_clicked();

    void on_lineEdit_fName_textChanged(const QString &arg1);

    void on_lineEdit_lName_textChanged(const QString &arg1);

    void on_comboBox_gender_currentIndexChanged(int index);

    void on_comboBox_profession_currentIndexChanged(int index);

    void on_lineEdit_username_textChanged(const QString &arg1);

    void on_lineEdit_password_textChanged(const QString &arg1);

    void on_comboBox_question_currentIndexChanged(int index);

    void on_lineEdit_answer_textChanged(const QString &arg1);

    void on_lineEdit_phone_textChanged(const QString &arg1);

private:
    Ui::SignUp_Admin *ui;
};

#endif // SIGNUP_ADMIN_H
