#include "member_messages.h"
#include "ui_member_messages.h"

Member_Messages::Member_Messages(QString user, QToolButton *iconbutton, QWidget *parent) : QWidget(parent),
                                                                                           ui(new Ui::Member_Messages)
{
    ui->setupUi(this);

//Deleting the Form instance when window is closed
    this->setAttribute(Qt::WA_DeleteOnClose);

    this->user = user;
    this->iconbutton = iconbutton;

    ui->lineEdit_searchBar->setReadOnly(true);

    ui->comboBox_search->addItem("Date");
    ui->comboBox_search->addItem("Time");
    ui->comboBox_search->addItem("Username");
    ui->comboBox_search->addItem("Subject");

    filter_proxy_model = new QSortFilterProxyModel();
    connect(ui->lineEdit_searchBar,SIGNAL(textChanged(QString)),filter_proxy_model,SLOT(setFilterFixedString(QString)));


    ui->stackedWidget_memMessages->setCurrentIndex(0);

    updateTable();
}

Member_Messages::~Member_Messages()
{
    delete ui;
}

void Member_Messages::updateTable()
{
//opening connection the the database
    Database::connectDB();
    model = new QSqlQueryModel();

    filter_proxy_model->setDynamicSortFilter(true);
    filter_proxy_model->setSourceModel(model);

    QSqlQuery query;

//preparing the query
    query.prepare("select Date, Time, Username as Admin_Username, Subject from Messages_Member where UID = '" + user + "'");

//executing the query and checking if query executed successfully
//if query executed
    if (query.exec())
    {
    //extracting the data for further operation
        model->setQuery(query);
    //setting up extracted value in table view
        ui->tableView_messages->setModel(filter_proxy_model);
        ui->tableView_messages->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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

void Member_Messages::on_pushButton_view_clicked()
{
    QModelIndex tblIndex = ui->tableView_messages->currentIndex();
    QModelIndex dateIndex = tblIndex.sibling(tblIndex.row(), 0);
    QModelIndex timeIndex = tblIndex.sibling(tblIndex.row(), 1);
    QString date = ui->tableView_messages->model()->data(dateIndex).toString();
    QString time = ui->tableView_messages->model()->data(timeIndex).toString();

    if (date == "")
    {
        QMessageBox::warning(this, "View Message", "No Message Selected! ");
        return;
    }

    Database::connectDB();

    QSqlQuery query;
    query.prepare("select * from Messages_Member where Date = '" + date + "' and Time = '" + time + "'");
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

    ui->stackedWidget_memMessages->setCurrentIndex(1);
}

void Member_Messages::on_pushButton_delete_clicked()
{
    QModelIndex tblIndex = ui->tableView_messages->currentIndex();
    QModelIndex dateIndex = tblIndex.sibling(tblIndex.row(), 0);
    QModelIndex timeIndex = tblIndex.sibling(tblIndex.row(), 1);
    QString date = ui->tableView_messages->model()->data(dateIndex).toString();
    QString time = ui->tableView_messages->model()->data(timeIndex).toString();

    if (date == "")
    {
        QMessageBox::warning(this, "Delete Message", "No Message Selected! ");
        return;
    }

    Database::connectDB();

    QSqlQuery query;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete Message", "Do You Want To Delete The Message?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        query.prepare("delete from Messages_Member where Date = '" + date + "' and Time = '" + time + "'");
    //executing the query and checking if query executed successfully
    //if query executed
        if (query.exec())
        {
            updateTable();
            QMessageBox::about(this, "Delete Message", "Message Deleted Successfully!");

            Database::connectDB();
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
                    iconbutton->setIcon(QIcon(":/img/images/emptyApplication.png"));
                }
            }

        //if query not executed
            else
            {
            //displaying critical system message with the corresponding error
                QMessageBox::critical(this, "Error", query.lastError().text());
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
}

void Member_Messages::on_toolButton_back_clicked()
{
    ui->stackedWidget_memMessages->setCurrentIndex(0);
}

void Member_Messages::on_comboBox_search_currentIndexChanged(int index)
{
    if(index == -1)
            ui->comboBox_search->setStyleSheet("color: #717072;");
        else
            ui->comboBox_search->setStyleSheet("color:white;");

    ui->lineEdit_searchBar->setReadOnly(false);

    filter_proxy_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
    filter_proxy_model->setFilterKeyColumn(index);
}


void Member_Messages::on_lineEdit_searchBar_textChanged(const QString &arg1)
{
    if(arg1 == "")
            ui->lineEdit_searchBar->setStyleSheet("color: #717072;");
        else
            ui->lineEdit_searchBar->setStyleSheet("color:white;");
}

