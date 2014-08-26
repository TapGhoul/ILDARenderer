#include "laserscene.h"

LaserScene::LaserScene(QObject *parent) :
    QGraphicsScene(parent),
    xPos(0), yPos(0), blanking(true)
{
    setBackgroundBrush(QBrush(Qt::black));
    laser = new QPen(QColor(255, 255, 255));
    laser->setWidthF(1.25);
}

void LaserScene::setColour(QColor newColour) {
    laser->setColor(newColour);
}

void LaserScene::setPos(int newX, int newY) {
    newX += offset;
    newY += offset;
    newX *= scale;
    newY *= scale;
    if (!blanking)
        addLine(xPos, yPos, newX, newY, *laser);

    xPos = newX;
    yPos = newY;
}

void LaserScene::setBlanking(bool blank) {
    blanking = blank;
}

void LaserScene::clearScene() {
    clear();
}
