#include "memberadmindetails.h"
#include "ui_memberadmindetails.h"

MemberAdminDetails::MemberAdminDetails(QWidget *parent) : QWidget(parent),
                                                          ui(new Ui::MemberAdminDetails)
{
    ui->setupUi(this);

//Deleting the Form instance when window is closed
    this->setAttribute(Qt::WA_DeleteOnClose);

    ui->lineEdit_searchBar->setReadOnly(true);
    ui->lineEdit_searchBar_2->setReadOnly(true);

    filter_proxy_model = new QSortFilterProxyModel();
    filter_proxy_model_2 = new QSortFilterProxyModel();

    connect(ui->lineEdit_searchBar,SIGNAL(textChanged(QString)),filter_proxy_model,SLOT(setFilterFixedString(QString)));
    connect(ui->lineEdit_searchBar_2,SIGNAL(textChanged(QString)),filter_proxy_model_2,SLOT(setFilterFixedString(QString)));


//creating the tableview for Members
    connectTableDatabase("Members");

    ui->comboBox_search->addItem("First Name");
    ui->comboBox_search->addItem("Last Name");
    ui->comboBox_search->addItem("UID");
    ui->comboBox_search->addItem("Gender");
    ui->comboBox_search->addItem("Profession");
    ui->comboBox_search->addItem("Contact No");

//creating the tableview for Admins
    connectTableDatabase("Admins");

    ui->comboBox_search_2->addItem("First Name");
    ui->comboBox_search_2->addItem("Last Name");
    ui->comboBox_search_2->addItem("Username");
    ui->comboBox_search_2->addItem("Gender");
    ui->comboBox_search_2->addItem("Profession");
    ui->comboBox_search_2->addItem("Contact No");
}

MemberAdminDetails::~MemberAdminDetails()
{
    delete ui;
}

void MemberAdminDetails::connectTableDatabase(QString tableName)
{
//opening connection the the database
    Database::connectDB();
    model = new QSqlQueryModel();
    QSqlQuery query;

    QString temp;
    if (tableName == "Members")
        temp = "UID";
    else
        temp = "Username";
//preparing the query
    query.prepare("select First_name, Last_Name, " + temp + ", Gender, Profession, Contact_No from " + tableName);

//executing the query and checking if query executed successfully
//if query executed
    if (query.exec())
    {
    //extracting the data for further operation
        model->setQuery(query);
    //setting up extracted value in table view
        if (tableName == "Members")
        {
            filter_proxy_model->setDynamicSortFilter(true);
            filter_proxy_model->setSourceModel(model);

            ui->tableView_member->setModel(filter_proxy_model);
            ui->tableView_member->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        }
        else
        {
            filter_proxy_model_2->setDynamicSortFilter(true);
            filter_proxy_model_2->setSourceModel(model);
            ui->tableView_admin->setModel(filter_proxy_model_2);
            ui->tableView_admin->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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

void MemberAdminDetails::on_pushButton_removeMem_clicked()
{
    QModelIndex tblIndex = ui->tableView_member->currentIndex();
    QModelIndex selectedUserIndex = tblIndex.sibling(tblIndex.row(), 2);
    QString UID = ui->tableView_member->model()->data(selectedUserIndex).toString();

    if (UID == "")
    {
        QMessageBox::warning(this, "Remove Member", "No Member Selected! ");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Remove Member", "Do You Want To Remove the Member?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        Database::connectDB();

        QSqlQuery query;
        query.prepare("delete from Members where UID = '" + UID + "'");
    //executing the query and checking if query executed successfully
    //if query executed
        if (query.exec())
        {

        //preparing the query
            query.prepare("select First_name, Last_Name, UID, Gender, Profession, Contact_No from Members");

        //executing the query and checking if query executed successfully
        //if query executed
            if (query.exec())
            {
                QMessageBox::about(this, "Remove Member", "Member Removed Successfully!");
            //extracting the data for further operation
                model->setQuery(query);
            //setting up extracted value in table view
                ui->tableView_member->setModel(model);
                ui->tableView_member->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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

    //if query not executed
        else
        {
        //displaying critical system message with the corresponding error
            QMessageBox::critical(this, "Error", query.lastError().text());
        }
    }
}

void MemberAdminDetails::on_comboBox_search_currentIndexChanged(int index)
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


void MemberAdminDetails::on_comboBox_search_2_currentIndexChanged(int index)
{
    if(index == -1)
            ui->comboBox_search_2->setStyleSheet("color: #717072;");
        else
            ui->comboBox_search_2->setStyleSheet("color:white;");

    ui->lineEdit_searchBar_2->setReadOnly(false);

    if(index == 5)
    {
        ui->lineEdit_searchBar_2->setValidator(new QIntValidator(ui->lineEdit_searchBar_2));
    }
    else
    {
        ui->lineEdit_searchBar_2->setValidator(nullptr);
    }

    filter_proxy_model_2->setFilterCaseSensitivity(Qt::CaseInsensitive);
    filter_proxy_model_2->setFilterKeyColumn(index);
}


void MemberAdminDetails::on_lineEdit_searchBar_textChanged(const QString &arg1)
{
    if(arg1 == "")
            ui->lineEdit_searchBar->setStyleSheet("color: #717072;");
        else
            ui->lineEdit_searchBar->setStyleSheet("color:white;");
}


void MemberAdminDetails::on_lineEdit_searchBar_2_textChanged(const QString &arg1)
{
    if(arg1 == "")
            ui->lineEdit_searchBar_2->setStyleSheet("color: #717072;");
        else
            ui->lineEdit_searchBar_2->setStyleSheet("color:white;");
}

