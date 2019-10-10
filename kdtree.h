#ifndef KDTREE_H
#define KDTREE_H
#include <QVector3D>
#include <QRectF>
#include <QVector>
#include <QDebug>
#include <QtAlgorithms>
#include <cmath>
#include <time.h>
struct Range{   //空间域的数据结构
    float Left,Right,Top,Bottom,Front,Back;
    float left(){return Left;}
    float right(){return Right;}
    float top(){return Top;}
    float bottom(){return Bottom;}
    float front(){return Front;}
    float back(){return Back;}
    float height(){return Bottom-Top;}
    float width(){return Right-Left;}
    float length(){return Back-Front;}
    Range(float left,float top,float front,float right,float bottom,float back)
    {
         Left=left;
         Right=right;
         Top=top;
         Bottom=bottom;
         Front=front;
         Back=back;
    }
    Range(){}
};
struct Data{
    QVector3D value;
    int index;
    Data(){}
    Data(QVector3D v,int i)
    {value = v;index = i; }
    bool operator ==(const Data& d)
    {
        if(d.index==index&& d.value == value)
            return true;
        else
            return false;
    }
};


struct KdNode{
//    QVector3D data;
    Data data;
    Range range;
    int split; //确定分割哪个平面 0：x ，1：y ，2：z；
    KdNode *left;//左子树
    KdNode *right;//右子树
    KdNode *father;//父节点
    KdNode()
    {
        split = -1;
        left = NULL;
        right = NULL;
        father = NULL;
    }
};

class KdTree
{
public:
    KdTree();
    void init(Range range,QVector<QVector3D>data_set); //初始化函数
    void init(Range range,QVector<Data>data_set); //初始化函数
    void sortTree();
    Data search(KdNode* node , QVector3D pos, bool *mask); //寻找最近邻
    QVector<Data> search(KdNode *node,float radius,QVector3D pos);//寻找范围点
    KdNode *header() const;
    float dist(QVector3D p1,QVector3D p2);
private:
    KdNode *m_header;
    QVector<Data> m_data; //初始数据集
    QVector<Data> m_datax;
    QVector<Data> m_datay;
    QVector<Data> m_dataz;
    KdNode* BuildTree(Range range, QVector<Data> data_set, KdNode *parent);//建立kd树
    void KDTraverse(KdNode* node);  //输出函数
    float variance(QVector<Data> &a, int dimen); //方差

    bool cmp(const QVector3D a,const QVector3D b,int demin)
    {
        if(demin == 0)
            return a.x()<b.x();
        if(demin == 1)
            return a.y()<b.y();
        if(demin == 2)
            return a.z()<b.z();

    }
    void quickSort(QVector<Data>&a,int left,int right,int demin)
    {
        if(left<right){
            int i=left;
            int j=right;
            Data  temp=a[left];
            if(left>=right)
                return;
            while(i<j)
            {
                while(i<j&& cmp(temp.value,a[j].value,demin))
                    j--;
                if(j>i)
                    a[i++]=a[j];//a[i]已经赋值给temp,所以直接将a[j]赋值给a[i],赋值完之后a[j],有空位
                while(i<j&&cmp(a[i].value,temp.value,demin))
                    i++;
                if(i<j)
                    a[j--]=a[i];
            }
            a[i]=temp;//把基准插入,此时i与j已经相等R[low..pivotpos-1].keys≤R[pivotpos].key≤R[pivotpos+1..high].keys
            quickSort(a,left,i-1,demin);/*递归左边*/
            quickSort(a,i+1,right,demin);/*递归右边*/
        }
    }


};

#endif // KDTREE_H
