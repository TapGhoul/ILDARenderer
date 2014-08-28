#define VISIBLEBLANKING false
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
    newX += offsetX;
    newY += offsetY;
    newX *= scale;
    newY *= scale;
#if !VISIBLEBLANKING
    if (!blanking)
#endif
        addLine(xPos, yPos, newX, newY, *laser);

    xPos = newX;
    yPos = newY;
}

void LaserScene::setBlanking(bool blank) {
#if VISIBLEBLANKING
    laser->setWidthF(blank ? 0.5 : 1.25);
#else
    blanking = blank;
#endif
}

void LaserScene::clearScene() {
    clear();
}
