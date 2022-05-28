#include "admin_messages.h"
#include "ui_admin_messages.h"

Admin_Messages::Admin_Messages(QString user, QWidget *parent) : QWidget(parent),
                                                                ui(new Ui::Admin_Messages)
{
    ui->setupUi(this);

//Deleting the Form instance when window is closed
    this->setAttribute(Qt::WA_DeleteOnClose);

    this->user = user;

    ui->stackedWidget_message->setCurrentIndex(0);

    filter_proxy_model = new QSortFilterProxyModel();

    connect(ui->lineEdit_searchBar,SIGNAL(textChanged(QString)),filter_proxy_model,SLOT(setFilterFixedString(QString)));

    ui->comboBox_search->addItem("Date");
    ui->comboBox_search->addItem("UID");
    ui->comboBox_search->addItem("Subject");

    ui->lineEdit_searchBar->setReadOnly(true);
}

Admin_Messages::~Admin_Messages()
{
    delete ui;
}

void Admin_Messages::on_pushButton_messageReset_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Reset Message", "Do You Want To Reset the Fields?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        ui->lineEdit_messageTo->setText("");
        ui->lineEdit_messageSubject->setText("");
        ui->textEdit_messageBody->setText("");
    }
}

void Admin_Messages::on_toolButton_sentMessg_clicked()
{

    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning(this, "Warning", "Switching the Window will Reset the Fields. Do you want to switch?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        ui->lineEdit_messageTo->setText("");
        ui->lineEdit_messageSubject->setText("");
        ui->textEdit_messageBody->setText("");

        updateTable();

        ui->stackedWidget_message->setCurrentIndex(1);
    }
}

void Admin_Messages::on_toolButton_back_clicked()
{
    ui->stackedWidget_message->setCurrentIndex(1);
}

void Admin_Messages::on_toolButton_sendMssg_clicked()
{
    ui->stackedWidget_message->setCurrentIndex(0);
}

void Admin_Messages::on_pushButton_messageSend_clicked()
{
    QString uid, subject, content;
    uid = ui->lineEdit_messageTo->text();
    subject = ui->lineEdit_messageSubject->text();
    content = ui->textEdit_messageBody->toPlainText();

//imposing constraints on the input fields and displaying warning messages accordingly
//if the form is empty
    if (uid == "" && content == "")
        QMessageBox::warning(this, "Warning!", "To and Content fields are Mandatory! ");

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
        query.prepare("select * from Members where UID = '" + uid + "'");

    //executing the query and checking if query executed successfully
    //if query executed
        if (query.exec())
        {
        //checking if admin exists
            bool var = false;
            if (query.next())
                var = true;

        //if match found
            if (var)
            {

                QSqlQuery query2;
                QString date = QDate::currentDate().toString();
                QString time = QTime::currentTime().toString();
            //preparing the query
                query.prepare("insert into Messages_Admin (Date, Time, Username, UID, Subject, Content) "
                              "values('" +
                              date + "','" + time + "','" + user + "','" + uid + "','" + subject + "','" + content + "')");

            //preparing the query
                query2.prepare("insert into Messages_Member (Date, Time, Username, UID, Subject, Content) "
                               "values('" +
                               date + "','" + time + "','" + user + "','" + uid + "','" + subject + "','" + content + "')");

            //executing the query and checking if query executed successfully
            //if query executed
                if (query.exec() && query2.exec())
                {
                //displaying system message
                    QMessageBox::about(this, "Successful!", "Message Sent Successfully! ");

                //closing the datbase connection
                    Database::closeDB();

                    ui->lineEdit_messageTo->setText("");
                    ui->lineEdit_messageSubject->setText("");
                    ui->textEdit_messageBody->setText("");
                }

            //if query does not execute
                else
                {
                //displaying the critical message dialog with the corresponding error
                    QMessageBox::critical(this, "Error : ", query.lastError().text());
                }
            }

        //if match not found
            else
            {
            //displaying system warning
                QMessageBox::warning(this, "Unsuccessful!", "Member Does not exists! ");
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

void Admin_Messages::on_pushButton_delete_clicked()
{
    QModelIndex tblIndex = ui->tableView_messageLog->currentIndex();
    QModelIndex dateIndex = tblIndex.sibling(tblIndex.row(), 0);
    QModelIndex timeIndex = tblIndex.sibling(tblIndex.row(), 1);
    QString date = ui->tableView_messageLog->model()->data(dateIndex).toString();
    QString time = ui->tableView_messageLog->model()->data(timeIndex).toString();

    if (date == "")
    {
        QMessageBox::warning(this, "Delete Message", "No Message Selected! ");
        return;
    }

    Database::connectDB();

    QSqlQuery query;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete Message", "Do You Want To Delete Message for Everyone?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (reply == QMessageBox::Yes)
    {
        QSqlQuery query2;
        query.prepare("delete from Messages_Admin where Date = '" + date + "' and Time = '" + time + "'");
        query2.prepare("delete from Messages_Member where Date = '" + date + "' and Time = '" + time + "'");
    //executing the query and checking if query executed successfully
    //if query executed
        if (query.exec() && query2.exec())
        {
            updateTable();
            QMessageBox::about(this, "Delete Message", "Message Deleted Successfully!");
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

    else if (reply == QMessageBox::No)
    {
        query.prepare("delete from Messages_Admin where Date = '" + date + "' and Time = '" + time + "'");
    //executing the query and checking if query executed successfully
    //if query executed
        if (query.exec())
        {
            updateTable();
            QMessageBox::about(this, "Delete Message", "Message Deleted Successfully!");
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

void Admin_Messages::on_pushButton_view_clicked()
{
    QModelIndex tblIndex = ui->tableView_messageLog->currentIndex();
    QModelIndex dateIndex = tblIndex.sibling(tblIndex.row(), 0);
    QModelIndex timeIndex = tblIndex.sibling(tblIndex.row(), 1);
    QString date = ui->tableView_messageLog->model()->data(dateIndex).toString();
    QString time = ui->tableView_messageLog->model()->data(timeIndex).toString();

    if (date == "")
    {
        QMessageBox::warning(this, "View Message", "No Message Selected! ");
        return;
    }

    Database::connectDB();

    QSqlQuery query;
    query.prepare("select * from Messages_Admin where Date = '" + date + "' and Time = '" + time + "'");
//executing the query and checking if query executed successfully
//if query executed
    if (query.exec())
    {
        query.next();
        ui->label_date->setText("Date : " + date);
        ui->label_time->setText("Time : " + time);
        ui->label_toUID->setText("To : " + query.value(3).toString());
        ui->label_subject->setText("Subject : " + query.value(4).toString());
        ui->textBrowser_mssgBody->setText("Content : " + query.value(5).toString());
    //closing the database connection
        Database::closeDB();
    }

//if query not executed
    else
    {
    //displaying critical system message with the corresponding error
        QMessageBox::critical(this, "Error", query.lastError().text());
    }

    ui->stackedWidget_message->setCurrentIndex(2);
}

void Admin_Messages::updateTable()
{
//opening connection the the database
    Database::connectDB();
    model = new QSqlQueryModel();

    filter_proxy_model->setDynamicSortFilter(true);
    filter_proxy_model->setSourceModel(model);


    QSqlQuery query;

//preparing the query
    query.prepare("select Date, Time, UID as MemberUID, Subject from Messages_Admin where Username = '" + user + "'");

//executing the query and checking if query executed successfully
//if query executed
    if (query.exec())
    {
    //extracting the data for further operation
        model->setQuery(query);
    //setting up extracted value in table view
        ui->tableView_messageLog->setModel(filter_proxy_model);
        ui->tableView_messageLog->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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

void Admin_Messages::on_comboBox_search_currentIndexChanged(int index)
{
    if(index == -1)
            ui->comboBox_search->setStyleSheet("color: #717072;");
    else
            ui->comboBox_search->setStyleSheet("color:white;");


    ui->lineEdit_searchBar->setReadOnly(false);

    if(index>0)
        index += 1;
    filter_proxy_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
    filter_proxy_model->setFilterKeyColumn(index);
}


void Admin_Messages::on_lineEdit_messageTo_textChanged(const QString &arg1)
{
    if(arg1 == "")
            ui->lineEdit_messageTo->setStyleSheet("color: #717072;");
        else
            ui->lineEdit_messageTo->setStyleSheet("color:white;");
}


void Admin_Messages::on_lineEdit_messageSubject_textChanged(const QString &arg1)
{
    if(arg1 == "")
            ui->lineEdit_messageSubject->setStyleSheet("color: #717072;");
        else
            ui->lineEdit_messageSubject->setStyleSheet("color:white;");
}


void Admin_Messages::on_lineEdit_searchBar_textChanged(const QString &arg1)
{
    if(arg1 == "")
            ui->lineEdit_searchBar->setStyleSheet("color: #717072;");
        else
            ui->lineEdit_searchBar->setStyleSheet("color:white;");
}

