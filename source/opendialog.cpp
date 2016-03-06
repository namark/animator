#include "opendialog.h"
#include "ui_opendialog.h"

OpenDialog::OpenDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenDialog)
{
    ui->setupUi(this);
}

OpenDialog::~OpenDialog()
{
    delete ui;
}

void OpenDialog::on_OPEN_BTN_clicked()
{
    d=ui->FILENAME->text().toLatin1();
    *pointer_toFN=d.data();
    accept();
}

void OpenDialog::on_CANCEL_BTN_clicked()
{
    reject();
}

void OpenDialog::Init(char **fn)
{
    pointer_toFN=fn;
}

void OpenDialog::showEvent(QShowEvent *se)
{
    ui->OPEN_BTN->setDisabled(ui->FILENAME->text()=="");
}

void OpenDialog::on_FILENAME_textEdited(const QString &arg1)
{
    ui->OPEN_BTN->setDisabled(arg1=="");
}
