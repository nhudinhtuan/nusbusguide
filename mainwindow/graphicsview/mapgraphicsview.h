#ifndef MAPGRAPHICSVIEW_H
#define MAPGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QtCore/qmath.h>
#include <QColor>
#include <QDebug>
#include <math.h>

class MapGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MapGraphicsView(QGraphicsScene *scene, QWidget *parent);
    virtual ~MapGraphicsView();

    double getZoomFactor() {return zoomFactor_; }
    void setZoomFactor(double factor);
    QRectF getGraphViewRect();
    QList<QGraphicsItem*> getSeenItems();
    QPoint& getMouseAnchor();

protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent *event);

private:

public slots:

signals:
    void announceZoomFactorChange(double zoomFactor);
    void announceMapviewScroll();

private:
    double zoomFactor_;
    QPoint mouseAnchor_;
    QPointF mapAnchor_;
};

#endif // MAPGRAPHICSVIEW_H
