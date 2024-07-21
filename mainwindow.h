#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QtCharts/QtCharts>
#include <QFile>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void update_info();
    int count_total();
    void on_pushButton_2_clicked();

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlTableModel *model;
    double total;
    QChart *chart;
};
#endif // MAINWINDOW_H
