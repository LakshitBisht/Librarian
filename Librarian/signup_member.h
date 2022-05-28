#ifndef SIGNUP_MEMBER_H
#define SIGNUP_MEMBER_H

#include <QMainWindow>
#include "database.h"

namespace Ui {
class SignUp_Member;
}

class SignUp_Member : public QMainWindow
{
    Q_OBJECT

public:
    explicit SignUp_Member(QWidget *parent = nullptr);
    ~SignUp_Member();

private slots:
    void on_pushButton_memLogin_clicked();
    void on_pushButton_memSubmit_clicked();

    void on_lineEdit_fName_textChanged(const QString &arg1);

    void on_lineEdit_lName_textChanged(const QString &arg1);

    void on_comboBox_gender_currentIndexChanged(int index);

    void on_comboBox_profession_currentIndexChanged(int index);

    void on_lineEdit_memUID_textChanged(const QString &arg1);

    void on_lineEdit_memPassword_textChanged(const QString &arg1);

    void on_comboBox_question_currentIndexChanged(int index);

    void on_lineEdit_answer_textChanged(const QString &arg1);

    void on_lineEdit_phone_textChanged(const QString &arg1);

private:
    Ui::SignUp_Member *ui;
};

#endif // SIGNUP_MEMBER_H
