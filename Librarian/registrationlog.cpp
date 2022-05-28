#include "registrationlog.h"
#include "ui_registrationlog.h"

RegistrationLog::RegistrationLog(QString account, QString user, QWidget *parent) : QWidget(parent),
                                                                                   ui(new Ui::RegistrationLog)
{
    ui->setupUi(this);

    this->account = account;
    this->user = user;

    ui->lineEdit_searchBar->setReadOnly(true);

//Deleting the Form instance when window is closed
    this->setAttribute(Qt::WA_DeleteOnClose);

//opening connection the the database
    Database::connectDB();
    model = new QSqlQueryModel();

    filter_proxy_model = new QSortFilterProxyModel();
    filter_proxy_model->setDynamicSortFilter(true);
    filter_proxy_model->setSourceModel(model);

    connect(ui->lineEdit_searchBar,SIGNAL(textChanged(QString)),filter_proxy_model,SLOT(setFilterFixedString(QString)));


    QSqlQuery query;

    if (account == "RegistrationLog")
    {
    //preparing the query
        query.prepare("select * from Registration_Log");

        ui->comboBox_search->addItem("Date");
        ui->comboBox_search->addItem("UID");
        ui->comboBox_search->addItem("Name");
        ui->comboBox_search->addItem("Account");
    }
    else if (account == "IssuedLog")
    {
        query.prepare("select Book_Name, Author_Name, Publication, Genre, Book_ID, Issued_Date from Issued_Log where UID = '" + user + "'");
        ui->groupBox_registrationLog->setTitle("Issued Books Log");

        ui->comboBox_search->addItem("Book Name");
        ui->comboBox_search->addItem("Author Name");
        ui->comboBox_search->addItem("Publication");
        ui->comboBox_search->addItem("Genre");
        ui->comboBox_search->addItem("Book ID");
        ui->comboBox_search->addItem("Issued Date");
    }
    else
    {
        query.prepare("select Book_Name , Author_Name, Publication, Genre, Book_ID, Issued_Date from Issued_Books where UID = '" + user + "'");
        ui->groupBox_registrationLog->setTitle("Currently Issued Books");

        ui->comboBox_search->addItem("Book Name");
        ui->comboBox_search->addItem("Author Name");
        ui->comboBox_search->addItem("Publication");
        ui->comboBox_search->addItem("Genre");
        ui->comboBox_search->addItem("Book ID");
        ui->comboBox_search->addItem("Issued Date");
    }

//executing the query and checking if query executed successfully
//if query executed
    if (query.exec())
    {
    //extracting the data for further operation
        model->setQuery(query);
    //setting up extracted value in table view
        ui->tableView_registerLog->setModel(filter_proxy_model);
        ui->tableView_registerLog->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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

RegistrationLog::~RegistrationLog()
{
    delete ui;
}

void RegistrationLog::on_comboBox_search_currentIndexChanged(int index)
{
    if(index == -1)
            ui->comboBox_search->setStyleSheet("color: #717072;");
        else
            ui->comboBox_search->setStyleSheet("color:white;");

    ui->lineEdit_searchBar->setReadOnly(false);

    filter_proxy_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
    filter_proxy_model->setFilterKeyColumn(index);
}


void RegistrationLog::on_lineEdit_searchBar_textChanged(const QString &arg1)
{
    if(arg1 == "")
            ui->lineEdit_searchBar->setStyleSheet("color: #717072;");
        else
            ui->lineEdit_searchBar->setStyleSheet("color:white;");
}

