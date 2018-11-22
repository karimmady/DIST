#include "view.h"
#include "ui_view.h"
#include<map>
#include<QString>
using namespace std;
view::view(map< pair <string,string>, string > ReceivedPictures,map< pair <string,string>, int > SentPictures,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::view)
{
    ui->setupUi(this);
    for(auto it:ReceivedPictures)
               ui->rec->addItem(QString::fromStdString("Picture : " + it.first.second + " ,From User : "+ it.first.first + "\n"));
    for(auto it:SentPictures)
       ui->sent->addItem(QString::fromStdString("Picture : " + it.first.second + " ,From User : "+ it.first.first + "\n"));

}

view::~view()
{
    delete ui;
}
