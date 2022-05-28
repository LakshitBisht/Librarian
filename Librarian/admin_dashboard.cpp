#include "admin_dashboard.h"
#include "ui_admin_dashboard.h"

Admin_Dashboard::Admin_Dashboard(QString user, QWidget *parent) : QMainWindow(parent),
                                                                  ui(new Ui::Admin_Dashboard)
{
    ui->setupUi(this);

    this->user = user;

    nativeParentWidget()->setWindowTitle("Librarian - Admin DashBoard");

//Deleting the Form instance when window is closed
    this->setAttribute(Qt::WA_DeleteOnClose);

//opening the connection to the database
    Database::connectDB();

    ui->stackedWidget_Admin->setCurrentIndex(0);

//creating a query object
    QSqlQuery query;

//preparing the query for setting the line edits
    query.prepare("select First_Name, Last_Name, Profession from Admins where Username = '" + user + "'");

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

        ui->label_name->setText(name);
        ui->label_username->setText(user);
        ui->label_profession->setText(profession);
    }

//if query not executed
    else
    {
    //displaying critical system message with the corresponding error
        QMessageBox::critical(this, "Error", query.lastError().text());
    }

//preparing the query for setting the line edits
    query.prepare("select count(*) from Admin_Applications");

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
            ui->toolButton_application->setIcon(QIcon(":/img/images/emptyApplication.png"));
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

Admin_Dashboard::~Admin_Dashboard()
{
    delete ui;
}

void Admin_Dashboard::on_pushButton_logout_clicked()
{
    ((QStackedWidget *)parent())->setCurrentIndex(1);
    nativeParentWidget()->setWindowTitle("Librarian - Login");
}

void Admin_Dashboard::on_pushButton_accountDetails_clicked()
{

//linking the admin_accountdetails to the dashboard
    accountDetailsWidget = new Admin_AccountDetails("Admins", user, ui->label_name, ui->label_username, this);
    ui->stackedWidget_Admin->insertWidget(0, accountDetailsWidget);

    ui->stackedWidget_Admin->setCurrentIndex(0);
}

void Admin_Dashboard::on_pushButton_memberAdminDetails_clicked()
{
//linking the memberadmindetails to the dashboard
    memberAdminDetailsWidget = new MemberAdminDetails(ui->stackedWidget_Admin);
    ui->stackedWidget_Admin->insertWidget(1, memberAdminDetailsWidget);

    ui->stackedWidget_Admin->setCurrentIndex(1);
}

void Admin_Dashboard::on_pushButton_bookDetails_clicked()
{

//linking the bookDeatils to the dashboard
    bookDetailsWidget = new BookDetails("Admins", user, ui->stackedWidget_Admin);
    ui->stackedWidget_Admin->insertWidget(2, bookDetailsWidget);

    ui->stackedWidget_Admin->setCurrentIndex(2);
}

void Admin_Dashboard::on_pushButton_isuueDetails_clicked()
{

//linking the issuedBooksDetails to the dashboard
    issuedBooksWidget = new IssuedBooks(ui->stackedWidget_Admin);
    ui->stackedWidget_Admin->insertWidget(3, issuedBooksWidget);

    ui->stackedWidget_Admin->setCurrentIndex(3);
}

void Admin_Dashboard::on_pushButton_mssg_clicked()
{
//linking the admin_messages to the dashboard
    adminMessagesWidget = new Admin_Messages(user, ui->stackedWidget_Admin);
    ui->stackedWidget_Admin->insertWidget(4, adminMessagesWidget);

    ui->stackedWidget_Admin->setCurrentIndex(4);
}

void Admin_Dashboard::on_toolButton_application_clicked()
{

//linking the admin_accountdetails to the dashboard
    adminApplicationWidget = new AdminApplication(ui->toolButton_application, ui->stackedWidget_Admin);
    ui->stackedWidget_Admin->insertWidget(5, adminApplicationWidget);

    ui->stackedWidget_Admin->setCurrentIndex(5);
}

void Admin_Dashboard::on_toolButton_log_clicked()
{

    registrationLogWidget = new RegistrationLog("RegistrationLog", user, ui->stackedWidget_Admin);
    ui->stackedWidget_Admin->insertWidget(6, registrationLogWidget);
    ui->stackedWidget_Admin->setCurrentIndex(6);
}
