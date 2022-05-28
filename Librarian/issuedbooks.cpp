#include "issuedbooks.h"
#include "ui_issuedbooks.h"

IssuedBooks::IssuedBooks(QWidget *parent) : QWidget(parent),
                                            ui(new Ui::IssuedBooks)
{
    ui->setupUi(this);

//Deleting the Form instance when window is closed
    this->setAttribute(Qt::WA_DeleteOnClose);

    ui->stackedWidget_issuedBooks->setCurrentIndex(0);

    ui->lineEdit_searchBar->setReadOnly(true);
    ui->lineEdit_searchBar_2->setReadOnly(true);

    filter_proxy_model = new QSortFilterProxyModel();
    filter_proxy_model_2 = new QSortFilterProxyModel();

    connect(ui->lineEdit_searchBar,SIGNAL(textChanged(QString)),filter_proxy_model,SLOT(setFilterFixedString(QString)));
    connect(ui->lineEdit_searchBar_2,SIGNAL(textChanged(QString)),filter_proxy_model_2,SLOT(setFilterFixedString(QString)));

    ui->comboBox_search->addItem("Book Name");
    ui->comboBox_search->addItem("Author Name");
    ui->comboBox_search->addItem("Publication");
    ui->comboBox_search->addItem("Genre");
    ui->comboBox_search->addItem("Book ID");
    ui->comboBox_search->addItem("UID");
    ui->comboBox_search->addItem("Issued Date");

    ui->comboBox_search_2->addItem("Book Name");
    ui->comboBox_search_2->addItem("Author Name");
    ui->comboBox_search_2->addItem("Publication");
    ui->comboBox_search_2->addItem("Genre");
    ui->comboBox_search_2->addItem("Book ID");
    ui->comboBox_search_2->addItem("UID");

//opening connection the the database
    Database::connectDB();
    model = new QSqlQueryModel();

    filter_proxy_model->setDynamicSortFilter(true);
    filter_proxy_model->setSourceModel(model);

    QSqlQuery query;

//preparing the query
    query.prepare("select * from Issued_Books");

//executing the query and checking if query executed successfully
//if query executed
    if (query.exec())
    {
    //extracting the data for further operation
        model->setQuery(query);
    //setting up extracted value in table view
        ui->tableView_issuedBooks->setModel(filter_proxy_model);
        ui->tableView_issuedBooks->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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

IssuedBooks::~IssuedBooks()
{
    delete ui;
}

void IssuedBooks::on_pushButton_returnBook_clicked()
{
    QModelIndex tblIndex = ui->tableView_issuedBooks->currentIndex();
    QModelIndex selectedUserIndex = tblIndex.sibling(tblIndex.row(), 4);
    QString BookID = ui->tableView_issuedBooks->model()->data(selectedUserIndex).toString();

    if (BookID == "")
    {
        QMessageBox::warning(this, "Delete Book", "No Book Selected! ");
        return;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Return Book", "Do You Want To Return Issued Book?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        Database::connectDB();

        QSqlQuery query;
        query.prepare("update Books set Status = 'Available' where Book_ID = '" + BookID + "'");

    //executing the query and checking if query executed successfully
    //if query executed
        if (query.exec())
        {
            query.prepare("delete from Issued_Books where Book_ID = '" + BookID + "'");
        //executing the query and checking if query executed successfully
        //if query executed
            if (query.exec())
            {

            //preparing the query
                query.prepare("select * from Issued_Books");

            //executing the query and checking if query executed successfully
            //if query executed
                if (query.exec())
                {
                    QMessageBox::about(this, "Return Book", "Book Returned Successfully!");
                //extracting the data for further operation
                    model->setQuery(query);
                //setting up extracted value in table view
                    ui->tableView_issuedBooks->setModel(filter_proxy_model);
                    ui->tableView_issuedBooks->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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

    //if query not executed
        else
        {
        //displaying critical system message with the corresponding error
            QMessageBox::critical(this, "Error", query.lastError().text());
        }
    }
}

void IssuedBooks::on_pushButton_requests_clicked()
{

//opening connection the the database
    Database::connectDB();
    model = new QSqlQueryModel();

    filter_proxy_model_2->setDynamicSortFilter(true);
    filter_proxy_model_2->setSourceModel(model);

    QSqlQuery query;

//preparing the query
    query.prepare("select * from Issued_Books_Requests");

//executing the query and checking if query executed successfully
//if query executed
    if (query.exec())
    {
    //extracting the data for further operation
        model->setQuery(query);
    //setting up extracted value in table view
        ui->tableView_issueRequests->setModel(filter_proxy_model_2);
        ui->tableView_issueRequests->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //closing the database connection
        Database::closeDB();
    }

//if query not executed
    else
    {
    //displaying critical system message with the corresponding error
        QMessageBox::critical(this, "Error", query.lastError().text());
    }

    ui->stackedWidget_issuedBooks->setCurrentIndex(1);
}

void IssuedBooks::on_toolButton_back_clicked()
{
    ui->stackedWidget_issuedBooks->setCurrentIndex(0);
}

void IssuedBooks::on_pushButton_accept_clicked()
{
    QModelIndex tblIndex = ui->tableView_issueRequests->currentIndex();
    QString bookdetails[7];

    for (int i = 0; i < 6; i++)
    {
        QModelIndex selectedUserIndex = tblIndex.sibling(tblIndex.row(), i);
        bookdetails[i] = ui->tableView_issueRequests->model()->data(selectedUserIndex).toString();
    }

    if (bookdetails[4] == "")
    {
        QMessageBox::warning(this, "Accept Request", "No Request Selected! ");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Accept", "Do You Want To Accept the Application?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        Database::connectDB();
        QSqlQuery query, query2, query3;
        query.prepare("insert into Issued_Books (Book_Name, Author_Name, Publication, Genre, Book_ID, UID, Issued_Date) "
                      "values('" +
                      bookdetails[0] + "','" + bookdetails[1] + "','" + bookdetails[2] + "','" + bookdetails[3] + "','" + bookdetails[4] + "','" + bookdetails[5] + "','" + QDate::currentDate().toString() + "')");

        query2.prepare("insert into Issued_Log (Book_Name, Author_Name, Publication, Genre, Book_ID, UID, Issued_Date) "
                       "values('" +
                       bookdetails[0] + "','" + bookdetails[1] + "','" + bookdetails[2] + "','" + bookdetails[3] + "','" + bookdetails[4] + "','" + bookdetails[5] + "','" + QDate::currentDate().toString() + "')");

        query3.prepare("update Books set Status = 'Issued' where Book_ID ='" + bookdetails[4] + "'");
    //executing the query and checking if query executed successfully
    //if query executed
        if (query.exec() && query2.exec() && query3.exec())
        {
        //closing the database connection
            Database::closeDB();

            QMessageBox::about(this, "Request Accepted", "Book Issued Successfully!");
            deleteData(bookdetails[4]);
        }

    //if query not executed
        else
        {
        //displaying critical system message with the corresponding error
            QMessageBox::critical(this, "Error", query.lastError().text());
        }
    }
}

void IssuedBooks::on_pushButton_decline_clicked()
{
    QModelIndex tblIndex = ui->tableView_issueRequests->currentIndex();
    QModelIndex selectedUserIndex = tblIndex.sibling(tblIndex.row(), 4);
    QString bookid = ui->tableView_issueRequests->model()->data(selectedUserIndex).toString();

    if (bookid == "")
    {
        QMessageBox::warning(this, "Decline Request", "No Request Selected! ");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Decline", "Do You Want To Decline the Request?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {

        if (deleteData(bookid))
        {
            QMessageBox::about(this, "Request Rejection", "Request Declined!");
        }
    }
}

bool IssuedBooks::deleteData(QString bookid)
{
    Database::connectDB();
    QSqlQuery query;
    query.prepare("delete from Issued_Books_Requests where Book_ID = '" + bookid + "'");
//executing the query and checking if query executed successfully
//if query executed
    if (query.exec())
    {
    //preparing the query
        query.prepare("select * from Issued_Books_Requests");

    //executing the query and checking if query executed successfully
    //if query executed
        if (query.exec())
        {
        //extracting the data for further operation
            model->setQuery(query);
        //setting up extracted value in table view
            ui->tableView_issueRequests->setModel(filter_proxy_model);
            ui->tableView_issueRequests->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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

void IssuedBooks::on_comboBox_search_currentIndexChanged(int index)
{
    if(index == -1)
            ui->comboBox_search->setStyleSheet("color: #717072;");
        else
            ui->comboBox_search->setStyleSheet("color:white;");

    ui->lineEdit_searchBar->setReadOnly(false);

    filter_proxy_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
    filter_proxy_model->setFilterKeyColumn(index);
}


void IssuedBooks::on_comboBox_search_2_currentIndexChanged(int index)
{
    if(index == -1)
            ui->comboBox_search_2->setStyleSheet("color: #717072;");
        else
            ui->comboBox_search_2->setStyleSheet("color:white;");

    ui->lineEdit_searchBar_2->setReadOnly(false);

    filter_proxy_model_2->setFilterCaseSensitivity(Qt::CaseInsensitive);
    filter_proxy_model_2->setFilterKeyColumn(index);
}


void IssuedBooks::on_lineEdit_searchBar_textChanged(const QString &arg1)
{
    if(arg1 == "")
            ui->lineEdit_searchBar->setStyleSheet("color: #717072;");
        else
            ui->lineEdit_searchBar->setStyleSheet("color:white;");
}


void IssuedBooks::on_lineEdit_searchBar_2_textChanged(const QString &arg1)
{
    if(arg1 == "")
            ui->lineEdit_searchBar_2->setStyleSheet("color: #717072;");
        else
            ui->lineEdit_searchBar_2->setStyleSheet("color:white;");
}

