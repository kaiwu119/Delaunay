#include "kdtree.h"

KdTree::KdTree()
{

}

void KdTree::init(Range range, QVector<QVector3D> data_set)
{
    if(data_set.size() ==0)
        return;

    for( int i =0; i< data_set.size(); i++)
    {
        m_data.push_back(Data(data_set[i],i));
    }

    m_header = BuildTree(range,m_data,NULL);
//    KDTraverse(m_header); 输出

}

void KdTree::init(Range range, QVector<Data> data_set)
{
    if(data_set.size() ==0)
        return;

    for( int i =0; i< data_set.size(); i++)
    {
        m_data.push_back(data_set[i]);
    }

    m_header = BuildTree(range,m_data,NULL);
}

void KdTree::sortTree()
{

//    quickSort(m_datax,0,m_datax.size()-1,0);
//    quickSort(m_datay,0,m_datay.size()-1,1);
//    quickSort(m_dataz,0,m_dataz.size()-1,2);
//    for( int i = 0; i<m_datay.size(); i++)
//    {
//        qDebug()<<m_datay[i];
//    }
//    qDebug()<<variance(m_datax,0);
    //    qDebug()<<variance(m_datay,1);
}

Data KdTree::search(KdNode *node, QVector3D pos,bool *mask)
{
    if(node == NULL)
        return Data(QVector3D(-1000,-1000,1000),-1);
    KdNode *p = node;
    while(p->left!=NULL || p->right!=NULL)
    {
        int split = p->split;
        if( pos[split]<= p->data.value[split])
        {
            if(p->left == NULL)
                break;
            p= p->left;
        }else
        {
           if(p->right == NULL)
               break;
           p=p->right;
        }
    }
    float minDis = 100000.0;
    Data result = p->data;
    minDis = dist(pos,p->data.value);
    if(result.value == pos || mask[result.index]==true)
    {
        minDis = 100000.0;
    }
    KdNode* q = p;
    KdNode* tmp = q;
    while(q!= node)
    {
        q= tmp->father;
        int split = q->split;
        float tmp_dis = dist(q->data.value,pos);
        if(tmp_dis < minDis && mask[q->data.index] == false)
        {
            minDis = tmp_dis;
            result = q->data;
        }
        if(fabs(pos[split]-q->data.value[split]) <(minDis))
            /* 如果以data为中心的圆（球或超球），半径为minDis的圆与分割超平面相交， 那么就要跳到另一边的子空间去搜索 */
        {
            Data tmpResult;
            if(tmp == q->left)
            {
                tmpResult = search(q->right,pos,mask);
            }else if(tmp == q->right)
            {
                tmpResult = search(q->left,pos,mask);

            }

            if(dist(tmpResult.value,pos) < minDis &&mask[tmpResult.index] == false)
            {
                minDis = dist(tmpResult.value,pos);
                result = tmpResult;
            }

        }
        tmp= q;
    }
    return result;
}

QVector<Data> KdTree::search(KdNode *node,float radius, QVector3D pos)
{
    QVector<Data> result;
    if(node == NULL)
        return result;

    KdNode *p = node;
    while(p->left!=NULL || p->right!=NULL)
    {
        int split = p->split;
        if( pos[split]<= p->data.value[split])
        {
            if(p->left == NULL)
                break;
            p= p->left;
        }else
        {
           if(p->right == NULL)
               break;
           p=p->right;
        }
    }
    float minDis = 100000.0;
     minDis = dist(pos,p->data.value);
    if(minDis <= radius )
    result.push_back(p->data);

    KdNode* q = p;
    KdNode* tmp = q;
    while(q!= node)
    {
        q= tmp->father;
        int split = q->split;
        float tmp_dis = dist(q->data.value,pos);
        if(tmp_dis <= radius)
        {

            result.push_back(q->data);
        }
        if(fabs(pos[split]-q->data.value[split]) <= radius)
        {
            QVector<Data> tmpResult;
            if(tmp == q->left)
            {
                tmpResult = search(q->right,radius,pos);
            }else if(tmp == q->right)
            {
                tmpResult = search(q->left,radius,pos);

            }
            for( int i= 0; i< tmpResult.size() ;i++)
            if(dist(tmpResult[i].value,pos) <= radius)
            {
               result.push_back(tmpResult[i]);
            }

        }
        tmp= q;
    }
    return result;
}

KdNode *KdTree::header() const
{
    return m_header;
}

KdNode* KdTree::BuildTree(Range range, QVector<Data> data_set, KdNode *parent)
{
    float vx =0,vy = 0,vz = 0;
    vx = variance(data_set,0);
    vy = variance(data_set,1);
    vz = variance(data_set,2);
    KdNode * node  =new KdNode();

//qDebug()<<parent;
    if(data_set.size() ==0)
        return NULL;
    node->father = parent;

    if(vx>=vy&&vx>=vz)
    {
      quickSort(data_set,0,data_set.size()-1,0);
      node->split = 0;
      node->data = data_set[data_set.size()/2];

      node->range = range;

      Range left  =Range(range.left(),range.top(),range.front(),node->data.value.x()-range.left(),range.height(),range.length());
      Range right =Range(node->data.value.x(),range.top(),range.front(),range.right()-node->data.value.x(),range.height(),range.length());
      QVector<Data> leftdata ;
      QVector<Data> rightdata;
      for( int i=0; i< data_set.size();i++)
      {
          if(i<data_set.size()/2)
              leftdata.push_back(data_set[i]);
          if(i>data_set.size()/2)
              rightdata.push_back(data_set[i]);
      }
       data_set.remove(data_set.size()/2);

      node->left = BuildTree(left , leftdata,node);
      //node->left->father
      node->right = BuildTree(right, rightdata,node);

    }else if(vy>=vx&&vy>=vz)
    {
        quickSort(data_set,0,data_set.size()-1,1);
        node->split = 1;
        node->data = data_set[data_set.size()/2];

        node->range = range;
        Range left  =Range(range.left(),range.top(),range.front(),range.width(),node->data.value.y()-range.top(),range.length());
        Range right =Range(range.left(),node->data.value.y(),range.front(),range.width(),range.bottom()-node->data.value.y(),range.length());
        QVector<Data> leftdata;
        QVector<Data> rightdata;
        for( int i=0; i< data_set.size();i++)
        {
            if(i<data_set.size()/2)
                leftdata.push_back(data_set[i]);
            if(i>data_set.size()/2)
                rightdata.push_back(data_set[i]);
        }

        data_set.remove(data_set.size()/2);
        node->left = BuildTree(left , leftdata,node);
        node->right = BuildTree(right, rightdata,node);
    }else if(vz>= vx&& vz>=vy)
    {
        quickSort(data_set,0,data_set.size()-1,2);
        node->split = 2;
        node->data = data_set[data_set.size()/2];

        node->range = range;
        Range left  =Range(range.left(),range.top(),range.front(),range.width(),range.height(),node->data.value.z());
        Range right =Range(range.left(),range.top(),node->data.value.z(),range.width(),range.height(),range.back()-node->data.value.z());
        QVector<Data> leftdata;
        QVector<Data> rightdata;
        for( int i=0; i< data_set.size();i++)
        {
            if(i<data_set.size()/2)
                leftdata.push_back(data_set[i]);
            if(i>data_set.size()/2)
                rightdata.push_back(data_set[i]);
        }

        data_set.remove(data_set.size()/2);
        node->left = BuildTree(left , leftdata,node);
        node->right = BuildTree(right, rightdata,node);
//        qDebug()<<"z";
    }
    return node;

}

void KdTree::KDTraverse(KdNode *node)
{
    if(node == NULL)
        return ;
    qDebug()<<node->data.value;
    KDTraverse(node->left);
    KDTraverse(node->right);
}

float KdTree::variance(QVector<Data> &a,int dimen)
{
    QVector3D sum(0,0,0);
    for( int i =0; i< a.size(); i++)
    {
        sum+=a[i].value;
    }
    QVector3D mind=sum/a.size();
    //qDebug()<<mind;
    float var = 0;

    if(dimen == 0){
        for( int i =0; i< a.size(); i++)
        {
            var+=(a[i].value.x()- mind.x() )*(a[i].value.x()- mind.x() );
        }
        return sqrt(var);
    }
    if(dimen == 1){
        for( int i =0; i< a.size(); i++)
        {
            var+=(a[i].value.y()- mind.y() )*(a[i].value.y()- mind.y() );
        }
        return sqrt(var);
    }
    if(dimen == 2){
        for( int i =0; i< a.size(); i++)
        {
            var+=(a[i].value.z()- mind.z() )*(a[i].value.z()- mind.z() );
        }
        return sqrt(var);
    }
    return -1;
}

float KdTree::dist(QVector3D p1, QVector3D p2)
{
    if(fabs(p1.x()-p2.x())<=1e-6&&fabs(p1.y()-p2.y())<=1e-6&&fabs(p1.z()-p2.z())<=1e-6 )
        return 100000.0;
    float dist = 0;
    for(int i =0; i< 3; i++)
    {
        dist+= (p1[i]-p2[i])*(p1[i]-p2[i]);
    }
    return sqrt(dist);
}
