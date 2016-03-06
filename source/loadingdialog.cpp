#include "loadingdialog.h"
#include "ui_loadingdialog.h"
#include <cstdio>
#include <QPainter>

// :D I guess I really liked C back in the days...
LoadingDialog::LoadingDialog(char* dir,int dir_len,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadingDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);



    int flen=dir_len+12;

    char *fn=new char[flen+1];
    memcpy(fn,dir,dir_len);
    memcpy(fn+dir_len,"animator.ims",13);

    FILE *file = nullptr;
    FILE *file2 = nullptr;
    int wh[2], wh2[2];
    if(file=fopen(fn,"rb"))
    {
        fread(wh,4,2,file);
        memcpy(fn+dir_len,"new.ims",7);
        memset(fn+dir_len+7,'\0',6);
        if(file2=fopen(fn,"rb"))
        {
            fread(wh2,4,2,file2);
            setFixedSize(wh[0]+wh2[0],wh[1]>wh2[1]?wh[1]:wh2[1]);
            QRect prect = parent->rect();
            QRect geom = geometry();
            geom.moveCenter( (prect.bottomRight() - prect.topLeft())/2 );
            setGeometry(geom);
        }
    }
    delete [] fn;

    int s=(wh[0]*wh[1])<<2, s2=(wh2[0]*wh2[1])<<2;
    unsigned char * image_data1, * image_data2;
    image_data1 = new unsigned char[s];
    image_data2 = new unsigned char[s2];

    if(file && file2)
    {

        while(fread(image_data1,1,s,file))
        {
            anisec.push_back( QPixmap::fromImage(QImage(image_data1,wh[0],wh[1],QImage::Format_ARGB32)) );
        }

        while(fread(image_data2,1,s2,file2))
        {
			colocle.push_back( QPixmap::fromImage(QImage(image_data2,wh2[0],wh2[1],QImage::Format_ARGB32)) );
        }
	}

    delete [] image_data1;
    delete [] image_data2;
    if(file)fclose(file);
    if(file2) fclose(file2);

    updateTimer.setInterval(1000/60);
    connect(&updateTimer, SIGNAL(timeout()), this, SLOT(update()));
    updateTimer.start();

    step = 0;
}

LoadingDialog::~LoadingDialog()
{
    updateTimer.stop();
    delete ui;
}

void LoadingDialog::progress()
{
    ++step;
}

void LoadingDialog::paintEvent(QPaintEvent *pe)
{
//    if(file && file2)
//    {
//        QPainter painter(this);
//        if(!fread(image_data1,1,s,file))
//        {
//            fseek(file,8,SEEK_SET);
//            fread(image_data1,1,s,file);
//        }
//        QPixmap im(image_data1,wh[0],wh[1],QImage::Format_ARGB32);
//        if(!fread(image_data2,1,s2,file2))
//        {
//            fseek(file2,8,SEEK_SET);
//            fread(image_data2,1,s2,file2);
//        }
//        QPixmap im2(image_data2,wh2[0],wh2[1],QImage::Format_ARGB32);
//        painter.fillRect(rect(),QColor(255,255,255));
//        painter.drawImage(0,0,im);
//        painter.drawImage(wh[0],0,im2);
//        painter.drawRect(rect().left(),rect().top(),rect().width()-1,rect().height()-1);
//    }
//    else close();
    QPainter painter(this);
	painter.fillRect(rect(),QColor(255,255,255));
    uint readStep = step; // a little bit of thread safety, to keep two animations in sync
    QPixmap pix = anisec[readStep%anisec.size()], pix2 = colocle[readStep%colocle.size()];
    painter.drawPixmap(0,0,pix);
    painter.drawPixmap(pix.width(),0,pix2);
	painter.drawRect(rect().left(),rect().top(),rect().width()-1,rect().height()-1);

}
