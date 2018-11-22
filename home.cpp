#include "home.h"
#include "ui_home.h"
#include<string>
#include<QString>
home::home(Peer &cspeer,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::home),
    cpeer(cspeer)
{
    ui->setupUi(this);
    QString ops="0 - Inquire About Pictures at User.\n1 - Request Picture from User.\n2 - Change view count at certain user.\n3 - View current counter at certain user.\n4 - View Picture.\n5 - Terminate Program. \n";
    ui->opcodes->addItem(ops);

}

home::~home()
{
    delete ui;
}

void home::on_submit_clicked()
{

}
