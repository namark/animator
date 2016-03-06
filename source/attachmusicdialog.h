#ifndef ATTACHMUSICDIALOG_H
#define ATTACHMUSICDIALOG_H

#include <QDialog>

namespace Ui {
    class AttachMusicDialog;
}

class AttachMusicDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AttachMusicDialog(QWidget *parent = 0);
    ~AttachMusicDialog();

    char* get();

private slots:
    void on_ATTACH_BTN_clicked();

    void on_CANCEL_BTN_clicked();

private:
    Ui::AttachMusicDialog *ui;
};

#endif // ATTACHMUSICDIALOG_H
