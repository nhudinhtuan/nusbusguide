#ifndef BUSSTOPPOPUP_H
#define BUSSTOPPOPUP_H
#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class BusStopPopUp;
}

class BusStopPopUp : public QWidget
{
    Q_OBJECT

public:
    explicit BusStopPopUp(QWidget *parent = 0);
    void displayStatic(QString& name, QPoint pos);
    ~BusStopPopUp();

signals:
    void closeSig();

private slots:
    void closeAction();

protected:
    void mousePressEvent(QMouseEvent *evt);
    void mouseMoveEvent(QMouseEvent *evt);

private:
    Ui::BusStopPopUp *ui_;
    QPoint oldPos_;
};

#endif // BUSSTOPPOPUP_H
