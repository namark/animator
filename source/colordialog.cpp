#include "colordialog.h"
#include "ui_colordialog.h"
#include <QPainter>
#include <QMouseEvent>
#include <stdio.h>

ColorDialog::ColorDialog(char * dir, int dir_len, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorDialog)
{
    ui->setupUi(this);
    l=dir_len;
    d=new char[l];
    memcpy(d,dir,l);
}


ColorDialog::~ColorDialog()
{
    Save();
    delete [] d;
    delete ui;
}

void ColorDialog::Select(QColor *c, QAction *ac)
{
    QIcon icon(":/icons/icons/Chart.png");
    this->setWindowIcon(icon);
    if(!Load())
    {
        int red=0, green=0, blue=0;
        for(int i=0;i<27;i++)
        {
            colors[0][i].setRgb(red,green,blue);
            blue+=125;
            if(blue==250) blue=255;
            if(blue>255)
            {
                blue=0;
                green+=125;
                if(green==250) green=255;
                if(green>255)
                {
                    green=0;
                    red+=125;
                    if(red==250) red=255;
                }
            }
        }
    }
    int size=30, pad=5;
    for(int i=0;i<3;i++)
        colorRects[i][0]=QRect(10,i*(size+(pad-1))+10,size,size);
    for(int i=0;i<9;i++)
        colorRects[0][i]=QRect(i*(size+(pad-1))+10,10,size,size);
    for(int i=1;i<3;i++)
        for(int j=1;j<9;j++)
        {
            colorRects[i][j].setLeft(colorRects[i][j-1].right()+pad);
            colorRects[i][j].setTop(colorRects[i-1][j].bottom()+pad);
            colorRects[i][j].setHeight(size);
            colorRects[i][j].setWidth(size);
        }
    color=c;
    QString s;
    s.setNum(color->red());
    ui->Red->setText(s);
    s.setNum(color->green());
    ui->Green->setText(s);
    s.setNum(color->blue());
    ui->Blue->setText(s);
    cur_col_rect.setRect(130,120,40,20);
    MCD=new MoreColorDialog(this);
    MCD->Init(color,ac);
}



void ColorDialog::on_Exit_clicked()
{
    close();
}

void ColorDialog::on_Red_textEdited(const QString &arg1)
{
    color->setRed(arg1.toInt());
    repaint(cur_col_rect);
}

void ColorDialog::on_Green_textEdited(const QString &arg1)
{
    color->setGreen(arg1.toInt());
    repaint(cur_col_rect);
}

void ColorDialog::on_Blue_textEdited(const QString &arg1)
{
    color->setBlue(arg1.toInt());
    repaint(cur_col_rect);
}

void ColorDialog::paintEvent(QPaintEvent *p)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    for(int i=0;i<27;i++)
    {
        QBrush brush(colors[0][i]);
        painter.setBrush(brush);
        painter.drawEllipse(colorRects[0][i]);
    }
    QRect rec(130,120,40,20);
    QBrush b(*color);
    painter.fillRect(cur_col_rect,b);
}

void ColorDialog::mousePressEvent(QMouseEvent *m)
{
    QPoint p=m->pos();
    for(int i=0;i<27;++i)
    {
        if(colorRects[0][i].contains(p))
        {
            if(m->button()==Qt::RightButton)
            {
                MCD->select(colors[0]+i);
                MCD->setup();
                MCD->repaint();
                MCD->show();
            }
            *color=colors[0][i];
            QString s;
            s.setNum(color->red());
            ui->Red->setText(s);
            s.setNum(color->green());
            ui->Green->setText(s);
            s.setNum(color->blue());
            ui->Blue->setText(s);
            repaint(cur_col_rect);
        }
    }
    if(cur_col_rect.contains(p))
    {
        MCD->select(color);
        MCD->setup();
        MCD->repaint();
        MCD->show();
    }
}

bool ColorDialog::Load()
{
    FILE *f;
    char fn[l+11];
    memcpy(fn,d,l);
    memcpy(fn+l,"color.conf",11);
    if(!(f=fopen(fn,"rb"))) return false;
    unsigned char c[81];
    if(fread(c,1,81,f) < 81) return false;
    for(int i=0, j=0; i<27; ++i, j+=3)
    {
        colors[0][i].setRgb(c[j],c[j+1],c[j+2]);
    }
    fclose(f);
    return true;
}

void ColorDialog::Save()
{
    FILE *f;
    char fn[l+11];
    memcpy(fn,d,l);
    memcpy(fn+l,"color.conf",11);
    f=fopen(fn,"wb");
    unsigned char c[81];
    for(int i=0, j=0, t[3]; i<27; ++i, j+=3)
    {
        colors[0][i].getRgb(t,t+1,t+2);
        c[j]=t[0];
        c[j+1]=t[1];
        c[j+2]=t[2];
    }
    fwrite(c,1,81,f);
    fclose(f);
}

void ColorDialog::Update(QColor col)
{
    repaint();
    *color=col;
    MCD->setColor(col);
}
