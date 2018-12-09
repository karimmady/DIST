#include "sign.h"
#include "ui_sign.h"
#include "Peer.h"
#include<QMessageBox>
#include"home.h"
sign::sign(Peer &cspeer,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sign),
    cpeer(cspeer)
{
    ui->setupUi(this);
    QPixmap pix("/home/karim/Desktop/DIST/a.jpg");
    ui->pic->setPixmap(pix.scaled(361,211));
}

sign::~sign()
{
    delete ui;
}

void sign::on_signin_clicked()
{
    QString qarg1=ui->username->text();
    string u = qarg1.toUtf8().constData();
    QString qarg2=ui->password->text();
    string p = qarg2.toUtf8().constData();
    bool x=cpeer.AnnouncePresence(u,p);
    //bool x=true;
    if(!x)
        QMessageBox::warning(this,"Login", "Username or password is not correct");
    else{
        hide();
        home *w=new home(cpeer,this);
        w->show();
    }

}

void sign::on_register_2_clicked()
{
    QString qarg1=ui->username->text();
    string u = qarg1.toUtf8().constData();
    QString qarg2=ui->password->text();
    string p = qarg2.toUtf8().constData();
    cpeer.Register(u,p);
    on_signin_clicked();
}
