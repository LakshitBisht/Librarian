#include "mainwindow.h"
#include "ui_mainwindow.h"

//declaring the static variable of class Database and Connecting the database
QSqlDatabase Database::librarianDb = QSqlDatabase::addDatabase("QSQLITE");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    StatusLabel = new QLabel(this);
    ui->statusbar->addPermanentWidget(StatusLabel);
    StatusLabel->setStyleSheet("color:white; background:transparent;");
    StatusLabel->setText("[+]Status    ");

    login_window = new Login(StatusLabel, this);
    ui->stackedWidget_mainWindow->insertWidget(1, login_window);

    ui->stackedWidget_mainWindow->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
//remove database
    Database::terminateDBconn();
}

void MainWindow::on_pushButtongetstarted_clicked()
{
//creating the login window
    ui->stackedWidget_mainWindow->setCurrentIndex(1);
    setWindowTitle("Librarian - Login");
}

void MainWindow::on_pushButton_clicked()
{
    QMessageBox::about(this, "About Librarian",
                       "Developers Info :\n\tLakshit Bisht\t20BCS6588\n\tRahul Banerjee\t20BCS6590"
                        "\n\nProject Info :\n\tLibrarian - Library Management Software\n"
                        "\tLanguage : C++\n\tGUI : QT\n\tDatabase : SQLite");
}

