#include "home.h"
#include "ui_home.h"
#include<string>
#include<QString>
#include<QMessageBox>
#include<map>
#include<vector>
#include <thread>
#include"view.h"
#include"steg.h"
#include<stdio.h>
using namespace std;

void call_server(Peer &server)
{
    server.rec();
}

home::home(Peer &cspeer,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::home),
    cpeer(cspeer)
{
    class thread server(call_server,ref(cpeer));
    server.detach();

    ui->setupUi(this);
    QString ops="0 - Inquire About Pictures at User.\n1 - Request Picture from User.\n2 - Change view count at certain user.\n3 - View current counter at certain user.\n4 - View Picture.\n";
    ui->opcodes->addItem(ops);
    cout << "Home\n";
    QPixmap pix("/home/karim/Desktop/DIST/c.jpg");
    ui->pic->setPixmap(pix.scaled(511,211));

    map <string,struct sockaddr_in> onlineuser_adds=cpeer.CheckOnlineFirst();
    for(auto it:onlineuser_adds)
        ui->users->addItem(QString::fromStdString(it.first+"\n"));
    SentPictures=cpeer.CheckSentPictures();
    ReceivedPictures=cpeer.CheckReceievedPictures();
    for(auto it:ReceivedPictures)
               ui->rec->addItem(QString::fromStdString("Picture : " + it.first.second + " ,From User : "+ it.first.first + "\n"));
    for(auto it:SentPictures)
       ui->sent->addItem(QString::fromStdString("Picture : " + it.first.second + " ,From User : "+ it.first.first + "\n"));


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
    ui->op->clear();
    ui->username->clear();
    ui->Image->clear();
    ui->Views->clear();
    int z;
    int v;
    try {
        z=stoi(u);
    } catch (exception &e) {
        QMessageBox::warning(this,"Error", "Please enter op code");
    }

    if(z == 0)							//Inquire
    {
          ui->nameofowner->clear();
          ui->imagesofuser->clear();
         if(uname=="")
             QMessageBox::warning(this,"Error", "Please enter username");
         else{
             vector<string>pics;
             try{
             pics=cpeer.Inquire(uname);}
             catch(exception e)
             {
                 cout << e.what()<< endl;
             }
             ui->nameofowner->setText(qarg2);
             for(int i=0;i<pics.size();i++)
                 ui->imagesofuser->addItem(QString::fromStdString(pics[i]));
                ui->imagesofuser->addItem(QString::fromStdString("\n"));

         }
    }

    else if(z == 1)       //Request Picture
    {
        if(uname=="")
           QMessageBox::warning(this,"Error", "Please enter username");
        else if(filename=="")
           QMessageBox::warning(this,"Error", "Please enter File name");
        else if(views=="")
            QMessageBox::warning(this,"Error", "Please enter views");
        else  {
            try{
            cpeer.req(uname,filename,views);}
            catch(exception e){
               cout << e.what() << endl;}
        }
    }
    else if(z == 2) //Change Count
    {
        if(uname=="")
           QMessageBox::warning(this,"Error", "Please enter username");
        else if(filename=="")
           QMessageBox::warning(this,"Error", "Please enter File name");
        else if(views=="")
            QMessageBox::warning(this,"Error", "Please enter views");
        else{
            try{
            cpeer.ControlAccess(uname,filename,views);}
            catch(exception e)
            {
                cout << e.what() << endl;
            }
        }
    }
    else if(z == 3)  //View Count
     {
        if(uname=="")
           QMessageBox::warning(this,"Error", "Please enter username");
        else if(filename=="")
           QMessageBox::warning(this,"Error", "Please enter File name");
        else{
            int views;
            try{
           views=cpeer.ViewCount(uname,filename);
            }
            catch(exception e)
            {
                cout << e.what() << endl;
            }
           if(views==-1)
               QMessageBox::warning(this,"Error", "User is not online");
           else
           {
               string s=to_string(views);
               QMessageBox::information(this, "Views",QString::fromStdString("Views = "+s));
           }

        }
     }
    else if(z==4)
    {
        if(uname=="")
           QMessageBox::warning(this,"Error", "Please enter username");
        else if(filename=="")
           QMessageBox::warning(this,"Error", "Please enter File name");
        else
        {
            string outname;
            steg stegan;
            try{
                outname=stegan.viewpic(filename,uname);
            }
            catch(exception e)
            {
                cout << e.what() << endl;
            }
            if(outname=="Unauthorized")
                QMessageBox::warning(this,"Error", "Unauthorized");
            else
            {
            string path="/home/karim/Desktop/build-DIST-Desktop_Qt_5_11_2_GCC_64bit-Debug/"+outname;
            cout<<path<<endl;
            view *w=new view(path,this);
            w->show();
            int s=remove(outname.c_str());

            }
        }


    }
    else
        QMessageBox::warning(this,"Error", "Invalid opcode");


}

void home::on_reload_clicked()
{
    ui->users->clear();
    ui->sent->clear();
    ui->rec->clear();
    ui->imagesofuser->clear();
    cpeer.refresh();
     map <string,struct sockaddr_in> onlineuser_adds=cpeer.CheckOnlineFirst();
     for(auto it:onlineuser_adds)
         ui->users->addItem(QString::fromStdString(it.first+"\n"));
     SentPictures.clear();
     ReceivedPictures.clear();
     SentPictures=cpeer.CheckSentPictures();
     ReceivedPictures=cpeer.CheckReceievedPictures();
     for(auto it:ReceivedPictures)
                ui->rec->addItem(QString::fromStdString("Picture : " + it.first.second + " ,From User : "+ it.first.first + "\n"));
     for(auto it:SentPictures)
        ui->sent->addItem(QString::fromStdString("Picture : " + it.first.second + " ,From User : "+ it.first.first + "\n"));

}
