#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QPaintEngine>
#include <QMessageBox>
#include <QSound>
#include <QFileDialog>
#include <QtConcurrent/QtConcurrent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowSystemMenuHint);
}

MainWindow::~MainWindow()
{
    delete mus;
    delete ui;
    delete [] ProgDir;
    delete [] FileDir;
    delete [] filename;
}

void MainWindow::Init(char *fn, char* dir)
{
    pip=false;
    PDlenght=0;
    if(dir)
    {
        while(dir[PDlenght]!='\0') ++PDlenght;
        while(dir[PDlenght]!='\\'  && dir[PDlenght]!='/' && --PDlenght>=0);
        ++PDlenght;
        if(PDlenght) ProgDir=new char[PDlenght];
        else ProgDir=0;
        memcpy(ProgDir,dir,PDlenght);
    }
    else ProgDir=0;
    coldi=0;
    sdi=0;

    line_drawn=false;
    RMB=LMB=false;

    repainted_by_me=false;
    selected=false;
    selected_is_black=false;

    break_line=LineMode=false;
    SketchMode=false;
    SelectingMode=false;

    MM=false;
    Redone=false;
    pasted=false;
    selected=false;

    selector.setRect(0,0,0,0);
    Ctrl=Shift=Alt=false;

    curcol.setRgb(0,0,0);
    cur_size=0;
//    prect=ui->paper->geometry();
//    prect.setTop(prect.top()+ui->menuBar->height());
//    prect.setBottom(prect.bottom()+ui->menuBar->height());
//    frame_width=prect.width();
//    frame_height=prect.height();

    resizeEvent(nullptr);

    outer_layer.fill(QColor(0,0,0,0));

    debuger=false;
    shade=true;
    connect(&timer,SIGNAL(timeout()),this,SLOT(changeFrame()));
    connect(ui->actionE_xit,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->action_Save,SIGNAL(triggered()),this,SLOT(Save()));
    connect(ui->action_Load,SIGNAL(triggered()),this,SLOT(Load()));
    connect(ui->action_Play,SIGNAL(triggered()),this,SLOT(Play()));
    connect(ui->actionNext,SIGNAL(triggered()),this,SLOT(Next()));
    connect(ui->actionPrev,SIGNAL(triggered()),this,SLOT(Prev()));
    connect(ui->action_Delete,SIGNAL(triggered()),this,SLOT(Delete()));
    connect(ui->action_Undo,SIGNAL(triggered()),this,SLOT(Undo()));
    connect(ui->action_Color,SIGNAL(triggered()),this,SLOT(Color()));
    connect(ui->actionSave_as,SIGNAL(triggered()),this,SLOT(SaveAs()));
    connect(ui->action_Open,SIGNAL(triggered()),this,SLOT(Open()));
    connect(ui->actionSize,SIGNAL(triggered()),this,SLOT(SizeOfPen()));
    connect(ui->actionCopy,SIGNAL(triggered()),this,SLOT(Copy()));
    connect(ui->actionPaste,SIGNAL(triggered()),this,SLOT(Paste()));
    connect(ui->action_Line,SIGNAL(toggled(bool)),this,SLOT(LineToggle(bool)));
    connect(ui->actionS_ketcher,SIGNAL(toggled(bool)),this,SLOT(SketchToggle(bool)));
    connect(ui->actionAttach_music,SIGNAL(triggered()),this,SLOT(AttachMusic()));
    connect(ui->actionDetach_music,SIGNAL(triggered()),this,SLOT(DetachMusic()));
    connect(ui->actionSelector,SIGNAL(toggled(bool)),this,SLOT(SelectingToggle(bool)));
    connect(&selection_color_change_timer,SIGNAL(timeout()),this,SLOT(update()));
    connect(ui->action_Pipette,SIGNAL(toggled(bool)),this,SLOT(TogglePipette(bool)));
    connect(ui->action_Reset,SIGNAL(triggered()),this,SLOT(ResetTools()));
    connect(ui->action_Background,SIGNAL(triggered()),this,SLOT(setBackgroundColor()));
    connect(ui->action_Insert,SIGNAL(triggered()),this,SLOT(insertFrame()));
    connect(ui->actionNew,SIGNAL(triggered()),this,SLOT(New()));
    connect(&MMrepaint_timer,SIGNAL(timeout()),this,SLOT(MMrepaint()));

    mus=0;

    FDlenght=0;
    filename = 0;
    if(fn)
    {
        while(fn[FDlenght]!='\0') ++FDlenght;
        filename=new char[FDlenght+1];
        memcpy(filename,fn,FDlenght+1);
        while(fn[FDlenght]!='\\'  && fn[FDlenght]!='/' && --FDlenght>=0);
        ++FDlenght;
        if(FDlenght)
        {
            FileDir=new char[FDlenght+1];
            FileDir[FDlenght]='\0';
        }
        else FileDir=0;
        memcpy(FileDir,fn,FDlenght);
        Load(true);
    }
    else
    {
        FileDir=0;
        New();
    }
//    new_file=false;
//    LoadingDialog LD(ProgDir,PDlenght);
//    LD.show();
//    LD.repaint();


//    frame_changed=true;
//    //this->setAttribute(Qt::WA_PaintOutsidePaintEvent);

//    copy_pointer=0;
//    ui->FN_LABEL->setText(filename);
//    //SAD.Init(&filename);
//    //OD.Init(&filename);


//    ShImage.fill(0);


//    //pix.fill(QColor(0,0,0,0));
//    //this->showFullScreen();


//    book.Load(filename);
//    if(book.getEnd())background_color=QColor(book.getEnd()->getBackground().toHex()); // get from book
//    else background_color=QColor(0xFFFFFFFF);
//    int c= book.getSize();
//    while(c)
//    {
//        redobook.Add(new LODL(0,0));
//        --c;
//    }
//    current_step=book.getStep();
//    QString s;
//    s.setNum(current_step);
//    ui->INTERVAL_LABEL->setText(s);
//    page=0;
//    ui->PAGE_LABEL->setText(QString::number(page));

//    if(book.getAttch()) mus=new QSound(QString(FileDir)+book.getAttch());
//    else mus=NULL;



//    if(book.getCur())
//    {
//        do
//        {

//            QImage temp_image(prect().width(),prect().height(),QImage::Format_ARGB32);
//            temp_image.fill(book.getCur()->getBackground().toHex());
//            QPainter painter(&temp_image);
//            painter.setRenderHint(QPainter::Antialiasing,book.getCur()->getAA());
//            book.drawCur(painter);
//            book.getCur()->setReadyImage(temp_image);
//            book.getCur()->compressReadyImage();
//            LD.repaint();
//        }
//        while(book.moveOn());
//        book.getCur()->decompressReadyImage();
//        book.getEnd()->decompressReadyImage();
//    }
//    LD.close();
}

void MainWindow::changeFrame()
{
    if(!(book.handleCompressionForwords() && book.moveOn()))
    {
        redobook.moveOn();
        timer.stop();
        shade=true;
        ShImage.fill(0);
        page=0;
        ui->PAGE_LABEL->setText(QString::number(page));
        if(mus)mus->stop();
    }
    else
    {
        redobook.moveOn();
        ++page;
        ui->PAGE_LABEL->setText(QString::number(page));
    }
    frame_changed=true;
    update(prect());
}

void MainWindow::mousePressEvent(QMouseEvent *me)
{
    if(pip)
    {
        if(!coldi)
        {
            coldi=new ColorDialog(ProgDir,PDlenght,this);
            coldi->Select(&curcol, ui->action_Pipette);
        }
        //curcol=QColor(book.getCur()->getReadyImage()[(me->y()-30)*book.getCur()->getIMwidth()+me->x()-10]);
        coldi->Update(QColor(book.getCur()->getReadyImage()[(me->y()-prect().top())*book.getCur()->getIMwidth()+me->x()-prect().left()]));
        coldi->update();
    }
    else
    switch(me->button())
    {
    case Qt::LeftButton:
        LMB=true;
        if(book.getEnd()==0){ book.Add(new LODL(prect().width(),prect().height(),background_color)); redobook.Add(new LODL(0,0)); }
        book.getCur()->Add(new dot_list(SketchMode,curcol,cur_size));
        book.getCur()->getEnd()->AddDot(me->x()-prect().left(),me->y()-prect().top());
        if(LineMode) book.getCur()->getEnd()->AddDot(me->x()-prect().left(),me->y()-prect().top());
        else
        {
            MMrepaint_timer.start(16);
            MMrepaint_count=0;
            MM=true;
        }
        repainted_by_me=true; update(prect());
        break;
    case Qt::RightButton:
        RMB=true;
        if(SelectingMode)
        {
            selection_color_change_timer.stop();
            selected=false;
            copy_list.~LODL();
            selecting_list.~LOPDL();
            selector.setTop(me->y()-prect().top());
            selector.setLeft(me->x()-prect().left());
            selector.setBottom(me->y()-prect().top());
            selector.setRight(me->x()-prect().left());
        }
    default:;
    }

}

void MainWindow::mouseReleaseEvent(QMouseEvent *me)
{
    switch(me->button())
    {
    case Qt::LeftButton:
        LMB=false;
        if(LineMode)
        {
            line_drawn=true;
            while(book.getCur()->getEnd()->getLast()->prev && *book.getCur()->getEnd()->getLast()==*book.getCur()->getEnd()->getLast()->prev) book.getCur()->getEnd()->RemDot();
            repainted_by_me=true; update(prect());
        }
        else
        {
            MMrepaint();
            MMrepaint_timer.stop();
        }
        break;
    case Qt::RightButton:
        RMB=false;
        if(SelectingMode)
        {
            selector=selector.normalized();
            if(book.getCur())
            {
                if(book.getCur()->SelectInRect(selecting_list,selector))
                {
                    do
                    {
                        copy_list.Add(new dot_list(*selecting_list.GetCur()));
                    }
                    while(selecting_list.moveOn());
                    //copy_pointer=NULL;
                    selection_color_change_timer.start(50);
                    selected=true;
//                    unsigned char rgb[3];
//                    selecting_list.GetCur()->getRGB(rgb);
//                    srand((int)rgb[0]+(int)rgb[1]+(int)rgb[2]+1);
                }
                else outer_layer.fill(QColor(0,0,0,0));
            }
            selector.setRect(0,0,0,0);
            /*repainted_by_me=true;*/ update(prect());
        }
    default:;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *me)
{
    if(LMB)
    {
        if(LineMode)
        {
            if(break_line)
            {
                book.getCur()->getEnd()->AddDot(me->x()-prect().left(),me->y()-prect().top());
                break_line=false;
            }
            else book.getCur()->getEnd()->setLastDot(me->x()-prect().left(),me->y()-prect().top());
            repainted_by_me=true; update(prect());
        }
        else
        {
            book.getCur()->getEnd()->AddDot(me->x()-prect().left(),me->y()-prect().top());
            ++MMrepaint_count;
            //MM=true;
    //        QPainter p(&pixmap);
    //        p.setRenderHint(QPainter::Antialiasing);
    //        book.getCur()->getEnd()->draw(p);
        }
        //repainted_by_me=true; repaint(prect());
    }
    else if(RMB)
    {
        if(SelectingMode)
        {
            selector.setRight(me->x()-prect().left());
            selector.setBottom(me->y()-prect().top());
            repainted_by_me=true; update(prect());
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *ke)
{
    QString s;
    switch(ke->key())
    {
    case Qt::Key_Right:
        if(SelectingMode && !Ctrl)
        {
            selecting_list.moveThoseX(1);
            copy_list.moveThoseX(1);
            repainted_by_me=!selecting_list.empty(); update(prect());
        }
        else Next();
        break;
    case Qt::Key_Left:
        if(SelectingMode && !Ctrl)
        {
            selecting_list.moveThoseX(-1);
            copy_list.moveThoseX(-1);
            repainted_by_me=!selecting_list.empty(); update(prect());
        }
        else Prev();
        break;
    case Qt::Key_Delete:
        Delete();
        break;
    case Qt::Key_Down:
        if(SelectingMode && !Ctrl)
        {
            selecting_list.moveThoseY(1);
            copy_list.moveThoseY(1);
            repainted_by_me=!selecting_list.empty(); update(prect());
        }
        else CUndo();
        break;
    case Qt::Key_Up:
        if(SelectingMode && !Ctrl)
        {
            selecting_list.moveThoseY(-1);
            copy_list.moveThoseY(-1);
            repainted_by_me=!selecting_list.empty(); update(prect());
        }
        else Play();
        break;
    case Qt::Key_Control:
        Ctrl=true;
        break;
    case Qt::Key_Alt:
        Alt=true;
        break;
    case Qt::Key_C:
        if(Ctrl) Copy();
        break;
    case Qt::Key_V:
        if(Ctrl) Paste();
        break;
    case Qt::Key_Z:
        if(Ctrl) Undo();
        break;
    case Qt::Key_Y:
        if(Ctrl) Redo();
        break;
    case Qt::Key_Space:
        if(timer.isActive())
        {
            timer.stop();
            shade=true;
            repainted_by_me=true; update(prect());
        }
        else
        {
            timer.start(current_step);
            shade=false;
        }
        break;
    case Qt::Key_Plus:
    case Qt::Key_Equal:
        current_step+=5;
        s.setNum(current_step);
        ui->INTERVAL_LABEL->setText(s);
        break;
    case Qt::Key_Minus:
        current_step-=5;
        s.setNum(current_step);
        ui->INTERVAL_LABEL->setText(s);
        break;
    case Qt::Key_B:
        if(Alt) setBackgroundColor();
        else break_line=true;
        break;
    case Qt::Key_S:
        if(Ctrl) Save(true);
        else
        {
            shade=!shade;
            repainted_by_me=true; update(prect());
        }
        break;
    case Qt::Key_A:
        book.getCur()->switchAA();
        repainted_by_me=true; update(prect());
        break;
    case Qt::Key_Escape:
        close();
        break;
    case Qt::Key_Backspace:
        book.getCur()->RemSketches();
        repainted_by_me=true; update(prect());
        break;
    case Qt::Key_Shift:
        Shift=true;
        break;
    case Qt::Key_M:
        if(mus)mus->play();
        break;
    case Qt::Key_R:
        if(SelectingMode) selecting_list.~LOPDL();
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *ke)
{
    switch(ke->key())
    {
    case Qt::Key_Control:
        Ctrl=false;
        break;
    case Qt::Key_Shift:
        Shift=false;
    }
}

void MainWindow::paintEvent(QPaintEvent *pe)
{
    QPainter painter, painter2(&pix);
    QImage Image;
    bool outer=true;

    if(selected)
    {
        painter.begin(&outer_layer);
        outer_layer.fill(QColor(0,0,0,0));
        painter.setRenderHint(QPainter::Antialiasing,book.getCur()->getAA());
        if(selected_is_black=!selected_is_black) copy_list.draw(painter,0,0,0);
        else copy_list.draw(painter,255,255,255);
        painter.end();
    }
    if(book.getCur())
    {
        QImage prerendered = QImage((unsigned char *)book.getCur()->getReadyImage(),book.getCur()->getIMwidth(), book.getCur()->getIMheight(), QImage::Format_ARGB32);
        if(frame_changed)
        {
            if(book.getCur()->prev)
            {
                int w=book.getCur()->prev->getIMwidth(), h=book.getCur()->prev->getIMheight();
                ShImage.fill(0);
                book.getCur()->prev->getShadeyImage(ShImage.bits(), ShImage.byteCount());
//                QImage im =QImage((unsigned char *) x,w,h,QImage::Format_ARGB32);
//                ShImage=im.copy();
            }
            else ShImage.fill(0);
            pix.fill(book.getCur()->getBackground().toHex());
            painter2.drawImage(0,0,prerendered);
            if(shade)painter2.drawImage(0,0,ShImage);
            frame_changed=false;
        }

        if(repainted_by_me)
        {
            if(RMB)
            {
                painter.begin(&outer_layer);
                outer_layer.fill(QColor(0,0,0,0));
                QPen pen;
                pen.setStyle(Qt::DotLine);
                painter.setPen(pen);
                painter.drawRect(selector);
                painter.end();
            }
            else if(LineMode && LMB)
            {
                painter.begin(&outer_layer);
                outer_layer.fill(QColor(0,0,0,0));
                painter.setRenderHint(QPainter::Antialiasing,book.getCur()->getAA());
                book.getCur()->drawLast(painter);
                painter.end();
            }
            else
            {
                Image=QImage(prect().size(), QImage::Format_ARGB32);
                painter.begin(&Image);
                painter.setRenderHint(QPainter::Antialiasing,book.getCur()->getAA());
                painter2.setRenderHint(QPainter::Antialiasing,book.getCur()->getAA());

                Image.fill(book.getCur()->getBackground().toHex());
                painter.drawImage(0,0, prerendered);

                if(MM)
                {
                    if(book.getCur()->getEnd())
                    {
                        book.getCur()->getEnd()->drawLast(painter,MMrepaint_count);
                        book.getCur()->getEnd()->drawLast(painter2,MMrepaint_count);
                    }
                    MMrepaint_count=0;
                    MM=false;
                    outer=false;
                }
                else if(line_drawn)
                {
                    outer_layer.fill(QColor(0,0,0,0));
                    book.getCur()->drawLast(painter);
                    book.getCur()->drawLast(painter2);
                    line_drawn=false;
                }
                else if(Redone)
                {
                    book.getCur()->drawLast(painter);
                    book.getCur()->drawLast(painter2);
                    Redone=false;
                }
                else if(pasted)
                {
                    copy_list.draw(painter);
                    copy_list.draw(painter2);
                    pasted=false;
                }
                else
                {

                //    if(debuger)
                //    {
                //        book.getCur()->getEnd()->draw(painter);
                //        debuger=false;
                //        return;
                //    }

                    //QBrush b(QColor(255,255,255));
                    //painter.fillRect(QRect(0,0,prect().width(),prect().height()),b);
                    Image.fill(book.getCur()->getBackground().toHex());

                    book.drawCur(painter);
                    painter2.drawImage(0,0,Image);
                    if(shade) painter2.drawImage(0,0,ShImage);
                    //if(shade) book.drawPrev(painter);
                }
                book.getCur()->setReadyImage(Image);
                painter.end();
            }
            repainted_by_me=false;
        }
    }
    else
    {
        pix.fill();
    }
    painter.begin(this);



    painter.drawPixmap(prect().topLeft(), pix);


    //if(shade) painter.drawImage(10,30,ShImage);

    if(outer) painter.drawPixmap(prect().left(),prect().top(),outer_layer);
}

void MainWindow::Play()
{
    //QMessageBox::critical(0,"play","fine");
    if(!book.getCur())
        return;
    book.setCur();
    redobook.setCur();
    page=0;
    ui->PAGE_LABEL->setText(QString::number(page));
    timer.start(current_step);
    shade=false;
    frame_changed=true;
    update(prect());
    if(mus)mus->play();
}

void MainWindow::Save(bool no_check)
{
//    if(book.getEnd()==NULL) return;
//    book.setCur();
//    file=fopen(filename,"wb");
//    unsigned char b[5];
//    fwrite(&standart_step,sizeof(int),1,file);
//    do
//    {
//        b[0]='=';
//        fwrite(b,1,1,file);
//        do
//        {
//            b[0]='>';
//            book.getCur()->getCur()->getRGB(b+1);
//            b[4]=book.getCur()->getCur()->getSize();
//            fwrite(b,1,5,file);
//            book.getCur()->getCur()->writeTo(file);
//        }
//        while(book.getCur()->moveOn());
//    }
//    while(book.moveOn());
//    fclose(file);
//    book.resetCur();
    if(new_file)
    {
        SaveAs();
        return;
    }
    if(no_check || QMessageBox::question(this, "Save", "Save?", "uhu", "not really")==0)
    {
        book.setStep(current_step);
        book.Save(filename);
    }
}

void MainWindow::Load(bool no_check)
{
//    book.~LOLODL();
//    unsigned char buf[4];
//    int temp[2];
//    if(file=fopen(filename,"rb"))
//    {
//        fread(&standart_step,sizeof(int),1,file);
//        while(!feof(file))
//        {
//            fread(buf,1,1,file);
//            switch (buf[0])
//            {
//            case '=':
//                book.Add(new LODL);
//                break;
//            case '>':
//                fread(buf,1,4,file);
//                book.getEnd()->Add(new dot_list(QColor(buf[0],buf[1],buf[2]),buf[3]));
//                break;
//            case ' ':
//                fread(temp,sizeof(int),2,file);
//                book.getEnd()->getEnd()->AddDot(temp[0],temp[1]);
//            }
//        }
//        QString s;
//        s.setNum(standart_step);
//        ui->INTERVAL_LABEL->setText(s);
//        page=0;
//        ui->PAGE_LABEL->setText(QString::number(page));
//        fclose(file);
//    }
    if(new_file)
    {
        timer.stop();
        Open();
        return;
    }
    if((no_check || QMessageBox::question(this, "Load", "Load?", "uhu", "not really")==0))
    {
        timer.stop();
        ui->FN_LABEL->setText(filename);

        LD = new LoadingDialog(ProgDir, PDlenght, this);
        LD->show();
        this->setEnabled(false);

        book.Load(filename);

        ui->paper->setMinimumSize(book.getCur()->getIMwidth(), book.getCur()->getIMheight());

        redobook.~LOLODL();
        selecting_list.~LOPDL();
        int c=book.getSize();
        while(c)
        {
            redobook.Add(new LODL(0,0));
            --c;
        }
        current_step=book.getStep();
        page=0;
        ui->PAGE_LABEL->setText(QString::number(page));
        QString s;
        s.setNum(current_step);
        ui->INTERVAL_LABEL->setText(s);

        delete mus;
        if(book.getAttch()) mus=new QSound(QString(FileDir)+book.getAttch());
        else mus=0;

        copy_pointer=0;

        if(book.getEnd())background_color=QColor(book.getEnd()->getBackground().toHex()); // get from book
        else background_color=QColor(0xFFFFFFFF);

        QtConcurrent::run([this]()
        {
            if(book.getCur())
            {
                do
                {
                    QImage temp_image(book.getCur()->getIMwidth(),book.getCur()->getIMheight(),QImage::Format_ARGB32);
                    temp_image.fill(book.getCur()->getBackground().toHex());
                    QPainter painter(&temp_image);
                    painter.setRenderHint(QPainter::Antialiasing,book.getCur()->getAA());
                    book.drawCur(painter);
                    book.getCur()->setReadyImage(temp_image);
                    book.getCur()->compressReadyImage();
                    LD->progress();
                }
                while(book.moveOn());
                book.getCur()->decompressReadyImage();
                book.getEnd()->decompressReadyImage();
            }
            ShImage.fill(0);
            frame_changed=true;
            update(prect());
            LD->close();
            delete LD;
            this->setEnabled(true);
        });
    }
}

void MainWindow::Next()
{
    if(book.handleCompressionForwords(false))
    {
        if(!book.moveOn())
        {

            redobook.Add(new LODL(0,0));
            book.Add(new LODL(prect().width(),prect().height(),background_color));
            book.moveBack();
        }
        redobook.moveOn();
        ++page;
        ui->PAGE_LABEL->setText(QString::number(page));
    }
    frame_changed=true;
    update(prect());
}

void MainWindow::Prev()
{
    if(book.getCur())
    {
        redobook.moveBack();
        book.moveBack();
        book.handleCompressionBackwords();
        if((--page)<0) page=book.getSize()-1;
        ui->PAGE_LABEL->setText(QString::number(page));
        frame_changed=true;
        update(prect());
    }
}

void MainWindow::Delete()
{
    if(SelectingMode)
    {
        if(book.getCur()) book.getCur()->RemGiven(selecting_list);
        repainted_by_me=true; update(prect());
    }
    else
    if(Shift || QMessageBox::question(this,"Delete", "Delete? \n(Hold shift key to avoid this question.)", "uhu", "not really")==0)
    {
        redobook.RemCur();
        book.RemCur();
        book.handleCompressionAfterRemoval();
        if(page)
        {
            --page;
            ui->PAGE_LABEL->setText(QString::number(page));
        }
        frame_changed=true;
        update(prect());
    }
}

void MainWindow::Undo()
{
    if(book.getCur() && book.getCur()->getEnd())
    {
        if(!redobook.getCur()) redobook.Add(new LODL(0,0));
        redobook.getCur()->Add(new dot_list(*book.getCur()->getEnd()));
        book.getCur()->Rem();
        repainted_by_me=true; update(prect());
    }
}

void MainWindow::CUndo()
{
    if(book.getCur() && book.getCur()->getEnd() && !book.getCur()->getEnd()->RemDot()) book.getCur()->Rem();
    repainted_by_me=true; update(prect());
}

void MainWindow::Color()
{
    if(!coldi)
    {
        coldi=new ColorDialog(ProgDir,PDlenght,this);
        coldi->Select(&curcol, ui->action_Pipette);
    }
    coldi->show();
}

void MainWindow::SaveAs()
{
    QString str=QFileDialog::getSaveFileName(this,"",FileDir,"Animations (*.ani);;All files(*.*)");
    if(!str.isEmpty())
    {
        new_file=false;
        delete [] filename;
        filename=new char[str.length()+1];
        memcpy(filename,str.toLatin1().data(),str.length()+1);
        delete [] FileDir;
        FDlenght=0;
        while(filename[FDlenght]!='\0') ++FDlenght;
        while(filename[FDlenght]!='\\' && filename[FDlenght]!='/' && --FDlenght>=0);
        ++FDlenght;
        if(FDlenght)
        {
            FileDir=new char[FDlenght+1];
            FileDir[FDlenght]='\0';
        }
        else FileDir=0;
        memcpy(FileDir,filename,FDlenght);
        Save(true);
        ui->FN_LABEL->setText(filename);
    }
}

void MainWindow::Open()
{
    switch(CheckForChngesToSave())
    {
    case SAVE:
        Save(true);
    case THATS_FINE:
        break;
    case WAIT_A_MINUTE:
        return;
    }
    QString str=QFileDialog::getOpenFileName(this,"",FileDir,"Animations (*.ani);;All files(*.*)");
    if(!str.isEmpty())
    {
        new_file=false;
        delete [] filename;
        filename=new char[str.length()+1];
        memcpy(filename,str.toLatin1().data(),str.length()+1);
        delete [] FileDir;
        FDlenght=0;
        while(filename[FDlenght]!='\0') ++FDlenght;
        while(filename[FDlenght]!='\\' && filename[FDlenght]!='/' && --FDlenght>=0);
        ++FDlenght;
        if(FDlenght)
        {
            FileDir=new char[FDlenght];
            FileDir[FDlenght]='\0';
        }
        else FileDir=0;
        memcpy(FileDir,filename,FDlenght);
        Load(true);
    }
}

void MainWindow::SizeOfPen()
{
    if(!sdi)
    {
        sdi=new SizeDialog(this);
        sdi->Select(&cur_size);
    }
    sdi->show();
}

void MainWindow::Copy()
{
    if(!SelectingMode)copy_pointer=book.getCur();
    else selecting_list.~LOPDL();
}

void MainWindow::Paste()
{
    if(!selected)
    {
        *(book.getCur())=*copy_pointer;
        book.getCur()->decompressReadyImage();
        frame_changed=true;
        update(prect());
    }
    else if(copy_list.GetCount())
    {
        if(book.getCur()==NULL) book.Add(new LODL(prect().width(),prect().height(),background_color));
        do
        {
            book.getCur()->Add(new dot_list(*copy_list.getCur()));
        }
        while(copy_list.moveOn());
        pasted=true;
        repainted_by_me=true; update(prect());
    }

}

void MainWindow::LineToggle(bool on)
{
    LineMode=on;
}

void MainWindow::SketchToggle(bool on)
{
    SketchMode=on;
}

void MainWindow::AttachMusic()
{
    if(AMD.exec())
    {
        book.setAttch(AMD.get());
        delete mus;
        mus=new QSound(QString(FileDir)+book.getAttch());
    }
}

void MainWindow::DetachMusic()
{
    book.setAttch(NULL);
    delete mus;
    mus=NULL;
}

void MainWindow::closeEvent(QCloseEvent *qe)
{
    switch(CheckForChngesToSave())
    {
    case SAVE:
        Save(true);
    case THATS_FINE:
        qe->accept();
        break;
    case WAIT_A_MINUTE:
        qe->ignore();
    }
}

void MainWindow::resizeEvent(QResizeEvent *re)
{
    QMainWindow::resizeEvent(re);

    ShImage = QImage(prect().width(),prect().height(),QImage::Format_ARGB32);
    ShImage.fill(0);

    outer_layer=QPixmap(prect().width(),prect().height());
    outer_layer.fill(QColor(0,0,0,0));
    pix=QPixmap(prect().width(),prect().height());
    pix.fill();
    repainted_by_me = true;
    update(prect());
}

void MainWindow::SelectingToggle(bool on)
{
    ui->actionCopy->setDisabled(on);
    if(!(SelectingMode=on))
    {
        selection_color_change_timer.stop();
        selected=false;
        outer_layer.fill(QColor(0,0,0,0));
        update(prect());
    }
}

void MainWindow::Redo()
{
    if(redobook.getCur()->getEnd())
    {
        book.getCur()->Add(new dot_list(*redobook.getCur()->getEnd()));
        redobook.getCur()->Rem();
        Redone=true;
        repainted_by_me=true; update(prect());
    }

}

void MainWindow::TogglePipette(bool p)
{
    pip=p;
}

void MainWindow::ResetTools()
{
    ui->actionSelector->setChecked(false);
    SelectingToggle(false);
    ui->action_Line->setChecked(false);
    LineToggle(false);
    ui->actionS_ketcher->setChecked(false);
    SketchToggle(false);
    ui->action_Pipette->setChecked(false);
    TogglePipette(false);
}

void MainWindow::setBackgroundColor()
{
    background_color=curcol;
    if(book.getCur()) book.getCur()->setBackground(background_color);
    else book.Add(new LODL(prect().width(),prect().height(),background_color));
    repainted_by_me=true; update(prect());
}

void MainWindow::insertFrame()
{
    book.addToLeft(new LODL(prect().width(),prect().height(),QColor(book.getCur()->getBackground().toHex()))); // not the best way... will change
    redobook.addToLeft(new LODL(0,0));
    ++page;
    Prev();
}

void MainWindow::New()
{
    switch(CheckForChngesToSave())
    {
    case SAVE:
        Save(true);
    case THATS_FINE:
        break;
    case WAIT_A_MINUTE:
        return;
    }
    selecting_list.~LOPDL();
    book.Load("\0");
    redobook.~LOLODL();
    delete [] filename;
    filename = new char[8];
    memcpy(filename,"new.ani",8);
    ui->FN_LABEL->setText(filename);
    delete mus;
    mus=0;
    copy_pointer=0;
    page=0;
    ui->PAGE_LABEL->setText(QString::number(page));
    current_step=50;
    book.setStep(current_step);
    ui->INTERVAL_LABEL->setNum(current_step);
    background_color=QColor(0xFFFFFFFF);
    ShImage.fill(0);
    new_file=true;

    book.Add(new LODL(prect().width(),prect().height(),background_color));
    redobook.Add(new LODL(0,0));

    frame_changed=true;
    update(prect());
}

int MainWindow::CheckForChngesToSave()
{
    if(book.isEmpty()) return THATS_FINE;
    LOLODL tempbook;
    tempbook.Load(filename);
    if(tempbook!=book || current_step!=tempbook.getStep())
    {
        return QMessageBox::question(this,"Closeing...","Something is not saved... ?","save","that's fine","wait a minute");
    }
    return THATS_FINE;
}

const QRect MainWindow::prect() const
{
    QRect geom = ui->paper->geometry();
    geom.adjust(0, ui->menuBar->height(), 0, ui->menuBar->height());
    return geom;
}

void MainWindow::MMrepaint()
{
    if(MMrepaint_count)
    {
        MM=true;
        repainted_by_me=true;
        update(prect());
    }
}
