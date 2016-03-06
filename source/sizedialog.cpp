#include "sizedialog.h"
#include "ui_sizedialog.h"
#include <QPainter>
#include <QMouseEvent>
SizeDialog::SizeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SizeDialog)
{
    ui->setupUi(this);
}

SizeDialog::~SizeDialog()
{
    delete ui;
}

void SizeDialog::paintEvent(QPaintEvent *p)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setWidth(*size);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);
    painter.drawPoint(127,127);
    for(int i=0;i<7;i++)
    {
        pen.setWidth(sizes[i]);
        painter.setPen(pen);
        painter.drawPoint(sizeRects[i].center());
    }
}

void SizeDialog::on_EXIT_BTN_clicked()
{
    close();
}

void SizeDialog::Select(unsigned char *s)
{
    this->setWindowIcon(QIcon(":/icons/icons/Paint.png"));
    size=s;
    sizeRects[0].setRect(275,35,81,30);
    for(int i=1;i<7;i++)
    {
        sizeRects[i].setLeft(275);
        sizeRects[i].setRight(355);
        sizeRects[i].setTop(sizeRects[i-1].top()+30);
        sizeRects[i].setBottom(sizeRects[i-1].bottom()+30);
    }
    sizes[0]=0;
    for(int i=1;i<7;i++) sizes[i]=sizes[i-1]+4;
}

void SizeDialog::on_lineEdit_textEdited(const QString &arg1)
{
    *size=arg1.toLatin1().toInt();
    repaint();
}

void SizeDialog::mousePressEvent(QMouseEvent *m)
{
    for(int i=0;i<7;i++)
    {
        if(sizeRects[i].contains(m->pos()))
        {
            *size=sizes[i];
            QString s;
            s.setNum(*size);
            ui->lineEdit->setText(s);
            repaint();
        }
    }
}
