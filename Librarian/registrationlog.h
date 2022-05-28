#ifndef REGISTRATIONLOG_H
#define REGISTRATIONLOG_H

#include <QWidget>
#include <database.h>

namespace Ui {
class RegistrationLog;
}

class RegistrationLog : public QWidget
{
    Q_OBJECT

public:
    explicit RegistrationLog(QString account, QString user, QWidget *parent = nullptr);
    ~RegistrationLog();

private slots:
    void on_comboBox_search_currentIndexChanged(int index);

    void on_lineEdit_searchBar_textChanged(const QString &arg1);

private:
    Ui::RegistrationLog *ui;

    QSqlQueryModel *model;
    QSortFilterProxyModel *filter_proxy_model;

    QString account;
    QString user;
};

#endif // REGISTRATIONLOG_H
