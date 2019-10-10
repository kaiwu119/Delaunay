#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "convexhull.h"
#include <QPainter>
#include <QDesktopWidget>
#include <QRect>
#include <QResizeEvent>
#include <QImage>
#include <QPen>
#include <QPaintEvent>
#include <QTime>
#include <QResizeEvent>
#include <QFileDialog>
#include <QBuffer>
#include <QColor>
#include <kdtree.h>
#include <QMoveEvent>
#include <QKeyEvent>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    void paintEvent(QPaintEvent*e);
    ~Widget();
    void resizeEvent(QResizeEvent *e);
    void resizePoints(int num,int change = 1);
    void initDemon();
     void mousePressEvent(QMouseEvent *e);
private slots:
    void on_comboBox_activated(int index);

    void on_genBtn_clicked();

    void on_spinBox_valueChanged(int arg1);


    void on_colorBtn_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_loadBtn_clicked();

    void on_saveBtn_clicked();

    void on_triBtn_clicked();

    void on_oriBtn_clicked();

private:
    Ui::Widget *ui;
    QVector<Point> m_point;
    QVector<Point> add_point;
    ConvexHull myHull;
    QList<Triangle> hullTins;
    QList<Triangle> DelaTins;
    bool isShowColor;
    QRectF box;
    int rand;
    int drawType;
    int drawTag;
    int len ;
    //三角化
    QImage oriImage;
    QImage triImage;
    QImage curImage;
    int imageType;
    QVector<Point> imagePoint;
    QList<Triangle> ImageTins;
    QList<QColor> ImageTinsColor;
    bool isGenTriImg;
};

#endif // WIDGET_H
