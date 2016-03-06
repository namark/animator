#ifndef SIZEDIALOG_H
#define SIZEDIALOG_H

#include <QDialog>

namespace Ui {
    class SizeDialog;
}

class SizeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SizeDialog(QWidget *parent = 0);
    ~SizeDialog();
    void Select(unsigned char* s);

private slots:
    void on_EXIT_BTN_clicked();

    void on_lineEdit_textEdited(const QString &arg1);

private:
    Ui::SizeDialog *ui;
    void paintEvent(QPaintEvent *p);
    void mousePressEvent(QMouseEvent *m);
    unsigned char* size;
    QRect sizeRects[7];
    unsigned char sizes[7];
};

#endif // SIZEDIALOG_H
