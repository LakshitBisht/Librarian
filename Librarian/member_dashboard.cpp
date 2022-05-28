#include "member_dashboard.h"
#include "ui_member_dashboard.h"

Member_Dashboard::Member_Dashboard(QString user, QWidget *parent) : QMainWindow(parent),
                                                                    ui(new Ui::Member_Dashboard)
{
    ui->setupUi(this);

    this->user = user;

    nativeParentWidget()->setWindowTitle("Librarian - Member DashBoard");

//Deleting the Form instance when window is closed
    this->setAttribute(Qt::WA_DeleteOnClose);

//opening the connection to the database
    Database::connectDB();

    ui->stackedWidget_Member->setCurrentIndex(0);

//creating a query object
    QSqlQuery query;

//preparing the query for setting the line edits
    query.prepare("select First_Name, Last_Name, Profession from Members where UID = '" + user + "'");

//executing the query and checking if query executed successfully
//if query executed
    if (query.exec())
    {
    //extracting values from the query and setting it to the line edits
        query.next();
        QString name, profession;
        name = query.value(0).toString();
        name += " " + query.value(1).toString();
        profession = query.value(2).toString();

        ui->label_Name->setText(name);
        ui->label_UID->setText(user);
        ui->label_Profession->setText(profession);
    }

//if query not executed
    else
    {
    //displaying critical system message with the corresponding error
        QMessageBox::critical(this, "Error", query.lastError().text());
    }

//preparing the query for setting the line edits
    query.prepare("select count(*) from Messages_Member");

//executing the query and checking if query executed successfully
//if query executed
    if (query.exec())
    {
    //extracting values from the query and setting it to the line edits
        query.next();
        int row;
        row = query.value(0).toInt();

        if (row == 0)
        {
            ui->toolButton_messages->setIcon(QIcon(":/img/images/emptyApplication.png"));
        }

    //closing the database connection
        Database::closeDB();
    }

//if query not executed
    else
    {
    //displaying critical system message with the corresponding error
        QMessageBox::critical(this, "Error", query.lastError().text());
    }
}

Member_Dashboard::~Member_Dashboard()
{
    delete ui;
}

void Member_Dashboard::on_pushButton_Logout_clicked()
{
    ((QStackedWidget *)parent())->setCurrentIndex(1);
    nativeParentWidget()->setWindowTitle("Librarian - Login");
}

void Member_Dashboard::on_pushButton_AccountDetails_clicked()
{
//linking the admin_accountdetails to the dashboard
    accountDetailsWidget = new Admin_AccountDetails("Members", user, ui->label_Name, ui->label_UID, this);
    ui->stackedWidget_Member->insertWidget(0, accountDetailsWidget);

    ui->stackedWidget_Member->setCurrentIndex(0);
}

void Member_Dashboard::on_pushButton_allBooks_clicked()
{
//linking the bookDeatils to the dashboard
    bookDetailsWidget = new BookDetails("Members", user, ui->stackedWidget_Member);
    ui->stackedWidget_Member->insertWidget(1, bookDetailsWidget);

    ui->stackedWidget_Member->setCurrentIndex(1);
}

void Member_Dashboard::on_pushButton_currentlyIssued_clicked()
{
    registrationLogWidget = new RegistrationLog("CurrentlyIssued", user, ui->stackedWidget_Member);
    ui->stackedWidget_Member->insertWidget(2, registrationLogWidget);
    ui->stackedWidget_Member->setCurrentIndex(2);
}

void Member_Dashboard::on_toolButton_messages_clicked()
{
    memberMessagesWidget = new Member_Messages(user, ui->toolButton_messages, ui->stackedWidget_Member);
    ui->stackedWidget_Member->insertWidget(3, memberMessagesWidget);
    ui->stackedWidget_Member->setCurrentIndex(3);
}

void Member_Dashboard::on_toolButton_issueLog_clicked()
{
    registrationLogWidget = new RegistrationLog("IssuedLog", user, ui->stackedWidget_Member);
    ui->stackedWidget_Member->insertWidget(4, registrationLogWidget);
    ui->stackedWidget_Member->setCurrentIndex(4);
}
