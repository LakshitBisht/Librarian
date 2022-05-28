#include "addbooks.h"
#include "ui_addbooks.h"
#include "bookdetails.h"

AddBooks::AddBooks(QWidget *parent, QString *bookDetails) : QDialog(parent),
                                                            ui(new Ui::AddBooks)
{
    ui->setupUi(this);

    if (bookDetails != nullptr)
    {
        ui->lineEdit_bookName->setText(bookDetails[0]);
        ui->lineEdit_authorName->setText(bookDetails[1]);
        ui->lineEdit_publication->setText(bookDetails[2]);
        ui->lineEdit_genre->setText(bookDetails[3]);
        ui->lineEdit_bookID->setText(bookDetails[4]);

        ui->lineEdit_bookID->setReadOnly(true);

        ui->label_title->setText("Update Book");
        ui->toolButton_icon->setIcon(QIcon(":/img/images/update_books.png"));

        ui->stackedWidget_addUpdateBooks->setCurrentIndex(1);
        this->setWindowTitle("Update Book Details");
    }

    else
    {
        ui->stackedWidget_addUpdateBooks->setCurrentIndex(0);
        this->setWindowTitle("Add Book");
    }
}

AddBooks::~AddBooks()
{
    delete ui;
}

void AddBooks::on_pushButton_cancel_clicked()
{
    this->close();
}

void AddBooks::on_pushButton_add_clicked()
{
    QString bookName, authorName, publisher, genre, bookID;
    bookName = ui->lineEdit_bookName->text();
    authorName = ui->lineEdit_authorName->text();
    publisher = ui->lineEdit_publication->text();
    genre = ui->lineEdit_genre->text();
    bookID = ui->lineEdit_bookID->text();

//imposing constraints on the input fields and displaying warning messages accordingly
//if the form is empty
    if (bookName == "" && authorName == "" && publisher == "" && genre == "" && bookID == "")
        QMessageBox::warning(this, "Warning!", "All Fields are Mandatory! ");

//if book name field is empty
    else if (bookName == "")
        QMessageBox::warning(this, "Warning", "Book Name cannot be Empty! ");

//if author name field is empty
    else if (authorName == "")
        QMessageBox::warning(this, "Warning", "Author Name cannot be Empty! ");

//if publication is not selected
    else if (publisher == "")
        QMessageBox::warning(this, "Warning", "Please Enter Publication Name ");

//if genre is selected
    else if (genre == "")
        QMessageBox::warning(this, "Warning", "Please Select the Genre ");

//if BookiD field is empty
    else if (bookID == "")
        QMessageBox::warning(this, "Warning", "Username cannot be Empty! ");

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
        query.prepare("select * from Books where Book_ID = '" + bookID + "'");

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
                query.prepare("insert into Books (Book_Name, Author_Name, Publication, Genre, Book_ID, Status) "
                              "values('" +
                              bookName + "','" + authorName + "','" + publisher + "','" + genre + "','" + bookID + "','Available')");

            //executing the query and checking if query executed successfully
            //if query executed
                if (query.exec())
                {
                //displaying system message
                    QMessageBox::about(this, "Successful!", "Book Added Successfully! ");

                //closing the datbase connection
                    Database::closeDB();

                    ((BookDetails *)parent())->updateTable();
                //closing the signup window
                    this->close();
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
                QMessageBox::warning(this, "Unsuccessful!", "Book already exists! ");
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

void AddBooks::on_pushButton_update_clicked()
{
    QString bookName, authorName, publisher, genre, bookID;
    bookName = ui->lineEdit_bookName->text();
    authorName = ui->lineEdit_authorName->text();
    publisher = ui->lineEdit_publication->text();
    genre = ui->lineEdit_genre->text();
    bookID = ui->lineEdit_bookID->text();

//imposing constraints on the input fields and displaying warning messages accordingly
//if the form is empty
    if (bookName == "" && authorName == "" && publisher == "" && genre == "")
        QMessageBox::warning(this, "Warning!", "All Fields are Mandatory! ");

//if book name field is empty
    else if (bookName == "")
        QMessageBox::warning(this, "Warning", "Book Name cannot be Empty! ");

//if author name field is empty
    else if (authorName == "")
        QMessageBox::warning(this, "Warning", "Author Name cannot be Empty! ");

//if publication is not selected
    else if (publisher == "")
        QMessageBox::warning(this, "Warning", "Please Enter Publication Name ");

//if genre is selected
    else if (genre == "")
        QMessageBox::warning(this, "Warning", "Please Select the Genre ");

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
        query.prepare("update Books set Book_Name = '" + bookName + "', Author_Name = '" + authorName + "', Publication = '" + publisher + "', Genre = '" + genre + "' where Book_ID = '" + bookID + "'");

    //executing the query and checking if query executed successfully
    //if query executed
        if (query.exec())
        {
        //displaying system message
            QMessageBox::about(this, "Successful!", "Book Data Updated Successfully! ");

        //closing the datbase connection
            Database::closeDB();

            ((BookDetails *)parent())->updateTable();
        //closing the signup window
            this->close();
        }

    //if query does not execute
        else
        {
        //displaying the critical message dialog with the corresponding error
            QMessageBox::critical(this, "Error : ", query.lastError().text());
        }
    }
}

void AddBooks::on_lineEdit_bookName_textChanged(const QString &arg1)
{
    if(arg1 == "")
            ui->lineEdit_bookName->setStyleSheet("color: #717072;");
        else
            ui->lineEdit_bookName->setStyleSheet("color:white;");
}


void AddBooks::on_lineEdit_authorName_textChanged(const QString &arg1)
{
    if(arg1 == "")
            ui->lineEdit_authorName->setStyleSheet("color: #717072;");
        else
            ui->lineEdit_authorName->setStyleSheet("color:white;");
}


void AddBooks::on_lineEdit_publication_textChanged(const QString &arg1)
{
    if(arg1 == "")
            ui->lineEdit_publication->setStyleSheet("color: #717072;");
        else
            ui->lineEdit_publication->setStyleSheet("color:white;");
}


void AddBooks::on_lineEdit_genre_textChanged(const QString &arg1)
{
    if(arg1 == "")
            ui->lineEdit_genre->setStyleSheet("color: #717072;");
        else
            ui->lineEdit_genre->setStyleSheet("color:white;");
}


void AddBooks::on_lineEdit_bookID_textChanged(const QString &arg1)
{
    if(arg1 == "")
            ui->lineEdit_bookID->setStyleSheet("color: #717072;");
        else
            ui->lineEdit_bookID->setStyleSheet("color:white;");
}

