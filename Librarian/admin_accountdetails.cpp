#include "admin_accountdetails.h"
#include "ui_admin_accountdetails.h"
#include "admin_dashboard.h"
#include "member_dashboard.h"

Admin_AccountDetails::Admin_AccountDetails(QString account, QString user, QLabel *dashlabelName, QLabel *dashlabelUser, QWidget *parent) : QWidget(parent),
                                                                                                                                           ui(new Ui::Admin_AccountDetails)
{
    ui->setupUi(this);

//Deleting the Form instance when window is closed
    this->setAttribute(Qt::WA_DeleteOnClose);

    this->user = user;
    this->account = account;

    this->dashlabelName = dashlabelName;
    this->dashlabelUser = dashlabelUser;

    //Restricting the input field for 'contact no.' lineedit to digits only
        ui->lineEdit_contact->setValidator(new QIntValidator(ui->lineEdit_contact));

        ui->label_passStrength->hide();

    id = "Username";
    if (account == "Members")
    {
        ui->label_username->setText("UID");
        id = "UID";
    }

//changing the page of the stacked widget
    ui->stackedWidget_AccountDetails->setCurrentIndex(0);

    ui->stackedWidget_button->setCurrentIndex(0);

//opening the connection to the database
    Database::connectDB();

//creating a query object
    QSqlQuery query;

//preparing the query for setting the line edits
    query.prepare("select First_Name, Last_Name, Contact_No from " + account + " where " + id + " = '" + user + "'");

//executing the query and checking if query executed successfully
//if query executed
    if (query.exec())
    {
    //extracting values from the query and setting it to the line edits
        query.next();
        ui->lineEdit_fname->setText(query.value(0).toString());
        ui->lineEdit_lname->setText(query.value(1).toString());
        ui->lineEdit_username->setText(user);
        ui->lineEdit_contact->setText(query.value(2).toString());

    //closing the database connection
        Database::closeDB();

    //making the line edits uneditable
        ui->lineEdit_fname->setReadOnly(true);
        ui->lineEdit_lname->setReadOnly(true);
        ui->lineEdit_username->setReadOnly(true);
        ui->lineEdit_contact->setReadOnly(true);
    }

//if query not executed
    else
    {
    //displaying critical system message with the corresponding error
        QMessageBox::critical(this, "Error", query.lastError().text());
    }
}

Admin_AccountDetails::~Admin_AccountDetails()
{
    delete ui;
}

void Admin_AccountDetails::on_pushButton_changePass_clicked()
{
//changing the page of the stacked widget
    ui->stackedWidget_AccountDetails->setCurrentIndex(1);
}

void Admin_AccountDetails::on_pushButton_updateDetails_clicked()
{
//taking in details from the line edits
    QString fname, lname, newUsername, contact;
    fname = ui->lineEdit_fname->text();
    lname = ui->lineEdit_lname->text();
    newUsername = ui->lineEdit_username->text();
    contact = ui->lineEdit_contact->text();

//imposing constraints on the input fields and displaying warning messages accordingly
//if first name field is empty
    if (fname == "")
        QMessageBox::warning(this, "Warning", "Please Enter your First Name! ");

//if last name field is empty
    else if (lname == "")
        QMessageBox::warning(this, "Warning", "Please Enter your Last Name!");

//if username field is empty
    else if (newUsername == "")
        QMessageBox::warning(this, "Warning", "Please Enter your " + id + "!");

//if contact field is empty
    else if (contact == "")
        QMessageBox::warning(this, "Warning", "Please Enter your Contact Number!");

//if contact no is less than 10 digits
    else if (contact.size() < 10)
        QMessageBox::warning(this, "Warning", "Contact Number cannot be less than 10 digits! ");

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

    //preparing the query for updating the password in Admins table
        query.prepare("update " + account + " set First_Name = '" + fname + "',First_Name = '" + fname + "',Last_Name = '" + lname + "'," + id + " = '" + newUsername + "',Contact_No = '" + contact + "'   where " + id + " = '" + user + "'");

    //executing the query and checking if query executed successfully
    //if query executed
        if (query.exec())
        {
        //displaying system message
            QMessageBox::about(this, account + " Account Updated!", "Account Updated Successfully! ");
        //closing the database connection
            Database::closeDB();

        //hiding the update details button and showing the update button
            ui->stackedWidget_button->setCurrentIndex(0);

            dashlabelName->setText(fname + " " + lname);
            dashlabelUser->setText(newUsername);

        //making the line edits uneditable
            ui->lineEdit_fname->setReadOnly(true);
            ui->lineEdit_lname->setReadOnly(true);
            ui->lineEdit_username->setReadOnly(true);
            ui->lineEdit_contact->setReadOnly(true);
        }

    //if query not executed
        else
        {
        //displaying critical system message with the corresponding error
            QMessageBox::critical(this, "Error", query.lastError().text());
        }
    }
}

void Admin_AccountDetails::on_pushButton_confirmChange_clicked()
{
//taking in new password and confirm password from the line edits
    QString new_pass, confirm_pass;
    new_pass = ui->lineEdit_newPass->text();
    confirm_pass = ui->lineEdit_confirmPass->text();

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

    //preparing the query for updating the password in Admins table
        query.prepare("update " + account + " set Password = '" + encryptedPassword + "' where " + id + " = '" + user + "'");

    //executing the query and checking if query executed successfully
    //if query executed
        if (query.exec())
        {
        //displaying system message
            QMessageBox::about(this, account + " Account Updated!", "Password changed Successfully, Please Login Again! ");

        //closing the database connection
            Database::closeDB();

            ((QStackedWidget *)parent()->parent()->parent()->parent())->setCurrentIndex(1);
            if (account == "Admin")
                ((Admin_Dashboard *)parent()->parent())->close();
            else
                ((Member_Dashboard *)parent()->parent())->close();
        }

    //if query not executed
        else
        {
        //displaying critical system message with the corresponding error
            QMessageBox::critical(this, "Error", query.lastError().text());
        }
    }
}

void Admin_AccountDetails::on_pushButton_changeCancel_clicked()
{
//resetting the line edits
    ui->lineEdit_newPass->setText("");
    ui->lineEdit_confirmPass->setText("");

//changing the page of the stacked widget
    ui->stackedWidget_AccountDetails->setCurrentIndex(0);
}

void Admin_AccountDetails::on_pushButton_update_clicked()
{
//making the line edits editable
    ui->lineEdit_fname->setReadOnly(false);
    ui->lineEdit_lname->setReadOnly(false);
    ui->lineEdit_username->setReadOnly(false);
    ui->lineEdit_contact->setReadOnly(false);

    ui->stackedWidget_button->setCurrentIndex(1);
}

void Admin_AccountDetails::on_pushButton_deleteAccount_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Remove Account", "Do You Want To Remove Account?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        Database::connectDB();

        QSqlQuery query;
        query.prepare("delete from " + account + " where " + id + " = '" + user + "'");
    //executing the query and checking if query executed successfully
    //if query executed
        if (query.exec())
        {
            QMessageBox::about(this, "Remove Account", "Account Removed Successfully!");
        //closing the database connection
            Database::closeDB();

            ((QStackedWidget *)parent()->parent()->parent()->parent())->setCurrentIndex(1);
            if (account == "Admin")
                ((Admin_Dashboard *)parent()->parent())->close();
            else
                ((Member_Dashboard *)parent()->parent())->close();
        }

    //if query not executed
        else
        {
        //displaying critical system message with the corresponding error
            QMessageBox::critical(this, "Error", query.lastError().text());
        }
    }
}

void Admin_AccountDetails::on_lineEdit_fname_textChanged(const QString &arg1)
{
    if(arg1 == "")
            ui->lineEdit_fname->setStyleSheet("color: #717072;");
        else
            ui->lineEdit_fname->setStyleSheet("color:white;");
}


void Admin_AccountDetails::on_lineEdit_lname_textChanged(const QString &arg1)
{
    if(arg1 == "")
            ui->lineEdit_lname->setStyleSheet("color: #717072;");
        else
            ui->lineEdit_lname->setStyleSheet("color:white;");
}


void Admin_AccountDetails::on_lineEdit_username_textChanged(const QString &arg1)
{
    if(arg1 == "")
            ui->lineEdit_username->setStyleSheet("color: #717072;");
        else
            ui->lineEdit_username->setStyleSheet("color:white;");
}


void Admin_AccountDetails::on_lineEdit_contact_textChanged(const QString &arg1)
{
    if(arg1 == "")
            ui->lineEdit_contact->setStyleSheet("color: #717072;");
        else
            ui->lineEdit_contact->setStyleSheet("color:white;");
}


void Admin_AccountDetails::on_lineEdit_newPass_textChanged(const QString &arg1)
{
    if(arg1 == "")
        {
            ui->lineEdit_newPass->setStyleSheet("color: #717072;");
            ui->label_passStrength->hide();
        }
        else
        {
            ui->lineEdit_newPass->setStyleSheet("color:white;");
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


void Admin_AccountDetails::on_lineEdit_confirmPass_textChanged(const QString &arg1)
{
    if(arg1 == "")
            ui->lineEdit_confirmPass->setStyleSheet("color: #717072;");
        else
            ui->lineEdit_confirmPass->setStyleSheet("color:white;");
}

