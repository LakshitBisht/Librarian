#include "adminapplication.h"
#include "ui_adminapplication.h"

AdminApplication::AdminApplication(QToolButton *iconButton, QWidget *parent) : QWidget(parent),
                                                                               ui(new Ui::AdminApplication)
{
    ui->setupUi(this);

    this->iconButton = iconButton;

//Deleting the Form instance when window is closed
    this->setAttribute(Qt::WA_DeleteOnClose);

    ui->comboBox_search->addItem("First Name");
    ui->comboBox_search->addItem("Last Name");
    ui->comboBox_search->addItem("Username");
    ui->comboBox_search->addItem("Gender");
    ui->comboBox_search->addItem("Profession");
    ui->comboBox_search->addItem("Contact No");

    ui->lineEdit_searchBar->setReadOnly(true);

//opening connection the the database
    Database::connectDB();

    model = new QSqlQueryModel();

    filter_proxy_model = new QSortFilterProxyModel();
    filter_proxy_model->setDynamicSortFilter(true);
    filter_proxy_model->setSourceModel(model);

    connect(ui->lineEdit_searchBar,SIGNAL(textChanged(QString)),filter_proxy_model,SLOT(setFilterFixedString(QString)));


    QSqlQuery query;

//preparing the query
    query.prepare("select First_name, Last_Name, Username, Gender, Profession, Contact_No from Admin_Applications");

//executing the query and checking if query executed successfully
//if query executed
    if (query.exec())
    {
    //extracting the data for further operation
        model->setQuery(query);
    //setting up extracted value in table view
        ui->tableView_adminApplication->setModel(filter_proxy_model);
        ui->tableView_adminApplication->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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

AdminApplication::~AdminApplication()
{
    delete ui;
}

void AdminApplication::on_pushButton_applicationAccept_clicked()
{
    QModelIndex tblIndex = ui->tableView_adminApplication->currentIndex();
    QModelIndex selectedUserIndex = tblIndex.sibling(tblIndex.row(), 2);
    QString username = ui->tableView_adminApplication->model()->data(selectedUserIndex).toString();

    selectedUserIndex = tblIndex.sibling(tblIndex.row(), 0);
    QString fName = ui->tableView_adminApplication->model()->data(selectedUserIndex).toString();
    selectedUserIndex = tblIndex.sibling(tblIndex.row(), 1);
    QString lName = ui->tableView_adminApplication->model()->data(selectedUserIndex).toString();

    if (username == "")
    {
        QMessageBox::warning(this, "Accept Application", "No Application Selected! ");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Accept", "Do You Want To Accept the Application?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        Database::connectDB();
        QSqlQuery query;
        query.prepare("insert into Admins select * from Admin_Applications where Username = '" + username + "'");

    //executing the query and checking if query executed successfully
    //if query executed
        if (query.exec())
        {
            query.prepare("insert into Registration_Log (Registration_Date, Username_UID, Name, Account) "
                          "values('" +
                          QDate::currentDate().toString() + "','" + username + "','" + fName + " " + lName + "','Admin')");
            if (!query.exec())
            {
            //displaying the critical message dialog with the corresponding error
                QMessageBox::critical(this, "Error : ", query.lastError().text());
            }

        //closing the database connection
            Database::closeDB();

            QMessageBox::about(this, "Application Accepted", "Admin Registered Successfully!");
            deleteApplication(username);
            updateIcon();
        }

    //if query not executed
        else
        {
        //displaying critical system message with the corresponding error
            QMessageBox::critical(this, "Error", query.lastError().text());
        }
    }
}

void AdminApplication::on_pushButton_applicationReject_clicked()
{
    QModelIndex tblIndex = ui->tableView_adminApplication->currentIndex();
    QModelIndex selectedUserIndex = tblIndex.sibling(tblIndex.row(), 2);
    QString username = ui->tableView_adminApplication->model()->data(selectedUserIndex).toString();

    if (username == "")
    {
        QMessageBox::warning(this, "Reject Application", "No Application Selected! ");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Reject", "Do You Want To Reject the Application?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {

        if (deleteApplication(username))
        {
            QMessageBox::about(this, "Application Rejection", "Application Rejected Successfully!");
            updateIcon();
        }
    }
}

bool AdminApplication::deleteApplication(QString username)
{
    Database::connectDB();
    QSqlQuery query;
    query.prepare("delete from Admin_Applications where Username = '" + username + "'");
//executing the query and checking if query executed successfully
//if query executed
    if (query.exec())
    {
    //preparing the query
        query.prepare("select First_name, Last_Name, Username, Gender, Profession, Contact_No from Admin_Applications");

    //executing the query and checking if query executed successfully
    //if query executed
        if (query.exec())
        {
        //extracting the data for further operation
            model->setQuery(query);
        //setting up extracted value in table view
            ui->tableView_adminApplication->setModel(model);
            ui->tableView_adminApplication->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        //closing the database connection
            Database::closeDB();
            return true;
        }

    //if query not executed
        else
        {
        //displaying critical system message with the corresponding error
            QMessageBox::critical(this, "Error", query.lastError().text());
        }
    }

//if query not executed
    else
    {
    //displaying critical system message with the corresponding error
        QMessageBox::critical(this, "Error", query.lastError().text());
    }
    return false;
}

void AdminApplication::updateIcon()
{
    Database::connectDB();
    QSqlQuery query;
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
            iconButton->setIcon(QIcon(":/img/images/emptyApplication.png"));
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

void AdminApplication::on_comboBox_search_currentIndexChanged(int index)
{
    if(index == -1)
            ui->comboBox_search->setStyleSheet("color: #717072;");
        else
            ui->comboBox_search->setStyleSheet("color:white;");


    ui->lineEdit_searchBar->setReadOnly(false);

    if(index == 5)
    {
        ui->lineEdit_searchBar->setValidator(new QIntValidator(ui->lineEdit_searchBar));
    }
    else
    {
        ui->lineEdit_searchBar->setValidator(nullptr);
    }
    filter_proxy_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
    filter_proxy_model->setFilterKeyColumn(index);
}


void AdminApplication::on_lineEdit_searchBar_textChanged(const QString &arg1)
{
    if(arg1 == "")
            ui->lineEdit_searchBar->setStyleSheet("color: #717072;");
        else
            ui->lineEdit_searchBar->setStyleSheet("color:white;");
}

