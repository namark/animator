#ifndef SAVEASDIALOG_H
#define SAVEASDIALOG_H

#include <QDialog>

namespace Ui {
    class SaveAsDialog;
}

class SaveAsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveAsDialog(QWidget *parent = 0);
    ~SaveAsDialog();
    void Init(char **fn);

private slots:
    void on_CANCEL_BTN_clicked();

    void on_SAVE_BTN_clicked();

    void on_FILENAME_textEdited(const QString &arg1);

private:
    Ui::SaveAsDialog *ui;
    char** pointer_toFN;
    QByteArray d;

    void showEvent(QShowEvent *se);
};

#endif // SAVEASDIALOG_H
