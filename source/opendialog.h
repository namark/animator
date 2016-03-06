#ifndef OPENDIALOG_H
#define OPENDIALOG_H

#include <QDialog>

namespace Ui {
    class OpenDialog;
}

class OpenDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenDialog(QWidget *parent = 0);
    ~OpenDialog();
    void Init(char **fn);



private slots:
    void on_OPEN_BTN_clicked();

    void on_CANCEL_BTN_clicked();

    void on_FILENAME_textEdited(const QString &arg1);

private:
    Ui::OpenDialog *ui;
    QByteArray d;
    char **pointer_toFN;

    void showEvent(QShowEvent *se);
};

#endif // OPENDIALOG_H
