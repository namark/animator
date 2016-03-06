#include "saveasdialog.h"
#include "ui_saveasdialog.h"

SaveAsDialog::SaveAsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveAsDialog)
{
    ui->setupUi(this);
}

SaveAsDialog::~SaveAsDialog()
{
    delete ui;
}

void SaveAsDialog::on_CANCEL_BTN_clicked()
{
    reject();
}

void SaveAsDialog::on_SAVE_BTN_clicked()
{
    d=ui->FILENAME->text().toLatin1();
    *pointer_toFN=d.data();
    accept();
}

void SaveAsDialog::Init(char **fn)
{
    pointer_toFN=fn;
}

void SaveAsDialog::showEvent(QShowEvent *se)
{
    ui->SAVE_BTN->setDisabled(ui->FILENAME->text()=="");
}

void SaveAsDialog::on_FILENAME_textEdited(const QString &arg1)
{
    ui->SAVE_BTN->setDisabled(arg1=="");
}
