#ifndef CONVEXHULL_H
#define CONVEXHULL_H
#include <QVector>
#include <QVector3D>
#include <QList>
#include "sort.h"
#include <QDebug>
extern bool cmpXandY(Point p1,Point p2);
extern bool cmpXsubY(Point p1,Point p2);
class ConvexHull
{
    bool IsLeftPoint(Point pt, Line line);
    bool IsRightPoint(Point pt, Line line);
    float Cross2D(Point a,Point b);
    QList<Point> hullpts;//保存凸包点
    QList<Point> dispts;//保存离散点
    QList<Triangle> tins;//保存三角形


public:
    ConvexHull();
    void generateHull(QVector<Point> pts);
    QList<Triangle> DivideHell(QList<Point> pts);
    QList<Point> getHull(){return hullpts;}
    QList<Triangle> getTins() const;
    QList<Triangle> getDelaunay(QList<Triangle> hulltins,QList<Point> pts);
    QList<Point> getDispts() const;
};

#endif // CONVEXHULL_H
