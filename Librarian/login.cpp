#include "login.h"
#include "ui_login.h"

Login::Login(QLabel *statusLabel, QWidget *parent) : QMainWindow(parent),
                                ui(new Ui::Login)
{
    ui->setupUi(this);

    statusLabel->setText(Database::check_statusDB());
//Deleting the Form instance when window is closed
    this->setAttribute(Qt::WA_DeleteOnClose);

    ui->stackedWidget_login->setCurrentIndex(0);
}

Login::~Login()
{
    delete ui;
}

void Login::on_checkBox_admin_stateChanged(int arg1)
{
//resetting the line edits
    ui->lineEdit_username->setText("");
    ui->lineEdit_password->setText("");

//if checkbox is checked
    if (arg1 == 2)
    {
    //changing the widgets state to implement Admin Login
        this->setWindowTitle("Librarian - Admin Login");
        ui->label_login->setText("Admin Login");
        ui->lineEdit_username->setPlaceholderText("Username");
        ui->pushButton_signup->setText("Apply for Admin Account");
        ui->pushButton_login->setText("Login as Admin");
    }

//if checkbox is unchecked
    else
    {
    //changing the widgets state to implement Member Login
        this->setWindowTitle("Librarian - Member Login");
        ui->label_login->setText("Login Here");
        ui->lineEdit_username->setPlaceholderText("UID");
        ui->pushButton_signup->setText("Don't have an account? Sign up");
        ui->pushButton_login->setText("Login");
    }
}

void Login::on_pushButton_forgot_clicked()
{
//resetting the line edits
    ui->lineEdit_username->setText("");
    ui->lineEdit_password->setText("");

//creating forgot password window
    Forgot_password f_pass_window(parentWidget());
    f_pass_window.setModal(true);
    f_pass_window.exec();
}

void Login::on_pushButton_signup_clicked()
{
//resetting the line edits
    ui->lineEdit_username->setText("");
    ui->lineEdit_password->setText("");

//if Admin checkbox is clicked
    if (ui->checkBox_admin->isChecked())
    {
    //creating admin signup window
        admin_window = new SignUp_Admin(this);
        ui->stackedWidget_login->insertWidget(1, admin_window);

        ui->stackedWidget_login->setCurrentIndex(1);
    }
//if checkbox is unchecked
    else
    {
    //creating member signup window
        member_window = new SignUp_Member(this);
        ui->stackedWidget_login->insertWidget(2, member_window);

        ui->stackedWidget_login->setCurrentIndex(2);
    }
}

void Login::on_pushButton_login_clicked()
{
//takin in values from the line edits
    QString lUsername, lPassword;
    lUsername = ui->lineEdit_username->text();
    lPassword = ui->lineEdit_password->text();

    //encrypting the password
            QByteArray encryptedPassword = lPassword.toUtf8();
            encryptedPassword = QCryptographicHash::hash(encryptedPassword, QCryptographicHash::Md5);

//resetting the line edits
    ui->lineEdit_username->setText("");
    ui->lineEdit_password->setText("");

//imposing constraints on the input fields and displaying warning messages accordingly
//if both UID/Username and Password fields are empty
    if (lUsername == "" && lPassword == "")
    {
    //if admin checkbox is checked
        if (ui->checkBox_admin->isChecked())
            QMessageBox::warning(this, "Warning!", "Username and Password Fields cannot be Empty!");
    //if checkbox is unchecked
        else
            QMessageBox::warning(this, "Warning!", "UID and Password Fields cannot be Empty!");
    }

//if UID/Username field is left empty
    else if (lUsername == "")
    {
    //if admin checkbox is checked
        if (ui->checkBox_admin->isChecked())
            QMessageBox::warning(this, "Warning!", "Username Field cannot be Empty! ");
    //if checkbox is unchecked
        else
            QMessageBox::warning(this, "Warning!", "UID Field cannot be Empty! ");
    }

//if password field is left empty
    else if (lPassword == "")
    {
        QMessageBox::warning(this, "Warning!", "Password Field cannot be Empty! ");
    }

//if database is not connected
    else if (!Database::connectDB())
    {
        QMessageBox::warning(this, "Warning!", "Database not Connected! ");
    }

//if all conditions are satisfied
    else
    {
    //connecting to the database
        Database::connectDB();
    //declaring a query object
        QSqlQuery query;
    //checking if the admin checkbox is clicked
        if (ui->checkBox_admin->isChecked())
        {
        //preparing the query on the Admins table to check for existing users
            query.prepare("select * from Admins where Username = '" + lUsername + "'and Password = '" + encryptedPassword + "'");

        //executing the prepared query and checking if executed or not
        //if executed
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
                    QMessageBox::about(this, "Login Successful!", "Logged in Successfully as an Admin ");
                //closing the datbase connection
                    Database::closeDB();

                //creating member signup window
                    adminDashboard_window = new Admin_Dashboard(lUsername, this);
                    ((QStackedWidget *)parent())->insertWidget(2, adminDashboard_window);
                    ((QStackedWidget *)parent())->setCurrentIndex(2);
                }

            //if match not found
                else
                {
                //displaying system warning
                    QMessageBox::warning(this, "Login Unsuccessful!", "Admin not Found. Wrong Credentials ");
                }
            }

        //if query not executed
            else
            {
            //displaying the critical message dialog with the corresponding error
                QMessageBox::critical(this, "Error!", query.lastError().text());

            //Updating the database connection in the status bar label
            }
        }

    //if admin checkbox is unchecked
        else
        {
        //preparing the query on the Members table to check for existing users
            query.prepare("select * from Members where UID = '" + lUsername + "'and Password = '" + encryptedPassword + "'");

        //executing the prepared query and checking if executed or not
        //if executed
            if (query.exec())
            {
            //checking if Member exists
                bool var = false;
                if (query.next())
                    var = true;

            //if match found
                if (var)
                {
                //displaying system message
                    QMessageBox::about(this, "Login Successful!", "Logged in Successfully! Welcome ");
                //closing the datbase connection
                    Database::closeDB();

                //creating member signup window
                    memberDashboard_window = new Member_Dashboard(lUsername, this);
                    ((QStackedWidget *)parent())->insertWidget(3, memberDashboard_window);
                    ((QStackedWidget *)parent())->setCurrentIndex(3);
                }

            //if match not found
                else
                {
                //displaying system warning
                    QMessageBox::warning(this, "Login Unsuccessful!", "Member not Found. Wrong Credentials ");
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

void Login::on_lineEdit_username_textChanged(const QString &arg1)
{
    if(arg1 == "")
        ui->lineEdit_username->setStyleSheet("color: #717072;");
    else
        ui->lineEdit_username->setStyleSheet("color:white;");
}


void Login::on_lineEdit_password_textChanged(const QString &arg1)
{
    if(arg1 == "")
        ui->lineEdit_password->setStyleSheet("color: #717072;");
    else
        ui->lineEdit_password->setStyleSheet("color:white;");
}

