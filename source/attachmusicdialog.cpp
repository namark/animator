#include "attachmusicdialog.h"
#include "ui_attachmusicdialog.h"

AttachMusicDialog::AttachMusicDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AttachMusicDialog)
{
    ui->setupUi(this);
}

AttachMusicDialog::~AttachMusicDialog()
{
    delete ui;
}

void AttachMusicDialog::on_ATTACH_BTN_clicked()
{
    accept();
}

void AttachMusicDialog::on_CANCEL_BTN_clicked()
{
    reject();
}

char * AttachMusicDialog::get()
{
    return ui->FILENAME->text().toLatin1().data();
}
