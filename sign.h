#ifndef SIGN_H
#define SIGN_H

#include <QDialog>
#include<Peer.h>
namespace Ui {
class sign;
}

class sign : public QDialog
{
    Q_OBJECT

public:
    explicit sign(Peer &cspeer,QWidget *parent = nullptr);
    ~sign();

private slots:
    void on_signin_clicked();

    void on_register_2_clicked();

private:
    Ui::sign *ui;
    Peer cpeer;
};

#endif // SIGN_H
