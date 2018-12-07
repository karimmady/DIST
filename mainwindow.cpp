#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QString>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include "Peer.h"
#include "Message.h"
#include "steg.h"
#include "base64.h"
#include <unistd.h>
#include <thread>
#include<QMessageBox>
#include"sign.h"
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pix("./b.jpg");
    ui->pic->setPixmap(pix.scaled(361,281));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connect_clicked()
{
    QString qarg1=ui->sport->text();
    string arg1 = qarg1.toUtf8().constData();
    QString qarg2=ui->myport->text();
    string arg2 = qarg2.toUtf8().constData();
    QString qarg3=ui->myip->text();
    string arg3 = qarg3.toUtf8().constData();
    QString qarg4=ui->serversip->text();
    string arg4 = qarg4.toUtf8().constData();

    char * server_po;
    char * client_po;
    bool x;
    int myip=inet_addr(arg3.c_str());
    int directoryserviceip=inet_addr(arg4.c_str());
    int clientport=strtol(arg2.c_str(),&server_po,10);
    int serverport=strtol(arg1.c_str(),&client_po,10);
    Peer cspeer(ntohl(myip),ntohl(directoryserviceip),serverport,clientport,x);
    if(!x)
         QMessageBox::warning(this,"BIND", "bind failed");
    else
    {
        hide();
        sign *w=new sign(cspeer,this);
        w->show();
    }
}
