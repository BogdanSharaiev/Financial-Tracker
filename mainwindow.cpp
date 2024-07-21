#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPixmap>
#include <QDebug>
#include <QDate>
#include <QStyledItemDelegate>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QVBoxLayout>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);






    QString tableViewStyle = R"(
    QTableView {
        gridline-color: #dcdcdc;
        font-size: 14px;
        selection-background-color: #82caff;
        selection-color: #000000;
        background-color: #ffffff;
        alternate-background-color: #f9f9f9;
        border: 1px solid #dcdcdc;
        border-radius: 8px;
    }

    QHeaderView::section {
        background-color: #5c6bc0;
        color: #ffffff;
        padding: 8px;
        border: 1px solid #dcdcdc;
        font-size: 14px;
    }

    QTableCornerButton::section {
        background-color: #5c6bc0;
        border: 1px solid #dcdcdc;
    }

    QTableView::item {
        border-bottom: 1px solid #dcdcdc;
    }

    QTableView::item:hover {
        background-color: #e0f7fa;
    }

    QTableView::item:selected {
        background-color: #82caff;
        color: #000000;
    }

    QHeaderView::section:horizontal {
        border-top-left-radius: 8px;
        border-top-right-radius: 8px;
    }

    QHeaderView::section:vertical {
        border-left: none;
    }
)";

    ui->tableView->setStyleSheet(tableViewStyle);
    ui->comboBox->addItem("Expence");
    ui->comboBox->addItem("Income");
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./fin.db");
    db.open();
    QSqlQuery qe;
    // label_4
   total = count_total();

     QString t = QString::number(total) + "$";
     ui->label_4->setText(t);
    QSqlQuery q;
// Transactions Table
    qDebug() << "Table created Transactions";
    model = new QSqlTableModel();
    model->setTable("Transactions");
    model->setSort(0, Qt::DescendingOrder);  // Сортировка по первому столбцу (id) в порядке убывания
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
    total = count_total();
    QString t = QString::number(total) + "$";
    ui->label_4->setText(t);
}


int MainWindow::count_total(){
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

    if(total>0){
        ui->label_4->setStyleSheet("font: 700 30pt 'Times New Roman'; color: rgb(0, 202, 30)");
    }
    else{
        ui->label_4->setStyleSheet("font: 700 30pt 'Times New Roman'; color: red");
    }


    return total;
}

void MainWindow::on_pushButton_2_clicked()
{
    QString res;
    for(int k = 1;k<5;k++){
        QString header = model->headerData(k,Qt::Horizontal).toString();
        res+= header + "    ";
    }
    res+="\n";
    int rows = model->rowCount();
    int columns = model->columnCount();

    for(int i = 0;i<rows;i++){
        for(int j = 1;j<columns;j++){

            if(j>0){
                res+= "    ";
            }

            res += model->data(model->index(i,j)).toString();
        }
        res+="\n";
    }

    qDebug() << res;

    QFile csvFile("test.csv");
    if(csvFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(&csvFile);
        out << res;
        csvFile.close();
    }

}

