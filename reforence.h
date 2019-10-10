#ifndef REFORENCE_H
#define REFORENCE_H
#include <QVector>
#include <QDebug>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include <QDebug>
#include <QGLFunctions>
#include <QFileInfo>
#include <cmath>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QFont>
#include <QGLFramebufferObject>
#include <QVector3D>
#include <QQueue>
#define PAI 3.1415
class Point{
public:
    float x,y,z;
    Point(){}
    Point(float x1,float y1,float z1=0){x = x1;y = y1;z = z1;}
    void init(float x1,float y1,float z1=0){x = x1;y = y1;z = z1;}
    bool operator ==(const Point &p)
    {
        return fabs(x-p.x)<= 1e-6&&fabs(y-p.y)<= 1e-6&&fabs(z-p.z)<= 1e-6;
    }
    Point operator +(const Point& p1)
    {
        return Point(p1.x+x,p1.y+y,p1.z+z);
    }
    Point operator -(const Point& p1)
    {
        return Point(x-p1.x,y-p1.y,z-p1.z);
    }

    void print()
    {
        qDebug()<<"("<<x<<","<<y<<","<<z<<")";
    }

};
class vec3{  //向量
public:
    float x,y,z;
    vec3(){}
    vec3(float x1,float y1,float z1=0){x = x1;y = y1;z = z1;}
    vec3(Point a){ x = a.x; y = a.y;z = a.z;}
    vec3(Point a,Point b){ x = b.x-a.x; y = b.y-a.y; z = b.z-a.z; }  //a指向b的向量
    void init(float x1,float y1,float z1=0){x = x1;y = y1;z = z1;}
    bool operator ==(const vec3 &p)
    {
        return fabs(x-p.x)<= 1e-6&&fabs(y-p.y)<= 1e-6&&fabs(z-p.z)<= 1e-6;
    }
    vec3 operator +(const vec3& p1)
    {
        return vec3(p1.x+x,p1.y+y,p1.z+z);
    }
    vec3 operator -(const vec3& p1)
    {
        return vec3(x-p1.x,y-p1.y,z-p1.z);
    }


};
struct Line{
    Point p1,p2; //有向线段，p1指向p2
    Line(){}
    Line(Point p1,Point p2){this->p1 = p1;this->p2 = p2; }
    void init(Point p1,Point p2){this->p1 = p1;this->p2 = p2; }
    bool operator ==(const Line &l)
    {
        return p1==l.p1&& p2 == l.p2;
    }
};


class Gemetry
{
public:
    Gemetry() {}
    static vec3 Cross(vec3 a, vec3 b)//法线
    {
        float x = a.y*b.z - a.z*b.y;
        float y = a.z*b.x - a.x*b.z;
        float z = a.x*b.y - a.y*b.x;
        return  vec3(x,y,z);
    }
    //计算距离
    static float PointToPoint(Point pos1, Point pos2)
    {
        float sum = 0;
        sum+= pow(pos2.x-pos1.x,2)+pow(pos2.y-pos1.y,2)+pow(pos2.z-pos1.z,2);
        return sqrt(sum);
    }
    static float PointToPoint(vec3 pos1, vec3 pos2)
    {
        float sum = 0;
        sum+= pow(pos2.x-pos1.x,2)+pow(pos2.y-pos1.y,2)+pow(pos2.z-pos1.z,2);
        return sqrt(sum);
    }
    //向量的模
    static float Norm(vec3 v){
        float sum = 0;
        sum+= pow(v.x,2)+pow(v.y,2)+pow(v.z,2);
        return sqrt(sum);
    }
    //点到线的距离
    static float PointToLine(Point p,Line l)
    {
        vec3 ab(l.p1,l.p2);
        vec3 ac(l.p1,p);
        return Norm(Cross(ab,ac))/PointToPoint(l.p1,l.p2); //d = (AB x AC)/|AB| ,|AB X AC|/2是三角形ABC的面积，这个三角形的底是|AB|，高就是C到AB的距离
    }

    //    static float angle(float x1,float y1, float x2,float y2){
    //        float n = (x1*x2+y1*y2);
    //        float m = sqrt(x1*x1+y1*y1)*sqrt(x2*x2+y2*y2);
    //        return acos(n/m)*180/PAI;
    //    }
    static float angle3D(Point a,Point b,Point c){
        vec3 v1(a-b);
        vec3 v2(c-b);
        float x1 = v1.x;
        float y1 = v1.y;
        float z1 = v1.z;
        float x2 = v2.x;
        float y2 = v2.y;
        float z2 = v2.z;
        float n = (x1*x2+y1*y2 + z1*z2);
        float m = sqrt(x1*x1+y1*y1+z1*z1)*sqrt(x2*x2+y2*y2 +z2*z2);
        return acos(n/m)*180/PAI;
    }
    //空间两向量夹角
    static float angle3D(QVector3D n1,QVector3D n2)
    {
        float x1 = n1.x();
        float y1 = n1.y();
        float z1 = n1.z();
        float x2 = n2.x();
        float y2 = n2.y();
        float z2 = n2.z();
        float n = (x1*x2+y1*y2 + z1*z2);
        float m = sqrt(x1*x1+y1*y1+z1*z1)*sqrt(x2*x2+y2*y2 +z2*z2);
        return acos(n/m)*180/PAI;
    }
    static float Cross2D(vec3 a, vec3 b)
    {
        return a.x*b.y-b.x*a.y;
    }
    static bool IsRightPoint(vec3 pt, Line line)
    {
        vec3 v1(line.p2.x-line.p1.x,line.p2.y-line.p1.y);//p1p2
        vec3 v2(line.p1.x-pt.x,line.p1.y-pt.y);// pp1
        float tmp =Cross2D(v1,v2);
        if(tmp>0) //大于0在右边
        {
            return true;
        }else
        {
            return false;
        }
    }
    static bool IsOnLine(vec3 pt, Line line)
    {
        vec3 v1(line.p2.x-line.p1.x,line.p2.y-line.p1.y);//p1p2
        vec3 v2(line.p1.x-pt.x,line.p1.y-pt.y);// pp1
        float tmp =Cross2D(v1,v2);
        float minx,miny;
        float maxx,maxy;
        if(line.p1.x < line.p2.x){
            minx = line.p1.x;
            maxx = line.p2.x;
        }
        else{
            minx = line.p2.x;
            maxx = line.p1.x;
        }

        if(line.p1.y < line.p2.y){
            miny= line.p1.y;
            maxy = line.p2.y;
        }
        else{
            miny = line.p2.y;
            maxy = line.p1.y;
        }
        if(fabs(tmp)<=1e-5 && pt.x>minx&&pt.x < maxx&& pt.y>miny && pt.y < maxy) //大于0在右边
        {
            return true;
        }else
        {
            return false;
        }
    }
};
struct Triangle{
    Point p1,p2,p3;//三个点
    Point p[3];
    Line l1,l2,l3;//三条线
    Line l[3];//三条线
    Triangle(){}
    Triangle(Point a,Point b,Point c){ init(a,b,c);}
    bool isInTriangle(Point a)
    {
        bool r1 = Gemetry::IsRightPoint(a,l1);
        bool r2 = Gemetry::IsRightPoint(a,l2);
        bool r3 = Gemetry::IsRightPoint(a,l3);
        if(r1 ==r2&& r2 == r3)
            return true;
        return false;
    }
    int isOnTriangle(Point a)
    {
        bool r1 = Gemetry::IsOnLine(a,l1);
        bool r2 = Gemetry::IsOnLine(a,l2);
        bool r3 = Gemetry::IsOnLine(a,l3);
        if(r1)
            return 1;
        if(r2)
            return 2;
        if(r3)
            return 3;
        return 0;
    }

    void init(Point a,Point b,Point c){
        p1 =a ;
        p2 =b;
        p3 = c;
        p[0] = a;
        p[1] = b;
        p[2] = c;
        l1 = Line(a,b);
        l2 = Line(b,c);
        l3 = Line(c,a);
        l[0] = l1;
        l[1] = l2;
        l[2] = l3;
    }
    int  containsLine(Line l)
    {
        if((l.p1 == p1 && l.p2 == p2) || (l.p1 == p2 && l.p2 == p1) )
            return 1;
        if((l.p1 == p2 && l.p2 == p3) || (l.p1 == p3 && l.p2 == p2) )
            return 2;
        if((l.p1 == p1 && l.p2 == p3) || (l.p1 == p3 && l.p2 == p1) )
            return 3;
        return 0;
    }
    bool operator ==(const Triangle& tri)
    {
        if(p1 == tri.p1 && p2 == tri.p2&& p3 == tri.p3)
            return true;
        return false;
    }
};
struct Circle{
    double radius;
    vec3 center;
    Circle(){}
    Circle(vec3 cent,double r){ center = cent; radius = r;}
    static Circle genTriCircle(Triangle tri){
        Point p1 = tri.p1;
        Point p2 = tri.p2;
        Point p3 = tri.p3;
        double  x1,x2,x3,y1,y2,y3;
        x1  =  p1.x;
        x2  =  p2.x;
        x3  =  p3.x;
        y1  =  p1.y;
        y2  =  p2.y;
        y3  =  p3.y;
        //求外接圆半径
        double a=sqrt( (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2) );
        double b=sqrt( (x1-x3)*(x1-x3)+(y1-y3)*(y1-y3) );
        double c=sqrt( (x2-x3)*(x2-x3)+(y2-y3)*(y2-y3) );
        double p=(a+b+c)/2;
        double S=sqrt( p*(p-a)*(p-b)*(p-c) );

        //求外接圆圆心
        double t1=x1*x1+y1*y1;
        double t2=x2*x2+y2*y2;
        double t3=x3*x3+y3*y3;
        double temp=x1*y2+x2*y3+x3*y1-x1*y3-x2*y1-x3*y2;
        double x=(t2*y3+t1*y2+t3*y1-t2*y1-t3*y2-t1*y3)/temp/2;
        double y=(t3*x2+t2*x1+t1*x3-t1*x2-t2*x3-t3*x1)/temp/2;
        double r=a*b*c/(4*S);
        vec3 cent(x,y,0);
        return Circle(cent,r);

    }
    bool isInCircle(vec3 v)
    {
        double dist = Gemetry::PointToPoint(center,v);
        return dist<radius;
    }
};



class Sort
{
public:
    Sort();

    static void quickSort(QVector<float>&a,int left,int right,bool(*cmp)(float ,float))
    {
        if(left<right){
            int i=left;
            int j=right;
            float  temp=a[left];
            if(left>=right)
                return;
            while(i<j)
            {
                while(i<j&& cmp(temp,a[j]))
                    j--;
                if(j>i)
                    a[i++]=a[j];//a[i]已经赋值给temp,所以直接将a[j]赋值给a[i],赋值完之后a[j],有空位
                while(i<j&&cmp(a[i],temp))
                    i++;
                if(i<j)
                    a[j--]=a[i];
            }
            a[i]=temp;//把基准插入,此时i与j已经相等R[low..pivotpos-1].keys≤R[pivotpos].key≤R[pivotpos+1..high].keys
            quickSort(a,left,i-1,cmp);/*递归左边*/
            quickSort(a,i+1,right,cmp);/*递归右边*/
        }
    }
    static void quickSort(QVector<Point>&a,int left,int right,bool(*cmp)(Point ,Point))
    {
        if(left<right){
            int i=left;
            int j=right;
            Point  temp=a[left];
            if(left>=right)
                return;
            while(i<j)
            {
                while(i<j&& cmp(temp,a[j]))
                    j--;
                if(j>i)
                    a[i++]=a[j];//a[i]已经赋值给temp,所以直接将a[j]赋值给a[i],赋值完之后a[j],有空位
                while(i<j&&cmp(a[i],temp))
                    i++;
                if(i<j)
                    a[j--]=a[i];
            }
            a[i]=temp;//把基准插入,此时i与j已经相等R[low..pivotpos-1].keys≤R[pivotpos].key≤R[pivotpos+1..high].keys
            quickSort(a,left,i-1,cmp);/*递归左边*/
            quickSort(a,i+1,right,cmp);/*递归右边*/
        }
    }
};
//导入纹理
class texture : public QGLWidget,protected QGLFunctions
{
public:
    static void loadTexture(QString filepath, GLuint *texture)
    {
        QImage tex, buf;

        if(!buf.load(filepath))
        {
            printf("Error: failed to load image!");
            exit(1);
        }

        tex = convertToGLFormat(buf);
        glGenTextures(1, texture);

        glBindTexture(GL_TEXTURE_2D, *texture);

        glTexImage2D(GL_TEXTURE_2D,0,4, tex.width(), tex.height(),0,GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
          glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
         glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );

    }

};
#endif // REFORENCE_H
