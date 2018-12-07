#include "upload_win.h"
#include "ui_upload_win.h"

upload_win::upload_win(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::upload_win)
{
    ui->setupUi(this);
}

upload_win::~upload_win()
{
    delete ui;
}

void upload_win::on_Upload_clicked()
{
    hide();
}
