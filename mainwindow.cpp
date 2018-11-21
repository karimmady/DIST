#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QString>
#include<string>
#include<iostream>
#include "ServiceDirectory.h"
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pix("/home/karim/Desktop/DIST/b.jpg");
    ui->pic->setPixmap(pix.scaled(361,281));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connect_clicked()
{
    QString qarg1=ui->port->text();
    string arg1 = qarg1.toUtf8().constData();
    QString qarg2=ui->myip->text();
    string arg2 = qarg2.toUtf8().constData();
    QString qarg3=ui->serversip->text();
    string arg3 = qarg3.toUtf8().constData();
    char * po;
    char * _myAddr=new char[arg2.length() + 1];
    strcpy(_myAddr, arg2.c_str());
    char* _peerAddr=new char[arg3.length() + 1];
    strcpy(_peerAddr, arg3.c_str());
    char* c=new char[arg1.length() + 1];
    strcpy(c, arg1.c_str());
    int myip=inet_addr(_myAddr);
    int port=strtol(c,&po,10);
    ServiceDirectory SD(ntohl(myip),port);
    SD.Listen();
    SD.Listen();
    SD.Listen();
    //mesh shayef el classes
}
