#include "forgot_password.h"
#include "ui_forgot_password.h"

Forgot_password::Forgot_password(QWidget *parent) : QDialog(parent),
                                                    ui(new Ui::Forgot_password)
{
    ui->setupUi(this);

//hiding new password field, confirm password field and change password button
    ui->lineEdit_NewPassword->hide();
    ui->lineEdit_ConfirmPassword->hide();
    ui->pushButton_ChangePass->hide();
    ui->label_passStrength->hide();

//creating signal and slot for cancel button
    connect(ui->pushButton_Cancel, SIGNAL(clicked()), this, SLOT(close()));

//adding questions in combo-box
    ui->comboBox_question->addItem("What is your birth year?");
    ui->comboBox_question->addItem("Which city do you live in?");
    ui->comboBox_question->addItem("What is your pet name?");
    ui->comboBox_question->addItem("What is your favourite Book?");
    ui->comboBox_question->addItem("Who is your favourite Author?");
}

Forgot_password::~Forgot_password()
{
    delete ui;
}

void Forgot_password::on_pushButton_Submit_clicked()
{
//taking in values from the input fields
    QString fUID, fQuestion, fAnswer;
    fUID = ui->lineEdit_username->text();
    fQuestion = ui->comboBox_question->currentText();
    fAnswer = ui->lineEdit_answer->text();

    //encrypting the password
                    QByteArray encryptedAnswer = fAnswer.toUtf8();
                    encryptedAnswer = QCryptographicHash::hash(encryptedAnswer, QCryptographicHash::Md5);

//imposing constraints on the input fields and displaying warning messages accordingly
//if both UID/Username and Password fields are empty
    if (fUID == "" && fQuestion == "" && fAnswer == "")
    {
        QMessageBox::warning(this, "Warning!", "All Fields are Mandatory! ");
    }

//if UID/Username field is empty
    else if (fUID == "")
    {
    //if admin checkbox is checked
        if (ui->checkBox_admin->isChecked())
            QMessageBox::warning(this, "Warning!", "Username Field cannot be Empty! ");

    //if checkbox is unchecked
        else
            QMessageBox::warning(this, "Warning!", "UID Field cannot be Empty! ");
    }

//if question is not selected
    else if (fQuestion == "")
    {
        QMessageBox::warning(this, "Warning!", "Please Select a Question ");
    }

//if answer field is empty
    else if (fAnswer == "")
    {
        QMessageBox::warning(this, "Warning!", "Answer Field cannot be Empty! ");
    }

//if database is not connected
    else if (!Database::connectDB())
    {
        QMessageBox::warning(this, "Warning!", "Database not Connected! ");
    }

//if all the conditions are satisfied
    else
    {
    //opening the database connection
        Database::connectDB();

    //creating query object
        QSqlQuery query;

    //if admin checkbox is checked
        if (ui->checkBox_admin->isChecked())
        {
        //preparing the query to check if entered Username,question and answer are present in Admins table
            query.prepare("select * from Admins where Username = '" + fUID + "' and Security_Ques = '" + fQuestion + "' and Answer = '" + encryptedAnswer + "'");

        //executing the query and checking if query executed successfully
        //if query executed
            if (query.exec())
            {
            //checking if Admin exists
                bool var = false;
                if (query.next())
                    var = true;

            //if match found
                if (var)
                {
                //displaying system message
                    QMessageBox::about(this, "Match Found!", "Proceed to Change Password ");

                //closing the database connection
                    Database::closeDB();

                //storing the state of admin checkbox
                    admin_check = ui->checkBox_admin->isChecked();

                //closing pre-existing widgets
                    ui->comboBox_question->close();
                    ui->lineEdit_username->setReadOnly(true);
                    ui->lineEdit_answer->close();
                    ui->pushButton_Submit->close();
                    ui->checkBox_admin->close();

                //show new password field, confirm password field and change password button
                    ui->lineEdit_NewPassword->show();
                    ui->lineEdit_ConfirmPassword->show();
                    ui->pushButton_ChangePass->show();
                }

            //if match not found
                else
                {
                //displaying system warning
                    QMessageBox::warning(this, "Warning!", "Match not Found. Wrong Credentials ");
                }
            }

        //if query not executed
            else
            {
            //displaying the critical message dialog with the corresponding error
                QMessageBox::critical(this, "Error!", query.lastError().text());
            }
        }

    //if checkbox is unchecked
        else
        {
        //preparing the query to check if entered UID,question and answer are present in Members table
            query.prepare("select * from Members where UID = '" + fUID + "' and Security_Ques = '" + fQuestion + "' and Answer = '" + encryptedAnswer + "'");

        //executing the query and checking if query executed successfully
        //if query executed
            if (query.exec())
            {
            //checking if member exists
                bool var = false;
                if (query.next())
                    var = true;

            //if match found
                if (var)
                {
                //displaying system message
                    QMessageBox::about(this, "Match Found!", "Proceed to Change Password ");

                //closing the database connection
                    Database::closeDB();

                //storing the state of checkbox
                    admin_check = ui->checkBox_admin->isChecked();

                //closing preexisting widgets
                    ui->comboBox_question->close();
                    ui->lineEdit_username->setReadOnly(true);
                    ui->lineEdit_answer->close();
                    ui->pushButton_Submit->close();
                    ui->checkBox_admin->close();

                //show new password field, confirm password field and change password button
                    ui->lineEdit_NewPassword->show();
                    ui->lineEdit_ConfirmPassword->show();
                    ui->pushButton_ChangePass->show();
                }

            //if match not found
                else
                {
                //displaying system warning
                    QMessageBox::warning(this, "Warning", "Match not Found. Wrong Credentials ");
                }
            }

        //if query not executed
            else
            {
            //displaying the critical message dialog with the corresponding error
                QMessageBox::critical(this, "Error!", query.lastError().text());
            }
        }
    }
}

void Forgot_password::on_checkBox_admin_stateChanged(int arg1)
{
//resetting the line edits and combobox
    ui->lineEdit_username->setText("");
    ui->comboBox_question->setCurrentIndex(-1);
    ui->lineEdit_answer->setText("");

//if Admin checkbox is checked
    if (arg1 == 2)
    {
    //changing the input field to Username
        ui->lineEdit_username->setPlaceholderText("Username");
    }

//if checkbox is unchecked
    else
    {
    //changing the input field to UID
        ui->lineEdit_username->setPlaceholderText("UID");
    }
}

void Forgot_password::on_pushButton_ChangePass_clicked()
{
//taking in values from the input fields
    QString uid, new_pass, confirm_pass;
    uid = ui->lineEdit_username->text();
    new_pass = ui->lineEdit_NewPassword->text();
    confirm_pass = ui->lineEdit_ConfirmPassword->text();

    //encrypting the password
                    QByteArray encryptedPassword = new_pass.toUtf8();
                    encryptedPassword = QCryptographicHash::hash(encryptedPassword, QCryptographicHash::Md5);

//imposing constraints on the input fields and displaying warning messages accordingly
//if new password field is empty
    if (new_pass == "")
        QMessageBox::warning(this, "Warning", "Please Enter the New Password! ");

//if confirm password field is empty
    else if (confirm_pass == "")
        QMessageBox::warning(this, "Warning", "Please Confirm the Password!");

    //if password is nt Strong
        else if(ui->label_passStrength->text() != "Password is Strong!")
            QMessageBox::warning(this, "Warning", "Password needs to be Strong! ");

//if confirm new password and confirm password do not match
    else if (new_pass != confirm_pass)
        QMessageBox::warning(this, "Warning", "Password Not Matched!");

//if database is not connected
    else if (!Database::connectDB())
        QMessageBox::warning(this, "Warning!", "Database not Connected! ");

//if all the conditions are satisfied
    else
    {
    //opening the connection to the database
        Database::connectDB();

    //creating a query object
        QSqlQuery query;

    //if admin checkbax was clicked
        if (admin_check)
        {
        //preparing the query for updating the password in Admins table
            query.prepare("update Admins set Password = '" + encryptedPassword + "' where Username = '" + uid + "'");

        //executing the query and checking if query executed successfully
        //if query executed
            if (query.exec())
            {
            //displaying system message
                QMessageBox::about(this, "Admin Account Updated!", "Password changed Successfully! ");

            //closing the database connection
                Database::closeDB();

            //closing the forget password dialog
                this->close();
            }

        //if query not executed
            else
            {
            //displaying critical system message with the corresponding error
                QMessageBox::critical(this, "Error", query.lastError().text());
            }
        }

    //if checkbox not checked
        else
        {
        //preparing the query for updating the password in Members table
            query.prepare("update Members set Password = '" + encryptedPassword + "' where UID = '" + uid + "'");

        //executing the query and checking if query executed successfully
        //if query executed
            if (query.exec())
            {
            //displaying system message
                QMessageBox::about(this, "Member Account Updated!", "Password changed Successfully! ");

            //closing database connection
                Database::closeDB();

            //closing the forget password dialog
                this->close();
            }

        //if query not executed
            else
            {
            //displaying critical system message with the corresponding error
                QMessageBox::critical(this, "Error", query.lastError().text());
            }
        }
    }
}

void Forgot_password::on_lineEdit_username_textChanged(const QString &arg1)
{
    if(arg1 == "")
        ui->lineEdit_username->setStyleSheet("color: #717072;");
    else
        ui->lineEdit_username->setStyleSheet("color:white;");
}


void Forgot_password::on_lineEdit_NewPassword_textChanged(const QString &arg1)
{
    if(arg1 == "")
        {
            ui->lineEdit_NewPassword->setStyleSheet("color: #717072;");
            ui->label_passStrength->hide();
        }
        else
        {
            ui->lineEdit_NewPassword->setStyleSheet("color:white;");
            ui->label_passStrength->show();
        }
        int n = arg1.length();

           bool hasLower = false, hasUpper = false, hasDigit = false, hasspecialChar = false;


           for (int i = 0; i < n; i++)
           {
               if (arg1[i] >='a' && arg1[i] <='z')
                   hasLower = true;
               else if (arg1[i] >='A' && arg1[i] <='Z')
                   hasUpper = true;
               else if (arg1[i] >='0' && arg1[i] <='9')
                   hasDigit = true;
               else
                   hasspecialChar = true;
           }

           // Strength of password
           if (hasLower && hasUpper && hasDigit && hasspecialChar && (n >= 8))
           {
               ui->label_passStrength->setText("Password is Strong!");
               ui->label_passStrength->setStyleSheet("color:green;");
           }
           else if ((hasLower || hasUpper) && hasspecialChar && (n >= 6))
           {
               ui->label_passStrength->setText("Password is Moderate!");
               ui->label_passStrength->setStyleSheet("color:yellow;");
           }
           else
           {
               ui->label_passStrength->setText("Password is Weak!");
               ui->label_passStrength->setStyleSheet("color:red;");
           }
}


void Forgot_password::on_lineEdit_ConfirmPassword_textChanged(const QString &arg1)
{
    if(arg1 == "")
        ui->lineEdit_ConfirmPassword->setStyleSheet("color: #717072;");
    else
        ui->lineEdit_ConfirmPassword->setStyleSheet("color:white;");
}


void Forgot_password::on_comboBox_question_currentIndexChanged(int index)
{
    if(index == -1)
        ui->comboBox_question->setStyleSheet("color: #717072;");
    else
        ui->comboBox_question->setStyleSheet("color:white;");
}


void Forgot_password::on_lineEdit_answer_textChanged(const QString &arg1)
{
    if(arg1 == "")
        ui->lineEdit_answer->setStyleSheet("color: #717072;");
    else
        ui->lineEdit_answer->setStyleSheet("color:white;");
}

