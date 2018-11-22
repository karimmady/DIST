#include "home.h"
#include "ui_home.h"
#include<string>
#include<QString>
#include<QMessageBox>
#include<map>
#include"view.h"
using namespace std;
home::home(Peer &cspeer,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::home),
    cpeer(cspeer)
{
    ui->setupUi(this);
    QString ops="0 - Inquire About Pictures at User.\n1 - Request Picture from User.\n2 - Change view count at certain user.\n3 - View current counter at certain user.\n4 - View Picture.\n5 - Terminate Program. \n";
    ui->opcodes->addItem(ops);
    QPixmap pix("/home/karim/Desktop/DIST/c.jpg");
    ui->pic->setPixmap(pix.scaled(511,211));
    map <string,struct sockaddr_in> onlineuser_adds=cpeer.CheckOnlineFirst();
    for(auto it:onlineuser_adds)
        ui->users->addItem(QString::fromStdString(it.first+"\n"));

}

home::~home()
{
    delete ui;
}

void home::on_submit_clicked()
{
    QString qarg1=ui->op->text();
    string u = qarg1.toUtf8().constData();
    QString qarg2=ui->username->text();
    string uname = qarg2.toUtf8().constData();
    QString qarg3=ui->Image->text();
    string filename = qarg3.toUtf8().constData();
    QString qarg4=ui->Views->text();
    string views = qarg4.toUtf8().constData();
    int z;
    int v;
    try {
        z=stoi(u);
    } catch (exception &e) {
        QMessageBox::warning(this,"Error", "Please enter op code");
    }

    if(z == 0)							//Inquire
    {

         if(uname=="")
             QMessageBox::warning(this,"Error", "Please enter username");
         else
             cpeer.Inquire(uname);
    }

    else if(z == 1)       //Request Picture
    {
        if(uname=="")
           QMessageBox::warning(this,"Error", "Please enter username");
        else if(filename=="")
           QMessageBox::warning(this,"Error", "Please enter File name");
        else if(views=="")
            QMessageBox::warning(this,"Error", "Please enter views");
        else
            cpeer.req(uname,filename,views);
    }
    else if(z == 2) //Change Count
    {
        map< pair <string,string>, string > ReceivedPictures;
        ReceivedPictures=cpeer.CheckReceievedPictures();
        map< pair <string,string>, int > SentPictures;
        SentPictures=cpeer.CheckSentPictures();
        view *v=new view(ReceivedPictures,SentPictures,this);
        v->show();
        if(uname=="")
           QMessageBox::warning(this,"Error", "Please enter username");
        else if(filename=="")
           QMessageBox::warning(this,"Error", "Please enter File name");
        else if(views=="")
            QMessageBox::warning(this,"Error", "Please enter views");
        else
            cpeer.ControlAccess(uname,filename,views);
    }
    else if(z == 3)  //View Count
     {
        map< pair <string,string>, string > ReceivedPictures;
        ReceivedPictures=cpeer.CheckReceievedPictures();
        map< pair <string,string>, int > SentPictures;
        SentPictures=cpeer.CheckSentPictures();
        view *v=new view(ReceivedPictures,SentPictures,this);
        v->show();
        if(uname=="")
           QMessageBox::warning(this,"Error", "Please enter username");
        else if(filename=="")
           QMessageBox::warning(this,"Error", "Please enter File name");
        else
           cpeer.ViewCount(uname,filename);
     }

}

void home::on_reload_clicked()
{
    ui->users->clear();
     map <string,struct sockaddr_in> onlineuser_adds=cpeer.CheckOnlineFirst();
     for(auto it:onlineuser_adds)
         ui->users->addItem(QString::fromStdString(it.first+"\n"));
}
