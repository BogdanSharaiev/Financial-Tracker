#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPixmap>
#include <QDebug>
#include <QDate>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap p(":resourses/resourse.qrc/house_logo.png");
    ui->comboBox->addItem("Expence");
    ui->comboBox->addItem("Income");
    ui->label->setPixmap(p.scaled(100,200));
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./fin.db");
    db.open();
    QSqlQuery qe;
    // label_4
     qe.prepare("SELECT id, amount, date, description, type FROM Transactions");
    qe.exec();

     total = 0;
     while(qe.next()){
        QString type = qe.value(4).toString();
         double amount = qe.value(1).toDouble();
        if(type == "Income"){
             total+=amount;
         }
        else{
            total-=amount;
        }
    }
     QString t = QString::number(total) + "$";
     ui->label_4->setText(t);
    QSqlQuery q;
// Transactions Table
    qDebug() << "Table created Transactions";
    model = new QSqlTableModel();
    model->setTable("Transactions");
    model->select();
    ui->tableView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QDate date = QDate::currentDate();
    QString cur = date.toString("yyyy-MM-dd");
    QString type = ui->comboBox->currentText();
    double value = ui->lineEdit_2->text().toDouble();
    QString description = ui->lineEdit->text();
    QSqlQuery query;
    query.prepare("INSERT INTO Transactions (amount, date, description,type) VALUES (:amount, :date, :description, :type)");
    query.bindValue(":amount", value);  // Example amount
    query.bindValue(":date", cur);  // Bind the current date
    query.bindValue(":description", description);  // Example description
    query.bindValue(":type", type);

    query.exec();

    update_info();
}

void MainWindow::update_info(){


    model->select();

    ui->tableView->setModel(model);
    QSqlQuery qe;
    // label_4
    qe.prepare("SELECT id, amount, date, description, type FROM Transactions");
    qe.exec();

    total = 0;
    while(qe.next()){
        QString type = qe.value(4).toString();
        double amount = qe.value(1).toDouble();
        if(type == "Income"){
            total+=amount;
        }
        else{
            total-=amount;
        }
    }
    QString t = QString::number(total) + "$";
    ui->label_4->setText(t);
}
