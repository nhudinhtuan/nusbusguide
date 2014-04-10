#include "mapgraphicsview.h"
#include <QDebug>

MapGraphicsView::MapGraphicsView(QGraphicsScene *scene, QWidget *parent):
    QGraphicsView(scene, parent)
{
    setCacheMode(CacheBackground);

    setAutoFillBackground(false);
    setBackgroundBrush(QBrush(Qt::white, Qt::SolidPattern));
    setMouseTracking(true);
    setRenderHint(QPainter::Antialiasing, false);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setDragMode(ScrollHandDrag);
    setWindowTitle(tr("Nus Map Guide"));

    zoomFactor_ = 1.0;
}

MapGraphicsView::~MapGraphicsView() {
}

QRectF MapGraphicsView::getGraphViewRect() {
    return QRectF(mapToScene(0,0), mapToScene(width(), height()));
}

QList<QGraphicsItem*> MapGraphicsView::getSeenItems() {
    return scene()->items(getGraphViewRect());
}

QPoint& MapGraphicsView::getMouseAnchor() {
    return mouseAnchor_;
}

void MapGraphicsView::setZoomFactor(double factor) {
    zoomFactor_ = factor;
}

void MapGraphicsView::mouseMoveEvent(QMouseEvent *event) {
    QGraphicsView::mouseMoveEvent(event);
}

//scrolls the view and after the mouse is released, triggers the signal
void MapGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    //QPointF after = mapToScene(event->pos());
    //qDebug() << "after (" << after.x() << "," << after.y() << ")";
    //qreal tan = (mapAnchor_.ry() - after.ry())/(after.rx() - mapAnchor_.rx());
    //qreal angle = qAtan(tan);
    //qDebug() << "angle = " << angle;
}

void MapGraphicsView::mousePressEvent(QMouseEvent *event)
{
    mouseAnchor_ = event->pos();
    mapAnchor_ = mapToScene(mouseAnchor_);
    if(event->buttons() == Qt::RightButton) {
        qDebug() << (int)(mapAnchor_.x()) << "," << (int)(mapAnchor_.y());
    }
    QGraphicsView::mousePressEvent(event);
}

void MapGraphicsView::wheelEvent(QWheelEvent *event)
{
    double numDegrees = event->delta()/8.0;
    double numSteps = numDegrees/15.0;
    double factor = pow(1.125, numSteps);
    zoomFactor_ = zoomFactor_*factor;
    scale(factor, factor);

    emit announceZoomFactorChange(zoomFactor_);
    emit announceMapviewScroll();
}

void MapGraphicsView::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
}

