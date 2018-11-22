#include "home.h"
#include "ui_home.h"
#include<string>
#include<QString>
#include<QMessageBox>
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
        QMessageBox::warning(this,"Login", "Please enter op code");
    }

    if(z == 0)							//Inquire
    {
         cpeer.CheckOnlineFirst();
         if(uname=="")
             QMessageBox::warning(this,"Login", "Please enter username");
         else
             cpeer.Inquire(uname);
    }

    else if(z == 1)       //Request Picture
    {
        if(uname=="")
           QMessageBox::warning(this,"Login", "Please enter username");
        else if(filename=="")
           QMessageBox::warning(this,"Login", "Please enter File name");
        else if(views=="")
            QMessageBox::warning(this,"Login", "Please enter views");
        else
            cpeer.req(uname,filename,views);
    }

    else if(z == 3)  //View Count
            {
                cout << "Received Pictures : " << endl;
                cpeer.CheckReceievedPictures();
                cout << "Sent Pictures : " << endl;
                cpeer.CheckSentPictures();
                cout << "*************************" << endl;
                cout << "Please enter file name " << endl;
                cin >> filename;
                cout << "Please enter Username " << endl;
                cin >> uname;
                cpeer.ViewCount(uname,filename);
            }

}
