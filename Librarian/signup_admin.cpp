#include "signup_admin.h"
#include "ui_signup_admin.h"

SignUp_Admin::SignUp_Admin(QWidget *parent) : QMainWindow(parent), ui(new Ui::SignUp_Admin)
{
    ui->setupUi(this);

    nativeParentWidget()->setWindowTitle("Librarian - Admin Application");

    ui->label_passStrength->hide();

//Deleting the Form instance when window is closed
    this->setAttribute(Qt::WA_DeleteOnClose);

//adding options in gender combo-box
    ui->comboBox_gender->addItem("Male");
    ui->comboBox_gender->addItem("Female");
    ui->comboBox_gender->addItem("Other");

//adding options in profession combo-box
    ui->comboBox_profession->addItem("Staff");
    ui->comboBox_profession->addItem("Professor");

//adding options in security question combo-box
    ui->comboBox_question->addItem("What is your birth year?");
    ui->comboBox_question->addItem("Which city do you live in?");
    ui->comboBox_question->addItem("What is your pet name?");
    ui->comboBox_question->addItem("What is your favourite Book?");
    ui->comboBox_question->addItem("Who is your favourite Author?");

//Restricting the input field for 'contact no.' lineedit to digits only
    ui->lineEdit_phone->setValidator(new QIntValidator(ui->lineEdit_phone));
}

SignUp_Admin::~SignUp_Admin()
{
    delete ui;
}

void SignUp_Admin::on_pushButton_adLogin_clicked()
{
    ((QStackedWidget *)parent())->setCurrentIndex(0);
    this->close();
    nativeParentWidget()->setWindowTitle("Librarian - Login");
}

void SignUp_Admin::on_pushButton_submit_clicked()
{
//taking values from the input fields
    QString fName, lName, gender, profession, UID, password, question, answer, contact;
    fName = ui->lineEdit_fName->text();
    lName = ui->lineEdit_lName->text();
    gender = ui->comboBox_gender->currentText();
    profession = ui->comboBox_profession->currentText();
    UID = ui->lineEdit_username->text();
    password = ui->lineEdit_password->text();
    question = ui->comboBox_question->currentText();
    answer = ui->lineEdit_answer->text();
    contact = ui->lineEdit_phone->text();

    //encrypting the password
            QByteArray encryptedPassword = password.toUtf8();
            encryptedPassword = QCryptographicHash::hash(encryptedPassword, QCryptographicHash::Md5);

            //encrypting the password
            QByteArray encryptedAnswer = answer.toUtf8();
            encryptedAnswer = QCryptographicHash::hash(encryptedAnswer, QCryptographicHash::Md5);


//imposing constraints on the input fields and displaying warning messages accordingly
//if the form is empty
    if (fName == "" && lName == "" && gender == "" && profession == "" && UID == "" && password == "" && question == "" && answer == "" && contact == "")
        QMessageBox::warning(this, "Warning!", "All Fields are Mandatory! ");

//if first name field is empty
    else if (fName == "")
        QMessageBox::warning(this, "Warning", "First Name cannot be Empty! ");

//if last name field is empty
    else if (lName == "")
        QMessageBox::warning(this, "Warning", "Last Name cannot be Empty! ");

//if gender is not selected
    else if (gender == "")
        QMessageBox::warning(this, "Warning", "Please Select your Gender ");

//if profession is selected
    else if (profession == "")
        QMessageBox::warning(this, "Warning", "Please Select your Profession ");

//if Username field is empty
    else if (UID == "")
        QMessageBox::warning(this, "Warning", "Username cannot be Empty! ");

//if password field is empty
    else if (password == "")
        QMessageBox::warning(this, "Warning", "Password cannot be Empty! ");

//if security question is not selected
    else if (question == "")
        QMessageBox::warning(this, "Warning", "Please Select a Security Question ");

//if answer field is empty
    else if (answer == "")
        QMessageBox::warning(this, "Warning", "Answer cannot be Empty! ");

//if contact no field is empty
    else if (contact == "")
        QMessageBox::warning(this, "Warning", "Contact Number cannot be Empty! ");

//if contact no is less than 10 digits
    else if (contact.size() < 10)
        QMessageBox::warning(this, "Warning", "Contact Number cannot be less than 10 digits! ");

//if password is nt Strong
    else if(ui->label_passStrength->text() != "Password is Strong!")
        QMessageBox::warning(this, "Warning", "Password needs to be Strong! ");

//if database is not connected
    else if (!Database::connectDB())
        QMessageBox::warning(this, "Warning!", "Database not Connected! ");

//if all conditions are satisfied
    else
    {
    //opening the connection to the database
        Database::connectDB();
    //creating the query object
        QSqlQuery query;

    //preparing the query
        query.prepare("select * from Admin_Applications where Username = '" + UID + "'");

    //executing the query and checking if query executed successfully
    //if query executed
        if (query.exec())
        {
        //checking if admin exists
            bool var = false;
            if (query.next())
                var = true;
        //closing the connection to the database
            Database::closeDB();

        //if no match found
            if (!var)
            {
            //opening the connection to the database
                Database::connectDB();

            //creating the query object
                QSqlQuery query;

            //preparing the query
                query.prepare("insert into Admin_Applications (First_Name, Last_Name, Gender, Profession, Username, Password, Security_Ques, Answer, Contact_No) "
                              "values('" + fName + "','" + lName + "','" + gender + "','" + profession + "','" + UID + "','" + encryptedPassword + "','" + question + "','" + encryptedAnswer + "','" + contact + "')");

            //executing the query and checking if query executed successfully
            //if query executed
                if (query.exec())
                {
                //displaying system message
                    QMessageBox::about(this, "Submission Successful!", "Admin Account Application has been Submitted ");

                //closing the datbase connection
                    Database::closeDB();

                //showing the login window
                    ((QStackedWidget *)parent())->setCurrentIndex(0);

                //closing the signup window
                    this->close();
                    nativeParentWidget()->setWindowTitle("Librarian - Login");
                }

            //if query does not execute
                else
                {
                //displaying the critical message dialog with the corresponding error
                    QMessageBox::critical(this, "Error : ", query.lastError().text());
                }
            }

        //if match found
            else
            {
            //displaying system warning
                QMessageBox::warning(this, "Submission Unsuccessful!", "Application already exists! ");
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

void SignUp_Admin::on_lineEdit_fName_textChanged(const QString &arg1)
{
    if(arg1 == "")
        ui->lineEdit_fName->setStyleSheet("color: #717072;");
    else
        ui->lineEdit_fName->setStyleSheet("color:white;");
}


void SignUp_Admin::on_lineEdit_lName_textChanged(const QString &arg1)
{
    if(arg1 == "")
        ui->lineEdit_lName->setStyleSheet("color: #717072;");
    else
        ui->lineEdit_lName->setStyleSheet("color:white;");
}


void SignUp_Admin::on_comboBox_gender_currentIndexChanged(int index)
{
    if(index == -1)
        ui->comboBox_gender->setStyleSheet("color: #717072;");
    else
        ui->comboBox_gender->setStyleSheet("color:white;");
}


void SignUp_Admin::on_comboBox_profession_currentIndexChanged(int index)
{
    if(index == -1)
        ui->comboBox_profession->setStyleSheet("color: #717072;");
    else
        ui->comboBox_profession->setStyleSheet("color:white;");
}


void SignUp_Admin::on_lineEdit_username_textChanged(const QString &arg1)
{
    if(arg1 == "")
        ui->lineEdit_username->setStyleSheet("color: #717072;");
    else
        ui->lineEdit_username->setStyleSheet("color:white;");
}


void SignUp_Admin::on_lineEdit_password_textChanged(const QString &arg1)
{
    if(arg1 == "")
    {
        ui->lineEdit_password->setStyleSheet("color: #717072;");
        ui->label_passStrength->hide();
    }
    else
    {
        ui->lineEdit_password->setStyleSheet("color:white;");
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


void SignUp_Admin::on_comboBox_question_currentIndexChanged(int index)
{
    if(index == -1)
        ui->comboBox_question->setStyleSheet("color: #717072;");
    else
        ui->comboBox_question->setStyleSheet("color:white;");
}


void SignUp_Admin::on_lineEdit_answer_textChanged(const QString &arg1)
{
    if(arg1 == "")
        ui->lineEdit_answer->setStyleSheet("color: #717072;");
    else
        ui->lineEdit_answer->setStyleSheet("color:white;");
}


void SignUp_Admin::on_lineEdit_phone_textChanged(const QString &arg1)
{
    if(arg1 == "")
        ui->lineEdit_phone->setStyleSheet("color: #717072;");
    else
        ui->lineEdit_phone->setStyleSheet("color:white;");
}

