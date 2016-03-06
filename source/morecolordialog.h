#ifndef MORECOLORDIALOG_H
#define MORECOLORDIALOG_H

#include <QDialog>

namespace Ui {
    class MoreColorDialog;
}

class MoreColorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MoreColorDialog(QWidget *parent = 0);
    ~MoreColorDialog();
    void Init(QColor *c, QAction *ac);
    void setChanged();
    void setup();
    void select(QColor *c);
    void setColor(QColor col);

private slots:
    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_CLOSE_BTN_clicked();


private:
    Ui::MoreColorDialog *ui;

    char fixed_color;
    QColor* color;
    QRect main_col_rect, slide_col_rect, color_selecting_rect, selected_color_rect, slider_rect;
    QImage main_col;
    QImage slide_col;
    QPixmap outer_layer;
    QAction *pipact;
    bool changed, LMB;

    void paintEvent(QPaintEvent *me);
    void mousePressEvent(QMouseEvent *me);
    void mouseMoveEvent(QMouseEvent *me);
    void mouseReleaseEvent(QMouseEvent *me);
    void closeEvent(QCloseEvent *ce);
};

#endif // MORECOLORDIALOG_H
