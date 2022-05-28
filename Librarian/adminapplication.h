#ifndef ADMINAPPLICATION_H
#define ADMINAPPLICATION_H

#include <QWidget>
#include "database.h"

namespace Ui {
class AdminApplication;
}

class AdminApplication : public QWidget
{
    Q_OBJECT

public:
    explicit AdminApplication(QToolButton *iconButton = nullptr, QWidget *parent = nullptr);
    ~AdminApplication();

    bool deleteApplication(QString username);
    void updateIcon();


private slots:
    void on_pushButton_applicationAccept_clicked();

    void on_pushButton_applicationReject_clicked();

    void on_comboBox_search_currentIndexChanged(int index);

    void on_lineEdit_searchBar_textChanged(const QString &arg1);

private:
    Ui::AdminApplication *ui;

    QToolButton *iconButton;
    QSqlQueryModel * model;
    QSortFilterProxyModel *filter_proxy_model;
};

#endif // ADMINAPPLICATION_H
