#ifndef HOME_H
#define HOME_H

#include <QDialog>
#include"Peer.h"
namespace Ui {
class home;
}

class home : public QDialog
{
    Q_OBJECT

public:
    explicit home(Peer &cspeer,QWidget *parent = nullptr);
    ~home();

private slots:
    void on_submit_clicked();

private:
    Ui::home *ui;
    Peer cpeer;
};

#endif // HOME_H
