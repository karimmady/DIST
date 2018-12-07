#ifndef UPLOAD_WIN_H
#define UPLOAD_WIN_H

#include <QDialog>

namespace Ui {
class upload_win;
}

class upload_win : public QDialog
{
    Q_OBJECT

public:
    explicit upload_win(QWidget *parent = nullptr);
    ~upload_win();

private slots:
    void on_Upload_clicked();

private:
    Ui::upload_win *ui;
};

#endif // UPLOAD_WIN_H
