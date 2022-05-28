#include "bookdetails.h"
#include "ui_bookdetails.h"

BookDetails::BookDetails(QString account, QString user, QWidget *parent) : QWidget(parent),
                                                                           ui(new Ui::BookDetails)
{
    ui->setupUi(this);

    this->account = account;
    this->user = user;

    filter_proxy_model = new QSortFilterProxyModel();

    ui->comboBox_search->addItem("Book Name");
    ui->comboBox_search->addItem("Author Name");
    ui->comboBox_search->addItem("Publication");
    ui->comboBox_search->addItem("Genre");
    ui->comboBox_search->addItem("Book ID");
    ui->comboBox_search->addItem("Book Status");

    ui->lineEdit_searchBar->setReadOnly(true);


    if (account == "Admins")
    {
        ui->stackedWidget_buttons->setCurrentIndex(0);
    }

    else
    {
        ui->stackedWidget_buttons->setCurrentIndex(1);
    }

//Deleting the Form instance when window is closed
    this->setAttribute(Qt::WA_DeleteOnClose);

    updateTable();

    connect(ui->lineEdit_searchBar,SIGNAL(textChanged(QString)),filter_proxy_model,SLOT(setFilterFixedString(QString)));
}

BookDetails::~BookDetails()
{
    delete ui;
    Database::closeDB();
}

void BookDetails::on_pushButton_addBook_clicked()
{
//creating forgot password window
    AddBooks addBooks_window(this);
    addBooks_window.setModal(true);
    addBooks_window.exec();
}

void BookDetails::on_pushButton_deleteBook_clicked()
{
    QModelIndex tblIndex = ui->tableView_bookDetails->currentIndex();
    QModelIndex selectedUserIndex = tblIndex.sibling(tblIndex.row(), 4);
    QString BookID = ui->tableView_bookDetails->model()->data(selectedUserIndex).toString();

    if (BookID == "")
    {
        QMessageBox::warning(this, "Delete Book", "No Book Selected! ");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Remove Book", "Do You Want To Remove Book?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        Database::connectDB();

        QSqlQuery query;
        query.prepare("delete from Books where Book_ID = '" + BookID + "'");
    //executing the query and checking if query executed successfully
    //if query executed
        if (query.exec())
        {
            updateTable();
            QMessageBox::about(this, "Remove Book", "Book Removed Successfully!");
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
}

void BookDetails::on_pushButton_Edit_clicked()
{
    QString bookDetails[5];
    QModelIndex tblIndex = ui->tableView_bookDetails->currentIndex();
    for (int i = 0; i < 6; i++)
    {
        QModelIndex selectedfield = tblIndex.sibling(tblIndex.row(), i);
        bookDetails[i] = ui->tableView_bookDetails->model()->data(selectedfield).toString();
    }

    if (bookDetails[4] == "")
    {
        QMessageBox::warning(this, "Edit Book Details", "No Book Selected! ");
        return;
    }
    updateTable();
//creating update book window
    AddBooks addBooks_window(this, bookDetails);
    addBooks_window.setModal(true);
    addBooks_window.exec();
}

void BookDetails::on_pushButton_request_clicked()
{
    QModelIndex tblIndex = ui->tableView_bookDetails->currentIndex();
    QString Book[6];

    for (int i = 0; i < 6; i++)
    {
        QModelIndex bookIndex = tblIndex.sibling(tblIndex.row(), i);
        Book[i] = ui->tableView_bookDetails->model()->data(bookIndex).toString();
    }

    if (Book[4] == "")
    {
        QMessageBox::warning(this, "Request Issue", "No Book Selected! ");
        return;
    }

    if (Book[5] == "Issued")
    {
        QMessageBox::warning(this, "Issue Request", "Book Already Issued!");
        return;
    }

//opening the connection to the database
    Database::connectDB();

//creating the query object
    QSqlQuery query;

//preparing the query
    query.prepare("select * from Issued_Books_Requests where Book_ID = '" + Book[4] + "'");

//executing the query and checking if query executed successfully
//if query executed
    bool var = false;
    if (query.exec())
    {
    //checking if admin exists

        if (query.next())
            var = true;
    }
//if query not executed
    else
    {
    //displaying critical system message with the corresponding error
        QMessageBox::critical(this, "Error", query.lastError().text());
        return;
    }

    if (!var)
    {
    //preparing the query
        query.prepare("insert into Issued_Books_Requests (Book_Name, Author_Name, Publication, Genre, Book_ID, UID) "
                      "values('" +
                      Book[0] + "','" + Book[1] + "','" + Book[2] + "','" + Book[3] + "','" + Book[4] + "','" + user + "')");

    //executing the query and checking if query executed successfully
    //if query executed
        if (query.exec())
        {
        //displaying system message
            QMessageBox::about(this, "Submission Successful!", "Issue Request has been Submitted ");

        //closing the datbase connection
            Database::closeDB();
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
        QMessageBox::warning(this, "Submission Unsuccessful!", "Request already exists! ");
    }
}

void BookDetails::updateTable()
{
//opening connection the the database
    Database::connectDB();
    model = new QSqlQueryModel();

    filter_proxy_model->setDynamicSortFilter(true);
    filter_proxy_model->setSourceModel(model);

    QSqlQuery query;

//preparing the query
    query.prepare("select * from Books");

//executing the query and checking if query executed successfully
//if query executed
    if (query.exec())
    {
    //extracting the data for further operation
        model->setQuery(query);
    //setting up extracted value in table view
        ui->tableView_bookDetails->setModel(filter_proxy_model);
        ui->tableView_bookDetails->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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

void BookDetails::on_comboBox_search_currentIndexChanged(int index)
{
    if(index == -1)
            ui->comboBox_search->setStyleSheet("color: #717072;");
        else
            ui->comboBox_search->setStyleSheet("color:white;");


    ui->lineEdit_searchBar->setReadOnly(false);

    filter_proxy_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
    filter_proxy_model->setFilterKeyColumn(index);
}


void BookDetails::on_lineEdit_searchBar_textChanged(const QString &arg1)
{
    if(arg1 == "")
            ui->lineEdit_searchBar->setStyleSheet("color: #717072;");
        else
            ui->lineEdit_searchBar->setStyleSheet("color:white;");
}

