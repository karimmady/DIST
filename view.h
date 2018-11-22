#ifndef VIEW_H
#define VIEW_H

#include <QDialog>
#include<map>
using namespace std;
namespace Ui {
class view;
}

class view : public QDialog
{
    Q_OBJECT

public:
    explicit view(map< pair <string,string>, string > ReceivedPictures,map< pair <string,string>, int > SentPictures,QWidget *parent = nullptr);
    ~view();

private:
    Ui::view *ui;
};

#endif // VIEW_H
