#ifndef VIEW_H
#define VIEW_H

#include <QDialog>
#include<map>
#include<string>
using namespace std;
namespace Ui {
class view;
}

class view : public QDialog
{
    Q_OBJECT

public:
    explicit view(string path,QWidget *parent = nullptr);
    ~view();

private:
    Ui::view *ui;
};

#endif // VIEW_H
