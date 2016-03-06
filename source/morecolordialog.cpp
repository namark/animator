#include "morecolordialog.h"
#include "ui_morecolordialog.h"

#include <QPainter>
#include <QMouseEvent>

MoreColorDialog::MoreColorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MoreColorDialog)
{
    ui->setupUi(this);
}

MoreColorDialog::~MoreColorDialog()
{
    delete ui;
}

void MoreColorDialog::Init(QColor *c, QAction* ac)
{
    QIcon icon(":/icons/icons/Chart.png");
    this->setWindowIcon(icon);
    ui->toolButton->setDefaultAction(ac);
    color=c;
    fixed_color='r';
    main_col_rect.setRect(10,10,256,256);
    main_col=QImage(256,256,QImage::Format_RGB32);
    slide_col=QImage(20,256,QImage::Format_RGB32);

    selected_color_rect.setRect(316,226,60,30);
    color_selecting_rect.setRect(color->blue()+9,color->green()+9,2,2);
    slide_col_rect.setRect(276,10,20,256);
    slider_rect.setRect(slide_col_rect.left()-1,color->red()+9,slide_col_rect.width()+2,2);
    changed=true;
    LMB=false;
}

void MoreColorDialog::setup()
{
    int x,y,r;
    switch(fixed_color)
    {
    case 'r':
        x=color->blue()+9;
        y=color->green()+9;
        r=color->red()+9;
        break;
    case 'g':
        x=color->blue()+9;
        y=color->red()+9;
        r=color->green()+9;
        break;
    case 'b':
        x=color->green()+9;
        y=color->red()+9;
        r=color->blue()+9;

    }

    color_selecting_rect.setTop(y);
    color_selecting_rect.setLeft(x);
    color_selecting_rect.setWidth(2);
    color_selecting_rect.setHeight(2);
    slider_rect.setTop(r);
    slider_rect.setHeight(2);
}


void MoreColorDialog::paintEvent(QPaintEvent *me)
{
    QPainter painter;
    unsigned int rgbcol=0xff000000, rgbcol_slider=0xff000000;
    if(changed)
    {
        switch(fixed_color)
        {
        case 'r':
            rgbcol+=color->red()<<16;
            for(unsigned int i=0;i<256;++i)
            {
                for(int j=0;j<20;++j)
                    slide_col.setPixel(j,i,rgbcol_slider+(i<<16));
                for(unsigned int j=0;j<256;++j)
                {
                    rgbcol= rgbcol & 0xffff0000;
                    rgbcol+=i<<8;
                    rgbcol+=j;
                    main_col.setPixel(j,i,rgbcol);
                }
            }
            break;
        case 'g':
            rgbcol+=color->green()<<8;
            for(unsigned int i=0;i<256;++i)
            {
                for(int j=0;j<20;++j)
                    slide_col.setPixel(j,i,rgbcol_slider+(i<<8));
                for(unsigned int j=0;j<256;++j)
                {
                    rgbcol= rgbcol & 0xff00ff00;
                    rgbcol+=i<<16;
                    rgbcol+=j;
                    main_col.setPixel(j,i,rgbcol);
                }
            }
            break;
        case 'b':
            rgbcol+=color->blue();
            for(unsigned int i=0;i<256;++i)
            {
                for(int j=0;j<20;++j)
                    slide_col.setPixel(j,i,rgbcol_slider+i);
                for(unsigned int j=0;j<256;++j)
                {
                    rgbcol= rgbcol & 0xff0000ff;
                    rgbcol+=i<<16;
                    rgbcol+=j<<8;
                    main_col.setPixel(j,i,rgbcol);
                }
            }
            changed=false;
        }
    }
    painter.begin(this);
    painter.drawImage(main_col_rect,main_col);
    painter.drawImage(slide_col_rect,slide_col);

    QPen pen(QColor(255,255,255));
    painter.setPen(pen);
    painter.drawRect(slider_rect);
    painter.drawRect(color_selecting_rect);
    pen.setColor(QColor(0,0,0));
    painter.setPen(pen);
    painter.drawRect(color_selecting_rect.left()-1,color_selecting_rect.top()-1,color_selecting_rect.width()+2,color_selecting_rect.height()+2);
    painter.drawRect(ui->CUR_COLOR_RECT->geometry());
    QBrush b(*color);
    painter.fillRect(ui->CUR_COLOR_RECT->geometry(),b);


}

void MoreColorDialog::mousePressEvent(QMouseEvent *me)
{
    if(me->button()==Qt::LeftButton)LMB=true;
}

void MoreColorDialog::mouseMoveEvent(QMouseEvent *me)
{
    if(LMB)
    {
        if(main_col_rect.contains(me->pos()))
        {
            switch(fixed_color)
            {
            case 'r':
                color->setGreen(me->pos().y()-main_col_rect.top());
                color->setBlue(me->pos().x()-main_col_rect.left());
                break;
            case 'g':
                color->setRed(me->pos().y()-main_col_rect.top());
                color->setBlue(me->pos().x()-main_col_rect.left());
                break;
            case 'b':
                color->setRed(me->pos().y()-main_col_rect.top());
                color->setGreen(me->pos().x()-main_col_rect.left());
            }
            setup();
            repaint();
        }
        if(slide_col_rect.contains(me->pos()))
        {
            switch(fixed_color)
            {
            case 'r':
                color->setRed(me->pos().y()-slide_col_rect.top());
                break;
            case 'g':
                color->setGreen(me->pos().y()-slide_col_rect.top());
                break;
            case 'b':
                color->setBlue(me->pos().y()-slide_col_rect.top());
            }
            changed=true;
            setup();
            repaint();
        }
    }
}

void MoreColorDialog::mouseReleaseEvent(QMouseEvent *me)
{
    if(me->button()==Qt::LeftButton) LMB=false;
}

void MoreColorDialog::setChanged()
{
    changed=true;
}

void MoreColorDialog::on_radioButton_clicked()
{
    fixed_color='r';
    setup();
    changed=true;
    repaint();
}

void MoreColorDialog::on_radioButton_2_clicked()
{
    fixed_color='g';
    setup();
    changed=true;
    repaint();
}

void MoreColorDialog::on_radioButton_3_clicked()
{
    fixed_color='b';
    setup();
    changed=true;
    repaint();
}


void MoreColorDialog::on_CLOSE_BTN_clicked()
{
    close();
}

void MoreColorDialog::select(QColor *c)
{
    color=c;
}

void MoreColorDialog::closeEvent(QCloseEvent *ce)
{
    this->parentWidget()->repaint();
}



void MoreColorDialog::setColor(QColor col)
{
    *color=col;
    setup();
    repaint();
}

