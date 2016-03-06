#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSound>
#include "dot_list.h"
#include "colordialog.h"
//#include "saveasdialog.h"
//#include "opendialog.h"
#include "sizedialog.h"
#include "attachmusicdialog.h"
#include "loadingdialog.h"




namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void Init(char *fn, char *dir=NULL);


private:
    Ui::MainWindow *ui;
    LOLODL book; // stores all animation data
    LOLODL redobook; // stores things that have been undone
    LODL *copy_pointer; // points to a frame to be copied
    LOPDL selecting_list; // points to drawings that have been selected
    LODL copy_list; // stores drawings that have been selected

    QTimer timer, // the actual animation timer
           selection_color_change_timer, // timer for animating the selection effect
           MMrepaint_timer; // timer for repainting when drawing(it's too heavy to repaint each time the mouse move event occurs)
    QImage ShImage; // stores the shady image
    QPixmap outer_layer; // some temporary stuff like selector rect are drawn here, to be easily removed...
    QPixmap pix; // main pixmap animation is drawn on
    QSound *mus;

    int MMrepaint_count; // how many new dotlist are there to repaint, since the MMrepaint_timer triggered
    int current_step; // current delay between frames in miliseconds
    int page; // stores the current page number
    QColor background_color; // last selected background color
//    QRect prect; // the area to be redrawn
    QRect selector; // selector rectangle
    QColor curcol; // currently selected pen color
    unsigned char cur_size; // currently selected pen size
    char *filename;
    char *FileDir; // directory path extracted from the filename
    char *ProgDir; // directory path extracted from the argv[0], i assume this is the program directory
    int PDlenght; // lenght of ProgDir
    int FDlenght; // lenght of FileDir

    ColorDialog *coldi,
                *bkg_coldi; // nod used
    SizeDialog *sdi;
//    SaveAsDialog SAD;
//    OpenDialog OD;
    AttachMusicDialog AMD;
    LoadingDialog *LD;

    bool RMB, LMB;
    bool shade;
    bool Ctrl, Shift, Alt;
    bool MM; // true when drawing
    bool Redone, pasted;
    bool LineMode, line_drawn, break_line, SketchMode, SelectingMode;
    bool repainted_by_me; // so that nothing heavy happens when the window is moved or covered or stuff..
    bool frame_changed;
    bool selected,
         selected_is_black; // the selcted drawing is flickering(between white and black) according to this
    bool pip; // the pippete tool active
    bool new_file; // new file was created

    bool debuger; // this is a dummy

    void mouseMoveEvent(QMouseEvent *me);
    void mousePressEvent(QMouseEvent *me);
    void mouseReleaseEvent(QMouseEvent *me);
    void keyPressEvent(QKeyEvent *ke);
    void keyReleaseEvent(QKeyEvent *ke);
    void paintEvent(QPaintEvent *pe);
    void closeEvent(QCloseEvent *qe);
    void resizeEvent(QResizeEvent *re);

    void CUndo(); // continious undraw
    enum {SAVE, THATS_FINE, WAIT_A_MINUTE}; // states retuned by following function
    int CheckForChngesToSave();
    const QRect prect() const;

private slots:
    void changeFrame();
    void Play();
    void Save(bool no_check=false);
    void Load(bool no_check=false);
    void Next();
    void Prev();
    void Delete();
    void Undo();
    void Redo();
    void Color();
    void SaveAs();
    void Open();
    void SizeOfPen();
    void Copy();
    void Paste();
    void LineToggle(bool on);
    void SketchToggle(bool on);
    void SelectingToggle(bool on);
    void AttachMusic();
    void DetachMusic();
    void TogglePipette(bool p);
    void ResetTools();
    void setBackgroundColor();
    void insertFrame();
    void New();
    void MMrepaint(); // speacial repaint function(connected to a timer) to improove drawing performance... i hope... GRRRMRRRIAAHHH!!! this turned out greeaat!..

};

#endif // MAINWINDOW_H
