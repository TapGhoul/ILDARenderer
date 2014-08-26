#ifndef LASERSCENE_H
#define LASERSCENE_H

#include <QGraphicsScene>
//#include <QPen>

class LaserScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit LaserScene(QObject *parent = 0);
    int offset = 1;
    float scale = 1.0f;

signals:

public slots:
    void setColour(QColor newColour);
    void setPos(int newX, int newY);
    void setBlanking(bool blank);
    void clearScene();

private:
    QPen *laser;
    int xPos;
    int yPos;
    bool blanking;

};

#endif // LASERSCENE_H
