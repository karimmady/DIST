#include "view.h"
#include "ui_view.h"
#include<map>
#include<QString>
#include<QPixmap>
using namespace std;
view::view(string path,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::view)
{
    ui->setupUi(this);
    QPixmap picture(QString::fromStdString(path));
    ui->pic->setPixmap(picture.scaled(661,401));
    remove(path.c_str());
}

view::~view()
{
    delete ui;
}
