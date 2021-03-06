#ifndef HOME_H
#define HOME_H

#include <QDialog>
#include"Peer.h"
#include<thread>
#include<map>
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

    void on_reload_clicked();

    void on_upload_clicked();

    void on_remove_clicked();

    void on_reloadpictures_clicked();

private:
    Ui::home *ui;
    Peer cpeer;
    map< pair <string,string>, string > ReceivedPictures;
    map< pair <string,string>, int > SentPictures;
};

#endif // HOME_H
