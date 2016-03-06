#ifndef DOT_LIST_H
#define DOT_LIST_H
//#include <stdio.h>
//#include <memory.h>
// #include <huffmancoder/huffmancoder.h>
#include <QPainter> // can be removed(with all consequenses) for use outside Qt
#include <stdexcept>

// typedefs from huffmacoder...
typedef  unsigned int unsint;
typedef  unsigned char Byte;

struct SimpleColor
{
    unsigned char red, green, blue, alph;
    SimpleColor(){red=green=blue=alph=0;}
    SimpleColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255)
    {
        red=r;
        green=g;
        blue=b;
        alph=a;
    }

    SimpleColor & operator=(QColor & c)
    {
        blue=c.blue();
        green=c.green();
        red=c.red();
        alph=c.alpha();
        return *this;
    }

    SimpleColor & operator=(SimpleColor & c)
    {
        blue=c.blue;
        green=c.green;
        red=c.red;
        alph=c.alph;
        return *this;
    }

    void set(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255)
    {
        red=r;
        green=g;
        blue=b;
        alph=a;
    }

    bool operator!=(SimpleColor &s) {return red!=s.red || green!=s.green || blue!=s.blue || alph!=s.alph;}

    unsigned int toHex()
    {
        unsigned int ret=alph;
        ret=ret<<8;
        ret+=red;
        ret=ret<<8;
        ret+=green;
        ret=ret<<8;
        ret+=blue;
        return ret;
    }
};

class dot_list
{
    struct dot
    {
        int x, y;
        dot* next, *prev;
        dot(){next=prev=NULL;}
        ~dot(){delete next;}
        bool operator!=(dot &d) { return x!=d.x || y!=d.y; }
        bool operator==(dot &d) { return x==d.x && y==d.y; }
    };

    dot *start, *end;
    SimpleColor color;
    //QColor color;

    unsigned char size;
    QPen pen; // a little Qt optimization, can be removed easyly
    unsigned char pr_alpha;
    int top, bottom, left, right;
    int count;
    unsigned char flags;  // 0 - is_sketch, 1 - , 2 - , 3 - , 4 - , 5 - , 6 - , 7 - ,
public:
    bool operator !=(dot_list &dl)
    {
        if(count!=dl.count || size!=dl.size || flags!=dl.flags || color!=dl.color) return true;
        dot* runner=end;
        dot* dlrunner=dl.end;
        while(runner)
        {
            if(*runner!=*dlrunner) return true;
            runner=runner->prev;
            dlrunner=dlrunner->prev;
        }
        return false;
    }

    dot_list *next, *prev;
    dot_list(QColor c=QColor(0,0,0),unsigned char s=0, unsigned char f=0){start=end=NULL;next=prev=NULL;
                                                       color=c;size=s;
                                                       pr_alpha=50;
                                                       pen.setCapStyle(Qt::RoundCap);
                                                       pen.setJoinStyle(Qt::RoundJoin);
                                                       pen.setWidth(size);pen.setColor(c);
                                                       top=bottom=left=right=count=0;
                                                       flags=f;}

    dot_list( bool sk, QColor c=QColor(0,0,0),unsigned char s=0){start=end=NULL;next=prev=NULL;
                                                       color=c;size=s;
                                                       pr_alpha=50;
                                                       pen.setCapStyle(Qt::RoundCap);
                                                       pen.setJoinStyle(Qt::RoundJoin);
                                                       pen.setWidth(size);pen.setColor(c);
                                                       top=bottom=left=right=count=0;
                                                       sk?flags=1:flags=0; /* will change if new flags added */}

//    dot_list(unsigned char r=0,unsigned char g=0,unsigned char b=0,unsigned char s=0,  unsigned char f=0){start=end=NULL;next=prev=NULL;
//                                                       color.red=r; color.green=g; color.blue=b; size=s;
//                                                       pr_alpha=50;                                                     // not Qt version, pen can be removed if needed... it's getting old... i'll keep it anyway
//                                                       pen.setCapStyle(Qt::RoundCap);
//                                                       pen.setWidth(size); pen.setColor(QColor(r,g,b));
//                                                       top=bottom=left=right=0;
//                                                       flags=f;}
    dot_list(dot_list &x)
    {
        start=end=NULL;
        next=prev=NULL;
        dot *runer=x.start;
        while(runer)
        {
            AddDot(runer->x,runer->y);
            runer=runer->next;
        }
        color=x.color;
        size=x.size;
        pr_alpha=x.pr_alpha;
        pen=x.pen;
//        pen.setCapStyle(Qt::RoundCap);
//        pen.setJoinStyle(Qt::RoundJoin);
//        pen.setWidth(size);
//        pen.setColor(QColor(color.red,color.green,color.red));
        top=x.top; bottom=x.bottom;
        left=x.left; right=x.right;
        count=x.count;
        flags=x.flags;
    }

    ~dot_list(){delete start; delete next;}
    void AddDot(int x, int y)
    {
        if(start==NULL)
        {
            start=new dot;
            end=start;
            left=right=x;
            bottom=top=y;
        }
        else
        {
            end->next=new dot;
            end->next->prev=end;
            end=end->next;
            if(x>right) right=x;
            if(x<left) left=x;
            if(y>bottom) bottom=y;
            if(y<top) top=y;
        }
        end->x=x;
        end->y=y;
        ++count;
    }
    bool RemDot()
    {
        if(start==NULL) return false;
        if(end->prev==NULL)
        {
            delete end;
            start=end=NULL;
        }
        else
        {
            end=end->prev;
            delete end->next;
            end->next=NULL;
        }
        --count;
        return true;
    }

    dot_list * RemGiven(dot *g)
    {
        dot_list * ret=NULL;
        throw new std::logic_error("Not implemented");
        return ret;
    }

    void setLastDot(int x, int y)
    {
        end->x=x;
        end->y=y;
    }

    void draw(QPainter &painter)
    {

//        if(start==NULL) return;
//        dot *a=start, *b=start;
//        painter.setPen(pen);
//        painter.drawPoint(b->x,b->y);
//        while(a=a->next)                                   old old way
//        {
//            painter.drawLine(b->x,b->y,a->x,a->y);
//            b=a;
//        }

        painter.setPen(pen);
        if(start==NULL) return;
        if(count==1) painter.drawPoint(start->x,start->y);
        else
        {
            dot *a=start;
            //QPoint p[count];//O_O... i'm going to get rid of all of you abominations one day...
            QPoint *p=new QPoint[count];
            for(int i=0;i<count;++i)
            {
                p[i].setX(a->x);
                p[i].setY(a->y);
                a=a->next;
            }
            painter.drawPolyline(p,count);
            delete [] p;
        }
    }

    void draw(QPainter &painter, int r, int g, int b)
    {

//        if(start==NULL) return;
//        dot *a=start, *b=start;
//        painter.setPen(pen);
//        painter.drawPoint(b->x,b->y);
//        while(a=a->next)                                   old old way
//        {
//            painter.drawLine(b->x,b->y,a->x,a->y);
//            b=a;
//        }
        pen.setColor(QColor(r,g,b));
        painter.setPen(pen);
        if(start==NULL) return;
        if(count==1) painter.drawPoint(start->x,start->y);
        else
        {
            dot *a=start;
            //QPoint p[count]; //O_O... what the hell?...
            QPoint *p=new QPoint[count];
            for(int i=0;i<count;++i)
            {
                p[i].setX(a->x);
                p[i].setY(a->y);
                a=a->next;
            }
            painter.drawPolyline(p,count);
            delete [] p;
        }
        pen.setColor(QColor(color.red,color.green,color.blue));
    }

    void drawShady(QPainter &painter)
    {

//        if(start==NULL) return;
//        dot *a=start, *b=start;
//        QColor cc=QColor(color.red,color.green,color.blue);
//        cc.setAlpha(alpha);
//        pen.setColor(cc);
//        painter.setPen(pen);
//        painter.drawPoint(b->x,b->y);
//        while(a=a->next)
//        {
//            painter.drawLine(b->x,b->y,a->x,a->y);
//            b=a;
//        }
//        pen.setColor(QColor(color.red,color.green,color.blue));


        QColor cc=QColor(color.red,color.green,color.blue,pr_alpha);
        pen.setColor(cc);
        painter.setPen(pen);

        if(start==NULL) return;
        if(count==1) painter.drawPoint(start->x,start->y);
        else
        {
            dot *a=start;
            //QPoint p[count];//O_O... you again!!!
            QPoint *p=new QPoint[count];
            for(int i=0;i<count;++i)
            {
                p[i].setX(a->x);
                p[i].setY(a->y);
                a=a->next;
            }
            painter.drawPolyline(p,count);
            delete [] p;
        }
        pen.setColor(QColor(color.red,color.green,color.blue));
    }

    void drawLast(QPainter &painter, int cnt=1) // draw last cnt lines
    {
        painter.setPen(pen);
        if(end->prev)
        {
            ++cnt;
            dot* b=end;
            //QPoint p[cnt];//O_O... but... but how?!
            QPoint *p=new QPoint[cnt];
            for(int i=0;i<cnt;++i)
            {
                p[i].setX(b->x);
                p[i].setY(b->y);
                b=b->prev;
            }
            painter.drawPolyline(p,cnt);
            delete [] p;
        }
        else painter.drawPoint(end->x,end->y);

//        painter.setPen(pen);
//        if(end->prev)
//        {
//        dot* b=end->prev;
//        painter.drawLine(b->x,b->y,end->x,end->y);
//        }
//        else painter.drawPoint(end->x,end->y);
    }

    void writeTo(FILE* file)
    {
        char b=' ';
        dot* c=start;
        int t[2];
        while(c)
        {
            fwrite(&b,sizeof(char),1,file);
            t[0]=c->x;
            t[1]=c->y;
            fwrite(t,sizeof(int),2,file);
            c=c->next;
        }
    }

    bool Empty(){return (start==NULL);}
    void getRGB(unsigned char* x)
    {
        x[0]=color.red;
        x[1]=color.green;
        x[2]=color.blue;
    }
    unsigned char getSize(){return size;}

    bool IsInRect(QRect rect)
    {
        dot* cur=start;
        while(cur)
        {
            if(rect.contains(cur->x,cur->y)) return true;
            cur=cur->next;
        }
        return false;
    }

    bool IsInRect(int t, int b, int l, int r)
    {
        dot* cur=start;
        while(cur)
        {
            if(cur->x>=l && cur->x<=r && cur->y>=t && cur->y<=b) return true;
            cur=cur->next;
        }
        return false;
    }

    int getWidth(){return right-left+size;}
    int getHeight(){return bottom-top+size;}
    int getTop(){return top-size/2;}
    int getBottom(){return bottom+size/2;}
    int getRight(){return right+size/2;}
    int getLeft(){return left-size/2;}
    dot* getFirst(){return start;}
    dot* getLast(){return end;}

    void crop(int l, int t)
    {
        dot* cur=start;
        while(cur)
        {
            cur->x-=l;//-1
            cur->y-=t;//-1
            cur=cur->next;
        }
        top-=t;//-1
        bottom-=t;//-1
        left-=l;//-1
        right-=l;//-1
    }

    void invert(int i, int correction)
    {
        dot* cur=start;
        while(cur)
        {
            cur->x+=(2*(i-cur->x)) + correction;
            cur=cur->next;
        }
    }

    bool isSketch(){ return flags&1; }
    void setToScketch() { flags=flags|1; } // don't think i'll need, but anyways...
    char getFlags(){ return flags; }

    void move(int x, int y)
    {
        dot* runner=start;
        while(runner)
        {
            runner->x+=x;
            runner->y+=y;
            runner=runner->next;
        }
    }

    void moveX(int x)
    {
        dot* runner=start;
        while(runner)
        {
            runner->x+=x;
            runner=runner->next;
        }
    }

    void moveY(int y)
    {
        dot* runner=start;
        while(runner)
        {
            runner->y+=y;
            runner=runner->next;
        }
    }

};

class LOPDL
{
    struct PDL
    {
        dot_list *to;
        PDL *next, *prev;
        PDL(){to=NULL; next=prev=NULL;}
        PDL(dot_list *t){to=t, next=prev=NULL;}
        ~PDL(){delete next;}
    };
    PDL *start, * end, *cur;

public:
    LOPDL(){start=end=cur=NULL;}
    ~LOPDL(){delete start; start=end=cur=NULL;}

    void AddDL(dot_list *x)
    {
        if(!start)
        {
            start=new PDL(x);
            end=start;
            cur=start;
        }
        else
        {
            end->next=new PDL(x);
            end->next->prev=end;
            end=end->next;
        }
    }

    dot_list * GetCur(){return cur->to;}
    bool moveOn()
    {
        if(cur=cur->next) return true;
        else { cur=start; return false;}
    }

    void moveThoseX(int x)
    {
        PDL *runner=start;
        while(runner)
        {
            runner->to->moveX(x);
            runner=runner->next;
        }

    }

    void moveThoseY(int y)
    {
        PDL *runner=start;
        while(runner)
        {
            runner->to->moveY(y);
            runner=runner->next;
        }
    }

    void moveThose(int x, int y)
    {
        PDL *runner=start;
        while(runner)
        {
            runner->to->move(x,y);
            runner=runner->next;
        }
    }

    bool empty(){return (start==NULL);}
};

class LODL
{
    dot_list *start, *end, *finder;
    dot_list *cur;
    int count;
    int top, bottom, left, right;
    LOPDL sketchlist;
    SimpleColor background;
    Byte *imageBuf; // for performance
    unsigned char imageFlags;// 0 - Image is compressed, 1 - is not antialiased, 2 - , 3 - , 4 - , 5 - , 6 - , 7 -
    unsigned int IMwidth, IMheight;
    unsigned int cmprIMsize;
public:
    LODL *next, *prev;
    bool operator!=(LODL &l)
    {
        if(count!=l.count || background!=l.background) return true;
        dot_list *runner=end, *lrunner=l.end;
        while(runner)
        {
            if(*runner!=*lrunner) return true;
            runner=runner->prev;
            lrunner=lrunner->prev;
        }
        return false;
    }
//////////////////////////////////////////////////////////////
    LODL(unsigned int imw=621, unsigned int imh=411, unsigned char bkg_r=255, unsigned char bkg_g=255, unsigned char bkg_b=255)
    {
        start=end=cur=finder=NULL;
        prev=next=NULL;
        imageBuf=NULL;
        top=bottom=left=right=count=0;
        background.set(bkg_r,bkg_g,bkg_b);
        unsigned int s=(IMwidth=imw)*(IMheight=imh);
        cmprIMsize=0;
        imageFlags=0;
        int *readyImage =(int*) (imageBuf = new Byte[s*sizeof(int)]);
        int bkg=background.toHex();
        for(unsint i=0;i<s;++i)
        {
            readyImage[i]=bkg;
        }
    }

    LODL( unsigned int imw, unsigned int imh, QColor bkgrnd) // QColor here can be exchanged with SimpleColor
    {
        start=end=cur=finder=NULL;
        prev=next=NULL;
        imageBuf=NULL;
        top=bottom=left=right=count=0;
        background=bkgrnd;
        unsigned int s=(IMwidth=imw)*(IMheight=imh);
        imageFlags=0;
        cmprIMsize=0;
        int * readyImage =(int *) (imageBuf = new Byte[s*sizeof(int)]);
        int bkg=background.toHex();
        for(unsint i=0;i<s;++i)
        {
            readyImage[i]=bkg;
        }
    }

//////////////////////////////////////////////////////////////////////////////////
    ~LODL(){delete start; delete next; delete [] imageBuf; imageBuf=0; start=end=cur=finder=0; top=bottom=left=right=count=0;}
    void Add(dot_list *x)
    {
        if(x==NULL) x=new dot_list();
        count++;
        if(x->isSketch()) sketchlist.AddDL(x);
        if(start==NULL)
        {
            start=x;
            end=start;
        }
        else
        {
            end->next=x;
            end->next->prev=end;
            end=end->next;
        }
        cur=start;
    }
    bool Rem()
    {
        count--;
        if(start==NULL) return false;
        if(end->prev==NULL)
        {
            delete end;
            start=end=NULL;
        }
        else
        {
            end=end->prev;
            delete end->next;
            end->next=NULL;
        }
        cur=start;
        return true;
    }

    bool RemCur()
    {
        if(cur==NULL) return false;
        if(cur->prev==NULL)
        {
            if(cur->next==NULL)
            {
                delete cur;
                cur=start=end=NULL;
            }
            else
            {
                cur=cur->next;
                cur->prev->next=NULL;
                cur->prev=NULL;
                delete start;
                start=cur;
            }
        }
        else
        {
            if(cur->next==NULL)
            {
                cur=cur->prev;
                cur->next=NULL;
                delete end;
                end=cur;
            }
            else
            {
                cur=cur->prev;
                cur->next=cur->next->next;
                cur->next->prev->next=NULL;
                delete cur->next->prev;
                cur->next->prev=cur;

            }
        }
        --count;
        return true;
    }

    bool RemGiven(dot_list *x)  // heyyy, dont pass x that is not fom the list >:[
    {
        if(x==NULL) return false;
        if(x->prev==NULL)
        {
            if(x->next==NULL)
            {
                delete x;
                x=start=end=cur=NULL;
            }
            else
            {
                if(x==cur) cur=cur->next;
                x=x->next;
                x->prev->next=NULL;
                x->prev=NULL;
                delete start;
                start=x;
            }
        }
        else
        {
            if(x->next==NULL)
            {
                if(x==cur) cur=cur->prev;
                x=x->prev;
                x->next=NULL;
                delete end;
                end=x;
            }
            else
            {
                if(x==cur) cur=cur->prev;
                x=x->prev;
                x->next=x->next->next;
                x->next->prev->next=NULL;
                delete x->next->prev;
                x->next->prev=x;

            }
        }
        --count;
        return true;
    }

    bool RemGiven(LOPDL &sel)
    {
        if(sel.empty()) return false;
        do
        {
            RemGiven(sel.GetCur());
        }
        while(sel.moveOn());
        sel.~LOPDL();
        return true;
    }

    void RemSketches()
    {
        if(sketchlist.empty()) return;
        do
        {
            RemGiven(sketchlist.GetCur());
        }
        while(sketchlist.moveOn());
        sketchlist.~LOPDL();
    }

    void draw(QPainter &painter)
    {
        dot_list* cur=start;
        while(cur)
        {
            cur->draw(painter);
            cur=cur->next;
        }

    }

    void draw(QPainter &painter, int r, int g, int b)
    {
        dot_list* cur=start;
        while(cur)
        {
            cur->draw(painter,r,g,b);
            cur=cur->next;
        }

    }

    void drawShady(QPainter &painter)
    {
        dot_list* cur=start;
        while(cur)
        {
            cur->drawShady(painter);
            cur=cur->next;
        }

    }

    void drawLast(QPainter &painter)
    {
        end->draw(painter);
    }

    bool moveOn()
    {
        cur=cur->next;
        if(cur==NULL)
        {
            cur=start;
            return false;
        }
        return true;
    }
    dot_list *getCur(){return cur;}
    dot_list *getEnd(){return end;}

    LODL& operator=(LODL &x)  // deal with self assignment? i'm not sure if it's important...
    {
        //              IMPROVE!
        delete start;
        start=end=cur=NULL;
        count=x.count;
        background=x.background;
        while(x.cur)
        {
            Add(new dot_list(*(x.cur)));
            x.cur=x.cur->next;
        }
        x.cur=x.start;
        imageFlags=x.imageFlags;
        if(imageFlags&1) setCompressedImage((unsigned char*) x.imageBuf,x.cmprIMsize);
        else setReadyImage(x.IMwidth,x.IMheight,(int*) x.imageBuf);
        return *this;
    }
    int GetCount(){return count;}
    int SelectInRect(dot_list **sel, QRect rect)
    {
        dot_list* cur=start;
        int i=0;
        while(cur)
        {
            if(cur->IsInRect(rect))
            {
                sel[i]=cur;
                ++i;
            }
            cur=cur->next;
        }
        return i;
    }

    int SelectInRect(dot_list **sel, int t, int b, int l, int r)
    {
        dot_list* cur=start;
        int i=0;
        while(cur)
        {
            if(cur->IsInRect(t,b,l,r))
            {
                sel[i]=cur;
                ++i;
            }
            cur=cur->next;
        }
        return i;
    }

    bool SelectInRect(LOPDL &sel, QRect rect)
    {
        sel.~LOPDL();
        dot_list* cur=start;
        int i=0;
        while(cur)
        {
            if(cur->IsInRect(rect))
            {
                sel.AddDL(cur);
                i++;
            }
            cur=cur->next;
        }
        return i;
    }

    bool SelectInRect(LOPDL &sel, int t, int b, int l, int r)
    {
        sel.~LOPDL();
        dot_list* cur=start;
        int i=0;
        while(cur)
        {
            if(cur->IsInRect(t,b,l,r))
            {
                sel.AddDL(cur);
                i++;
            }
            cur=cur->next;
        }
        return i;
    }

    int getWidth(){return right-left;}
    int getHeight(){return bottom-top;}
    int getTop(){return top;}
    int getLeft(){return left;}
    int getBottotm(){return bottom;}
    int getRight(){return right;}

    void minimalize(bool crop=true)
    {
        dot_list* cur=start;
        left=cur->getLeft();
        right=cur->getRight();
        bottom=cur->getBottom();
        top=cur->getTop();
        while(cur)
        {
            int cl=cur->getLeft(), cr=cur->getRight(), ct=cur->getTop(), cb=cur->getBottom();
            if(cl<left) left=cl;
            if(cr>right) right=cr;
            if(ct<top) top=ct;
            if(cb>bottom) bottom=cb;
            cur=cur->next;
        }
        cur=start;
        while(cur && crop)
        {
            cur->crop(left,top);
            cur=cur->next;
        }
        //cur=start;
    }

    void crop(int l, int t)
    {
        dot_list* cur=start;
        while(cur)
        {
            cur->crop(l,t);
            cur=cur->next;
        }
        top-=t;//-1
        bottom-=t;//-1
        left-=l;//-1
        right-=l;//-1

    }

    void invert()
    {
        dot_list* cur=start;
        while(cur)
        {
            cur->invert((right-left)>>1,(right-left)&1);
            cur=cur->next;
        }
    }

    dot_list* find(unsigned char s)
    {
        if(finder)finder=finder->next;
        else finder=start;
        while(finder && finder->getSize()!=s)  finder=finder->next;
        return finder;
    }

    void moveThoseX(int x)
    {
        dot_list *runner=start;
        while(runner)
        {
            runner->moveX(x);
            runner=runner->next;
        }

    }

    void moveThoseY(int y)
    {
        dot_list *runner=start;
        while(runner)
        {
            runner->moveY(y);
            runner=runner->next;
        }
    }

    void moveThose(int x, int y)
    {
        dot_list *runner=start;
        while(runner)
        {
            runner->move(x,y);
            runner=runner->next;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////
    SimpleColor getBackground(){return background;}
    void setBackground(QColor x){ background=x;}
    int *getReadyImage(){return (int *)imageBuf;}
    bool setReadyImage(QImage &im)
    {
        if(!(im.format()==QImage::Format_ARGB32 || im.format()==QImage::Format_RGB32)) return false;
        unsigned int s=(IMwidth=im.width())*(IMheight=im.height());
        if(imageBuf) delete [] imageBuf;
        imageBuf=new Byte[s*sizeof(int)];
        unsigned char* imb=im.bits();
        if(imb==NULL) return NULL;
        unsigned char * readyImage=(unsigned char*)imageBuf;
        memcpy(readyImage,imb,s<<2);
        return true;
    }

    void setReadyImage(unsigned int w,unsigned int h, int* ARGBarray)
    {
        unsigned int s;
        if(!(IMwidth==w && IMheight==h))
        {
            if(imageBuf) delete [] imageBuf;
            s=(IMwidth=w)*(IMheight=h);
            imageBuf= new Byte[s*sizeof(int)];
        }
        else
        {
            s=w*h;
            if(imageBuf==NULL) imageBuf=new Byte[s*sizeof(int)];
        }
        int* readyImage=(int*)imageBuf;
        for(unsigned int i=0;i<s;++i)
        {
            readyImage[i]=ARGBarray[i];
        }
    }

    void getShadeyImage(uchar *destination, unsigned int width_limit = -1, unsigned int height_limit = -1)
    {
        int* si = (int*)destination;
        unsigned int w=std::min(IMwidth, width_limit/4);
        unsigned int h=std::min(IMheight, height_limit/4);
        int bkg=background.toHex();
        int* readyImage=(int*)imageBuf;
        for(unsigned int k=0;k<w;++k)
            for(unsigned int j=0;j<h;++j)
        {
            unsigned int i = j*w + k;
            if(readyImage[i]==bkg) si[i]=readyImage[i] & 0x00ffffff;
            else si[i]=readyImage[i] - (0xff000000-0x32000000);
        }
    }

    void getBkgTransImage(int *bti)
    {
        unsigned int s=IMwidth*IMheight;
        int bkg=background.toHex();
        int* readyImage=(int*)imageBuf;
        for(unsigned int i=0;i<s;++i)
        {
            if(readyImage[i]==bkg) bti[i]=readyImage[i] & 0x00ffffff;
            else bti[i]=readyImage[i];
        }
    }

    bool compressReadyImage() // not really compression, just some optimiziation... of  memory... hope it'll work for most of the cases
    {
        if(imageFlags&1) return false;
        cmprIMsize=0;
        unsigned int s=IMwidth*IMheight, ss=s<<2;
        if(s==0) return false;
        unsigned int b=background.toHex(), pr=b;
        unsigned int *readyImage=(unsigned int*)imageBuf;
        unsigned char * temp=new unsigned char[ss](); //try to make this static... i think it will perform better that way... in that case use maximum 1024x1024 resolution
        //memset(temp,0,ss);
        --ss;
        for(unsigned int i=0, j=0;i<s;++i)//write compressed data to temp image, while determining size of compressed image...
        {
            if(readyImage[i]==b) pr=b;
            else
            {
                if(readyImage[i]==pr)
                {
                    if(*((unsigned int *)(temp+j))<0x10000000)
                        *((unsigned int *)(temp+j))+=0x10000000;
                    switch(++(temp[cmprIMsize]))
                    {
                    case 0:
                        *((unsigned int *)(temp+j))+=0x10000000;
                        break;
                    case 255:
                        if(*((unsigned int *)(temp+j))>=0xF0000000) pr=b;
                        break;
                    }
                }
                else
                {
                    cmprIMsize+=6+(temp[cmprIMsize] || ((cmprIMsize>0)?(temp[cmprIMsize-1]&0xF0):false));
                    if(cmprIMsize>=ss) //if compressed image size >= ready images' size-1, teriminate everything, return false
                    {
                        delete [] temp;
                        return false;
                    }
                    *((unsigned int *) (temp+cmprIMsize-6))=(pr=readyImage[i])&0x00FFFFFF;
                    *((unsigned int *) (temp+(j=cmprIMsize -4)))|=(i<<8);
                }
            }

        }
        cmprIMsize+=(temp[cmprIMsize] || ((cmprIMsize>0)?(temp[cmprIMsize-1]&0xF0):false));
        delete [] imageBuf;
        imageBuf=new Byte[cmprIMsize]; // create space for compressed image and give it to imageBuf, becouse temp is to big
        memcpy(imageBuf,temp,cmprIMsize);
        delete [] temp; // not required if static
        imageFlags|=1;
        return true;
    }

    void decompressReadyImage()
    {
        if(imageFlags&1)
        {
            unsigned int s=IMwidth*IMheight;
            unsigned int * temp= new unsigned int[s];//new image
            unsigned char * cmprIM =  (unsigned char*)imageBuf;
            unsigned int bkg=background.toHex();
            unsigned int i=0, j=0;
            for(; i<s; ++i) temp[i]=bkg; // fill with background
            i=0;
            //log( "starting to work with possibly undefined behaviour");
            while(i<cmprIMsize) //read compressed data and transform it to new image
            {
                unsigned int k=*((unsigned int* ) (cmprIM+i+3)); // in some cases highest byte of k will be copied from unallocated(?) 'space', and will contain undefined(random) value... it is even possible to get entire int from unallocated(?) 'space'... char *c=new char[2]; int i=*((int*) (c+6)); or even alter it : int * i= (int*)(c+6); ++(*i);... interesting... probably not portable... x86... surpriseing... maybe dangerous...
                j=k&0x000FFFFF;
                temp[j] = *((unsigned int* ) (cmprIM+i)) | 0xff000000;
                if(bkg=k&0x00F00000) // bkg is used as a temporary variable, nothing to do with background
                {
                    k=(bkg>>12)-0x100 + (k>>24) + j;
                    while(k>j)
                    {
                        temp[k]=temp[j];
                        --k;
                    }
                    i+=7;
                }
                else i+=6;
            }
            //log ( "work with possibly undefined behavior succsessfully done");
            delete [] imageBuf;
            imageBuf=(Byte*)temp;//set imageBuf to new image
            imageFlags&=0b11111110;
        }
    }

    bool imageIsCompressed(){ return imageFlags&1;}

    void setCompressedImage(const unsigned char *ci,const unsigned int s)
    {
        delete [] imageBuf;
        cmprIMsize=s;
        imageBuf=new Byte[s];
        memcpy(imageBuf,ci,s);
    }

    void writeImageToFile(FILE *file)
    {
        if(imageFlags&1) fwrite(imageBuf,1,cmprIMsize,file);
        else  fwrite(imageBuf,4,IMwidth*IMheight,file);
    }

    unsigned int getIMwidth(){return IMwidth;}
    unsigned int getIMheight(){return IMheight;}

    unsint getCompressedImageSize(){return cmprIMsize;}

    bool getAA(){return !(imageFlags&2);}
    void setAA(bool value=true){value?imageFlags&~2:imageFlags|=2;}
    void switchAA(){imageFlags^=2;}

};

class LOLODL
{
    LODL filtered; // provides special functianality, that should be used with caution... or maybe shouldn't be used at all...
    LODL *start, *end;
    LODL *cur, *bcur;
    int size, step;
    char *attachment;
    int attlenght;
public:
    bool operator!=(LOLODL &l)
    {
        if(size!=l.size || attlenght!=l.attlenght) return true;
        LODL *runner=end, *lrunner=l.end;
        while(runner)
        {
            if(*runner!=*lrunner) return true;
            runner=runner->prev;
            lrunner=lrunner->prev;
        }
        for(int i=0;i<attlenght;++i)
            if(attachment[i]!=l.attachment[i]) return true;
        return false;
    }

    LOLODL(){start=end=cur=0; size=0; step=50; attachment=0; attlenght=0;}
    ~LOLODL(){delete start; delete [] attachment; start=end=cur=0; attachment=0; size=0; attlenght=0;}

    void Add(LODL *x)
    {
        if(x==0) x=new LODL();
        if(start==0)
        {
            start=x;
            end=start;
        }
        else
        {
            end->next=x;
            end->next->prev=end;
            end=end->next;
        }
        cur=start; // hmmm... really?
        ++size;
    }

    void addToLeft(LODL *x)
    {
        if(x==0) x=new LODL();
        if(cur)
        {
            if(cur==start)
            {
                start=cur->prev=x;
                start->next=cur;
            }
            else
            {
                cur->prev->next=x;
                cur->prev->next->prev=cur->prev;
                cur->prev->next->next=cur;
                cur->prev=cur->prev->next;
            }
        }
        else
        {
            cur=start=end=x;
        }
        ++size;
    }

    void addToRight(LODL *x)
    {
        if(x==0) x=new LODL();
        if(cur)
        {
            if(cur==end)
            {
                end=cur->next=x;
                end->prev=cur;
            }
            else
            {
                cur->next->prev=x;
                cur->next->prev->next=cur->next;
                cur->next->prev->prev=cur;
                cur->next=cur->next->prev;
            }
        }
        else
        {
            cur=start=end=x;
        }
        ++size;
    }

    bool RemCur()
    {
        if(cur==NULL) return false;
        if(cur->prev==NULL)
        {
            if(cur->next==NULL)
            {
                delete cur;
                cur=start=end=NULL;
            }
            else
            {
                cur=cur->next;
                cur->prev->next=NULL;
                cur->prev=NULL;
                delete start;
                start=cur;
            }
        }
        else
        {
            if(cur->next==NULL)
            {
                cur=cur->prev;
                cur->next=NULL;
                delete end;
                end=cur;
            }
            else
            {
                cur=cur->prev;
                cur->next=cur->next->next;
                cur->next->prev->next=NULL;
                delete cur->next->prev;
                cur->next->prev=cur;

            }
        }
        --size;
        return true;
    }
    bool drawAndMoveOn(QPainter &painter)
    {
        bool ret;
        if(cur)
        {
            cur->draw(painter);
            cur=cur->next;
        }
        if(ret=(cur==NULL)) cur=start;
        return !ret;
    }
    void drawCur(QPainter &painter)
    {
        if(cur) cur->draw(painter);
    }
    void drawPrev(QPainter &painter)
    {
        if(cur && cur->prev) cur->prev->drawShady(painter);
    }

    bool moveOn()
    {
        cur=cur->next;
        if(cur==NULL)
        {
            cur=start;
            return false;
        }
        return true;
    }
    bool moveBack()
    {
        cur=cur->prev;
        if(cur==NULL)
        {
            cur=end;
            return false;
        }
        return true;
    }

    bool isEmpty(){return start==0 || (start->getEnd()==0 && size==1);}

    LODL *getCur(){return cur;}
    LODL *getEnd(){return end;}
    void resetCur()
    {
        bcur->compressReadyImage();
        if(bcur->prev) bcur->prev->compressReadyImage();
        else end->compressReadyImage();
        cur=bcur;
        cur->decompressReadyImage();
        if(cur->prev) cur->prev->decompressReadyImage();
    }
    void setCur()
    {
        if(cur->prev)// cur isn't start
        {
            cur->compressReadyImage();
            cur->prev->compressReadyImage();
            start->decompressReadyImage();
        }
        bcur=cur;
        cur=start;
    }
    void setStep(int s) {step=s;}
    int getSize(){return size;}
    int getStep(){return step;}

    LODL filter_size(unsigned char s) //overwrites bcur!
    {
        bcur=start;
        while(bcur)
        {
            filtered.Add(bcur->find(s));
            bcur=bcur->next;
        }
        return filtered;
    }

    int filter_size(unsigned char s, int *&x, int *&y)// ?
    {
        x=new int[size];
        y=new int[size];
        bcur=start;
        int i=0;
        dot_list *f;
        while(bcur)
        {
            f=bcur->find(s);
            x[i]=f->getFirst()->x;
            y[i]=f->getFirst()->y;
            ++i;
            bcur=bcur->next;
        }
        return size;
    }

    bool Load(const char *filename)
    {
        FILE *file;
        this->~LOLODL();
        unsigned char buf[5];

        delete [] attachment;
        attachment=NULL;
        attlenght=0;

        int temp[5];
        if(file=fopen(filename,"rb"))
        {
            fread(&step,sizeof(int),1,file);
            while(fread(buf,1,1,file) && !feof(file))
            {
                switch (buf[0])
                {
                case '=':
                    fread(temp+2,4,3,file);
                    Add(new LODL(temp[2],temp[3], temp[4]>>16, temp[4]>>8, temp[4]));
                    break;
                case '>':
                    fread(buf,1,5,file);
                    end->Add(new dot_list(QColor(buf[0],buf[1],buf[2]),buf[3],buf[4]));
                    break;
                case ' ':
                    fread(temp,sizeof(int),2,file);
                    end->getEnd()->AddDot(temp[0],temp[1]);
                    break;
                case 'M':
                    fread(&attlenght,sizeof(int),1,file);
                    attachment= new char[attlenght];
                    fread(attachment,1,attlenght,file);
                }
            }
            fclose(file);
            return true;
        }
        return false;
    }

    void Save(const char * filename)
    {
        FILE *file;
        bcur=start;
        file=fopen(filename,"wb");
        unsigned char b[6];
        unsigned int temp[3];
        fwrite(&step,sizeof(int),1,file);
        if(attachment)
        {
            b[0]='M';
            fwrite(b,1,1,file);
            fwrite(&attlenght,sizeof(int),1,file);
            fwrite(attachment,1,attlenght,file);
        }
        if(bcur)
        do
        {
            b[0]='=';
            fwrite(b,1,1,file);
            temp[0]=bcur->getIMwidth();
            temp[1]=bcur->getIMheight();
            temp[2]=bcur->getBackground().toHex();
            fwrite(temp,4,3,file);
            if(bcur->getCur())
                do
                {
                    b[0]='>';
                    bcur->getCur()->getRGB(b+1);
                    b[4]=bcur->getCur()->getSize();
                    b[5]=bcur->getCur()->getFlags();
                    fwrite(b,1,6,file);
                    bcur->getCur()->writeTo(file);
                }
                while(bcur->moveOn());
        }
        while(bcur=bcur->next);
        fclose(file);
    }

    char* getAttch(){return attachment;}
    void setAttch(char* str)
    {
        delete [] attachment;
        attlenght=0;
        if(str)
        {
            while(str[attlenght]!='\0') ++attlenght;
            ++attlenght;
            attachment= new char[attlenght];
            memcpy(attachment,str,attlenght);
        }
        else attachment=0;
    }

    int getMinLeft()
    {
        if(start==NULL) return 0;
        int ret=start->getLeft();
        LODL *cur=start->next;
        while(cur)
        {
            int t=cur->getLeft();
            if(ret > t) ret=t;
            cur=cur->next;
        }
        return ret;
    }

    int getMinTop()
    {
        if(start==NULL) return 0;
        int ret=start->getTop();
        LODL *cur=start->next;
        while(cur)
        {
            int t=cur->getTop();
            if(ret > t) ret=t;
            cur=cur->next;
        }
        return ret;
    }

    int getMaxRight()
    {
        if(start==NULL) return 0;
        int ret=start->getRight();
        LODL *cur=start->next;
        while(cur)
        {
            int t=cur->getRight();
            if(ret < t) ret=t;
            cur=cur->next;
        }
        return ret;
    }

    int getMaxBottom()
    {
        if(start==NULL) return 0;
        int ret=start->getBottotm();
        LODL *cur=start->next;
        while(cur)
        {
            int t=cur->getBottotm();
            if(ret < t) ret=t;
            cur=cur->next;
        }
        return ret;
    }

    bool handleCompressionForwords(bool loop=true) // called before moving forwords(onwords)
    {
        if(cur)
        {
            if(cur->prev)
            {
                cur->prev->compressReadyImage();
            }
            else
            {
                end->compressReadyImage();
            }

            if(cur->next)
            {
                cur->next->decompressReadyImage();
            }
            else if(loop || cur==start)
            {
                start->decompressReadyImage();
            }
            return true;
        }
        return false;
    }

    bool handleCompressionBackwords(bool loop=true) // called after moveing backwords
    {
        if(cur)
        {

            if(cur->next)
            {
                cur->next->compressReadyImage();
            }
            else
            {
                start->compressReadyImage();
            }

            if(cur->prev)
            {
                cur->prev->decompressReadyImage();
            }
            else if(loop || cur==end)
            {
                end->decompressReadyImage();
            }

            return true;
        }
        return false;
    }

    void handleCompressionAfterRemoval(bool loop=true)
    {
        if(handleCompressionBackwords(loop) && cur==start) cur->decompressReadyImage();
    }
};


#endif // DOT_LIST_H
