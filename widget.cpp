#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QDesktopWidget* desktopWidget = QApplication::desktop();

    QFile qssfile(":/qss/blue.qss");
        qssfile.open(QFile::ReadOnly);
        QString qssstr = qssfile.readAll();
        qssfile.close();


    //获取设备屏幕大小
    QRect screenRect = desktopWidget->screenGeometry();
    int x = screenRect.width();
    int y = screenRect.height();
    //    this->setGeometry(150,200,x-300,y-300);
    this->setGeometry(x/4,y/4,x/2,y/2);
    rand = 0;
    int pad = 200;
    int w = this->width() - pad;
    int h = this->height()-pad/2;

    ui->tabWidget->setGeometry(pad/2,10,w,h/6);
    ui->tabWidget->setCurrentIndex(0);
    resizePoints(20);
    ui->spinBox->setValue(20);
    ui->comboBox->setCurrentIndex(3);
    this->setWindowTitle("Delaunay三角剖分_by_wk");
    drawType =3;
    isShowColor = false;
    drawTag =0;
    imageType = -1;  //负一表示没有导入图片
    isGenTriImg =false;
    this->setStyleSheet(qssstr);


}
void Widget::initDemon()
{

}

void Widget::mousePressEvent(QMouseEvent *e)
{
    if(drawTag == 0&&box.contains(e->pos()))
    {

        if(e->buttons() & Qt::LeftButton )
        {

            add_point.push_back(Point(e->pos().x(),e->pos().y()));
            m_point.push_back(Point(e->pos().x(),e->pos().y()));
        }
        myHull.generateHull(m_point);
        hullTins = myHull.DivideHell(myHull.getHull());
        DelaTins=myHull.getDelaunay(myHull.getTins(),myHull.getDispts());
        update();
    }
}
void Widget::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.setBrush(QColor(255,255,255));
    int w = this->width();
    int h = this->height();
    p.drawImage(QRectF(0,0,w,h),QImage(":/img/4.png"));
    p.drawRoundRect(box,1,1);
    QPen pen;
    if(drawTag == 0){
        if(drawType == 0){

            pen.setColor(QColor(250,0,0));
            pen.setWidthF(5);
            p.setPen(pen);

            for(int i =0; i< len; i++)
            {
                p.drawPoint(QPointF(m_point[i].x,m_point[i].y));
            }
        }
        if(drawType == 1|| drawType == 2||drawType == 3){
            pen.setColor(QColor(250,0,0));
            pen.setWidthF(7);
            p.setPen(pen);
            QList<Point> mv = myHull.getHull();
            for(int i =0; i< mv.size(); i++)
            {
                p.drawPoint(QPointF(mv[i].x,mv[i].y));
            }
            pen.setColor(QColor(250,0,0));
            pen.setWidthF(7);
            p.setPen(pen);
            QList<Point> dispts = myHull.getDispts();
            for(int i =0; i< dispts.size(); i++)
            {
                p.drawPoint(QPointF(dispts[i].x,dispts[i].y));
            }
            pen.setColor(QColor(62,190,124));
            pen.setWidthF(3);
            p.setPen(pen);

            for(int i =0; i< mv.size(); i++) //凸包
            {
                if(i == mv.size()-1)
                    p.drawLine(QPointF(mv[i].x,mv[i].y),QPointF(mv[0].x,mv[0].y));
                else
                    p.drawLine(QPointF(mv[i].x,mv[i].y),QPointF(mv[i+1].x,mv[i+1].y));
            }
        }
        if(drawType == 2 ){

            pen.setColor(QColor(62,190,124));
            pen.setWidthF(1);
            p.setPen(pen);

            QList<Triangle> m_tri = hullTins;  //绘制三角面片
            for(int i = 0; i< m_tri.size(); i++)
            {
                Triangle tri = m_tri[i];
                Point a,b,c;
                a = tri.p1;
                b = tri.p2;
                c = tri.p3;
                p.drawLine(QPointF(a.x,a.y),QPointF(b.x,b.y));
                p.drawLine(QPointF(a.x,a.y),QPointF(c.x,c.y));
                p.drawLine(QPointF(c.x,c.y),QPointF(b.x,b.y));
            }
        }

        if(drawType == 3){
            pen.setColor(QColor(10,10,10));
            //        pen.setColor(QColor(62,190,124));
            pen.setWidthF(1);
            p.setPen(pen);
            p.setBrush(QColor(255,255,255));
            QList<Triangle> m_tri = DelaTins;  //绘制三角面片
            for(int i = 0; i< m_tri.size(); i++)
            {
                Triangle tri = m_tri[i];
                QPointF tripts[3];
                for(int j = 0; j< 3;j++)
                    tripts[j] = QPointF(tri.p[j].x,tri.p[j].y);
                p.drawPolygon(tripts,3);
            }

        }
        if(drawType == 4){
            pen.setColor(QColor(10,10,10,0));
            //        pen.setColor(QColor(62,190,124));
            pen.setWidthF(0.1);
            p.setPen(pen);

            QList<Triangle> m_tri = DelaTins;  //绘制三角面片
            for(int i = 0; i< m_tri.size(); i++)
            {
                Triangle tri = m_tri[i];
                QPointF tripts[3];
                for(int j = 0; j< 3;j++)
                    tripts[j] = QPointF(tri.p[j].x,tri.p[j].y);
                int ran = qrand()%80+176;
                int r = ran/10*9;
                int g =ran;
                int b = 255;
                p.setBrush(QColor(r,g,b));
                p.drawPolygon(tripts,3);
            }

        }

    }
    if(drawTag == 1)
    {

        if(imageType == 0)
        {
            p.drawImage(box.x(),box.y(),oriImage);
        }
        if(imageType == 1)
        {
            if(isGenTriImg == false)
            {
                QPainter p;
                p.begin(&triImage);
                pen.setColor(QColor(10,10,10,0));
                //        pen.setColor(QColor(62,190,124));
                pen.setWidthF(0.1);
                p.setPen(pen);

                QList<Triangle> m_tri = ImageTins;  //绘制三角面片
                for(int i = 0; i< m_tri.size(); i++)
                {
                    Triangle tri = m_tri[i];
                    QPointF tripts[3];
                    for(int j = 0; j< 3;j++)
                        tripts[j] = QPointF(tri.p[j].x,tri.p[j].y);

                    p.setBrush(ImageTinsColor[i]);
                    p.drawPolygon(tripts,3);
                }

                isGenTriImg =true;
            }
            p.drawImage(box.x(),box.y(),triImage);

        }
    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::resizeEvent(QResizeEvent *e)
{


    int pad = this->height()/10;
    int w = this->width() - pad;
    int h = this->height()-pad/2;

    ui->tabWidget->setGeometry(pad/2,10,w,h/6);
    box = QRectF(pad/2,h/5,w,h/10*8 );
    resizePoints(len,0);
    float rateh = float(oriImage.height())/box.height();
    float ratew =float(oriImage.width())/box.width();
    if(rateh > ratew)
        oriImage = oriImage.scaled(oriImage.width()/rateh,oriImage.height()/rateh);
    else
        oriImage = oriImage.scaled(oriImage.width()/ratew,oriImage.height()/ratew);
    curImage = oriImage;
    triImage = triImage.scaled(oriImage.width(),oriImage.height());
    update();
}

void Widget::resizePoints(int num, int change)
{
    if(change == 1)
        rand++;
    qDebug()<<rand<<"begin";

    m_point.clear();

    int pad = this->height()/10;
    int w = this->width()-pad;
    int h = this->height()-pad/2;

    qsrand(rand);

    len = num;

    for( int i =0; i< len; i++){
        float x = (float)(qrand()%w)+pad/2;
        float y = (float)(qrand()%(h/5*4))+h/5;
        m_point.push_back(Point(x,y));
    }
//    box = QRectF(pad/2,pad/3*2,w,h );
    box = QRectF(pad/2,h/5,w,h/10*8 );
    myHull.generateHull(m_point);
    hullTins = myHull.DivideHell(myHull.getHull());
    DelaTins=myHull.getDelaunay(myHull.getTins(),myHull.getDispts());


}



void Widget::on_comboBox_activated(int index)
{
    drawType =index;
    update();
}

void Widget::on_genBtn_clicked()
{
    int num = ui->spinBox->value();
    if(num>=1){
        resizePoints(num);
        update();
    }
}

void Widget::on_spinBox_valueChanged(int arg1)
{
    if(arg1<=0)
        return;
    resizePoints(arg1,0);
    update();
}


void Widget::on_colorBtn_clicked()
{
    if(isShowColor ==false){
        drawType = 4;
        ui->colorBtn->setText(QString("去除颜色"));
    }else
    {
        drawType = 3;
        ui->colorBtn->setText(QString("生成色彩"));
    }
    isShowColor =!isShowColor;

    update();
}

void Widget::on_tabWidget_currentChanged(int index)
{
    drawTag = index;
    update();
}

void Widget::on_loadBtn_clicked()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this,
                                            tr("选择图像"),
                                            "",
                                            tr("Images (*.png *.bmp *.jpg *.tif *.GIF)"));
    if(filename!=NULL){
        imageType =0;
        isGenTriImg =false;
        oriImage.load(filename);
        float rateh = float(oriImage.height())/box.height();
        float ratew =float(oriImage.width())/box.width();
        if(rateh > ratew)
            oriImage = oriImage.scaled(oriImage.width()/rateh,oriImage.height()/rateh);
        else
            oriImage = oriImage.scaled(oriImage.width()/ratew,oriImage.height()/ratew);

        curImage = oriImage;
        update();
    }
}

void Widget::on_saveBtn_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Save Image"),"",tr("Images (*.png *.bmp *.jpg)"));
    if(filename!=NULL){

        QFile file(filename);
        if (!file.open(QIODevice::ReadWrite))
        {
            return;
        }
        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);
        triImage.save(&buffer, "PNG");
        file.write(ba);
    }
}

void Widget::on_triBtn_clicked()
{
    if(imageType!= -1)
    {
        imageType = 1;
        if(isGenTriImg == true){
         update();
         return;
        }

        curImage = Image::TransToEdge(oriImage);
        triImage = curImage;
        qDebug()<<"ori:"<<oriImage.width();
        qDebug()<<"cur:"<<oriImage.width();
        QVector<QVector3D> points1;
        int h =triImage.height();
        int w = triImage.width();
        for( int i = 0; i< h; i++){
            for(int j = 0; j < w; j++){
                if(QColor(triImage.pixel(Image::getIndex(j,w),Image::getIndex(i,h))) == QColor(255,255,255))
                    points1.push_back(QVector3D(j,i,0));
            }
        }
        int max = 10000;
        QVector<QVector3D> points;
        bool maskPoint[points1.size()];
        memset(maskPoint,0,points1.size()*sizeof(bool));
//        if(points1.size() > max)
//        {
//            for( int i =0; i< max;i++){
//                int index =qrand()%points1.size();
//                while( maskPoint[index] ==true)
//                {
//                    index =qrand()%points1.size();

//                }
//                maskPoint[index]=true;
//                points.push_back(points1[index]);
//            }
//        }else{
//            points = points1;
//        }
                points = points1;

//                if(points.size()>max)
//                {
//                    int num = points.size()-max;
//                    for( int i =0; i< num;i++){
//                    int index =qrand()%points.size();
//                    points.remove(index);
//                    }

//                }

        qDebug()<<"img pts"<<points.size();
        imagePoint.clear();
        ImageTinsColor.clear();
        KdTree mtree;
        Range range(0,0,0,w,h,0);
        mtree.init(range,points);
        bool mask[points.size()];
        memset(mask,0,points.size()*sizeof(bool));
        for( int i =0; i< points.size(); i++)
        {
            if(mask[i] == false){
                imagePoint.push_back(Point(points[i].x(),points[i].y()));
                QVector<Data> neighbor = mtree.search(mtree.header(),20,points[i]);
                for( int j = 0; j< neighbor.size();j++)
                {
                    mask[neighbor[j].index] = true;
                }
                mask[i] = true;
            }

        }
        qDebug()<<"final pts"<<imagePoint.size();

        qsrand(rand);

        int addNum = 100;

        for( int i =0; i< addNum; i++){
            float x = (float)(qrand()%w);
            float y = (float)(qrand()%h);
            imagePoint.push_back(Point(x,y));
        }
        imagePoint.push_back(Point(0,0));
        imagePoint.push_back(Point(w-1,0));
        imagePoint.push_back(Point(w-1,h-1));
        imagePoint.push_back(Point(0,h-1));
        ConvexHull tmpHull;
        tmpHull.generateHull(imagePoint);
        tmpHull.DivideHell(tmpHull.getHull());
        ImageTins=tmpHull.getDelaunay(tmpHull.getTins(),tmpHull.getDispts());
        QPoint OffsetInside[8] = {
            {-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}
        };
        triImage = oriImage;
        qDebug()<<"bengin gen color!";
        for( int i =0; i< ImageTins.size(); i++)
        {
            //            QList<Point> tinPts;
            QList<QColor> color;

            //            tinPts.push_back(ImageTins[i].p1);
            //            while(tinPts.size()>0){
            //                Point bp = tinPts.front();

            //                triImage.setPixel(bp.x,bp.y,qRgb(255,255,255));
            //                tinPts.pop_front();
            //                for( int j =0; j< 8;j++)
            //                {
            //                    Point p = Point( bp.x + OffsetInside[j].x(), bp.y + OffsetInside[j].y());
            //                    int x = p.x;
            //                    int y = p.y;
            //                    QColor curColor =QColor(triImage.pixel(Image::getIndex(x,w),Image::getIndex(y,h)));
            //                    if(curColor!=QColor(255,255,255)  &&ImageTins[i].isInTriangle(p))
            //                    {
            //                        tinPts.push_back(p);

            //                        color.push_back( curColor );
            //                    }
            //                }
            //            }
            for( int j  =0; j< 3;j++)
            {
                QColor curColor =QColor(triImage.pixel(ImageTins[i].p[j].x,ImageTins[i].p[j].y));
                color.push_back(curColor);
            }
            int r,g,b;
            r=g=b=0;
            for(int j = 0; j< color.size();j++)
            {
                r += color[j].red();
                g += color[j].green();
                b += color[j].blue();
            }
            r/=(color.size()+0.1);
            g/=(color.size()+0.1);
            b/=(color.size()+0.1);
            ImageTinsColor.push_back(QColor(r,g,b));

        }

        qDebug()<<"gen tri ok!";
        update();
    }
}

void Widget::on_oriBtn_clicked()
{
    if(imageType!= -1)
    {
        imageType = 0;
        update();
    }
}
