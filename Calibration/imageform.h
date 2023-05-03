#ifndef IMAGEFORM_H
#define IMAGEFORM_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

#include "kfc.h"

namespace Ui {
class ImageForm;
}

class MainFrame;
class ImageForm : public QWidget
{
    Q_OBJECT

private:
    Ui::ImageForm   *ui;
    QString         _q_stID;
    MainFrame*      _q_MainFrame;

public:
    QImage*         _q_bmpMain;
    KImageGray      _igMain;
    KImageColor     _icMain;
    KImageWord      _iwMain;
    double          _dScale;

public:
    explicit ImageForm(const QString& q_stFile,  const QString& q_stID, MainFrame* owner);
    explicit ImageForm(const KImageColor& icImg, const QString& q_stID, MainFrame* owner);    
    explicit ImageForm(const KImageGray& igImg,  const QString& q_stID, MainFrame* owner);
    ~ImageForm();

    KImageGray&     ImageGray(){    return  _igMain;   }
    KImageColor&    ImageColor(){   return  _icMain;   }
    QString&        ID(){        return  _q_stID;   }
    void            Update(const KImageGray&  igImg);
    void            Update(const KImageColor& icImg);

    void            DrawEllipse(const QPoint& q_ptCenter,  int nRx, int nRy, const QColor& qColor, int nWidth=1);
    void            DrawPolygon(QRectF q_Rect, const QPolygon& q_pPolygon,const QColor& q_Color, int nWidth=1,QColor* pBrush=0);
    void            DrawPolygon(const QPolygon& q_Polygon, const QColor& q_Color, int nWidth=1, QColor* pBrush=0);
    void            DrawLines(const QPolygon& q_Polygon, const QColor& q_Color, int nWidth=1);
    void            DrawPoints(const QPolygon&  q_pPolygon, const QColor&, int nWidth=1);
    void            DrawLine(int nXo,int nYo, int nX1, int nY1,const QColor& q_Color, int nWidth=1);
    void            DrawText(int nX, int nY, const QString& q_stTxt, const QColor& q_Color, int nPixelSize);


protected:
    void            paintEvent(QPaintEvent* event);
    void            focusInEvent(QFocusEvent *event);
    void            focusOutEvent(QFocusEvent *event);
    void            closeEvent(QCloseEvent *event);
    void            mousePressEvent(QMouseEvent *event);
    void            resizeEvent(QResizeEvent *event);

private slots:

};

#endif // IMAGEFORM_H
