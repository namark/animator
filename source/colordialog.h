#ifndef COLORDIALOG_H
#define COLORDIALOG_H

#include <QDialog>
#include "morecolordialog.h"

namespace Ui {
    class ColorDialog;
}

class ColorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ColorDialog(char* dir=NULL, int dir_len=0, QWidget *parent = 0);
    ~ColorDialog();
    void Select(QColor* c, QAction * ac);
    void Update(QColor col);

private:
    MoreColorDialog *MCD;
    char *d;
    int l;
    QRect colorRects[3][9];
    QRect cur_col_rect;
    QColor colors[3][9];
    QColor *color;
    Ui::ColorDialog *ui;

    void paintEvent(QPaintEvent *p);
    void mousePressEvent(QMouseEvent *m);
    bool Load();
    void Save();

private slots:

    void on_Exit_clicked();
    void on_Red_textEdited(const QString &arg1);
    void on_Green_textEdited(const QString &arg1);
    void on_Blue_textEdited(const QString &arg1);
};

#endif // COLORDIALOG_H
