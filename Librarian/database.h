#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QStackedWidget>
#include <QToolButton>

class Database
{

    static QSqlDatabase librarianDb;
public:
    Database();

  //function to open connection to the database and return a boolean value
    static bool connectDB()
    {
        //finding the absolute path of the database
            QDir dir;
            QString path = dir.absoluteFilePath("Librarian.db");
            QString str1, str2;
            str1="/build-untitled-Desktop_Qt_6_1_2_MinGW_64_bit-Debug/Librarian.txt";
            str2="/Librarian/database/Librarian.db";
            path.replace(path.size() - str1.size() + 1, str1.size(), str2);

        //making a connection to the database
            librarianDb.setDatabaseName(path);

        //Checking for database connection
            if(librarianDb.open())
                return true;
            else
                return  false;;
    }

  //function to close connection to the database
    static void closeDB()
    {
        //closing the datbase connection
            librarianDb.close();

        //remove database
            //librarianDb.removeDatabase(QSqlDatabase::defaultConnection);
    }

    //function to terminate connection to the database
      static void terminateDBconn()
      {
          //remove database
              librarianDb.removeDatabase(QSqlDatabase::defaultConnection);
      }

  //function to check the database connection and reurn a QString
    static QString check_statusDB()
    {
        //Checking for database connection
            if(librarianDb.open())
                return("Status : Database Connected Successfully!  ");
            else
                return("Status : Unable to Connect to the Database  ");
    }
};

#endif // DATABASE_H
