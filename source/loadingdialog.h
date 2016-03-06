#ifndef LOADINGDIALOG_H
#define LOADINGDIALOG_H

#include <QDialog>
#include <QPixmap>
#include <vector>
#include <QTimer>

namespace Ui {
    class LoadingDialog;
}

class LoadingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoadingDialog(char* dir=NULL, int dir_len=0, QWidget *parent = 0);
    ~LoadingDialog();

    void progress();

private:
    Ui::LoadingDialog *ui;

    std::vector<QPixmap> anisec, colocle;
    uint step;

    QTimer updateTimer;

    void paintEvent(QPaintEvent *pe);

private slots:


};

#endif // LOADINGDIALOG_H
