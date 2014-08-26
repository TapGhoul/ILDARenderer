#include <QtCore>
#include <QColor>
#include "laserfunctions.h"

using namespace std;

LaserFunctions::LaserFunctions()
{
}

void LaserFunctions::cycleColours(LaserScene *scene, int time)
{
    scene->setBlanking(true);
    scene->setColour(Qt::white);
    scene->setPos(0, 0);
    scene->setBlanking(false);
    scene->setPos(0, 100);
    scene->setPos(100, 100);
    scene->setPos(100, 0);
    scene->setPos(0, 0);
    scene->setColour(QColor(time, 256-time, 0));
    scene->setPos(100, 100);
    scene->setBlanking(true);
    scene->setPos(0, 100);
    scene->setColour(QColor(256-time, time, 0));
    scene->setBlanking(false);
    scene->setPos(100, 0);
    scene->setBlanking(true);
}

void spinSquareEdgePos(int &x, int &y, int time)
{
    x = time > 127 ? max((192-time)*100/64, 0) : min(time*100/64, 100);
    y = time > 127 ? min(100, max((256-time)*100/64, 0)) : max(min((time-64)*100/64, 100), 0);
}

void LaserFunctions::spinSquareEdges(LaserScene *scene, int time)
{
    //int sX = time > 127 ? max((192-time)*100/64, 0) : min(time*100/64, 100);
    //int sY = time > 127 ? min(100, max((256-time)*100/64, 0)) : max(min((time-64)*100/64, 100), 0);

    //int &sX = 0, &sY = 0;
    int sX = 0, sY = 0;

    spinSquareEdgePos(sX, sY, time);

    scene->setBlanking(true);
    scene->setColour(Qt::red);
    scene->setPos(sX, sY);
    scene->setBlanking(false);
    switch (time/64) {
    case 0:
        scene->setPos(100, 0);
        scene->setPos(100, 100);
        scene->setPos(100-sX, 100-sY);
        scene->setColour(Qt::green);
        scene->setPos(0, 100);
        scene->setPos(0, 0);
        break;
    case 1:
        scene->setPos(100, 100);
        scene->setPos(0, 100);
        scene->setPos(100-sX, 100-sY);
        scene->setColour(Qt::green);
        scene->setPos(0, 0);
        scene->setPos(100, 0);
        break;
    case 2:
        scene->setPos(0, 100);
        scene->setPos(0, 0);
        scene->setPos(100-sX, 100-sY);
        scene->setColour(Qt::green);
        scene->setPos(100, 0);
        scene->setPos(100, 100);
        break;
    case 3:
        scene->setPos(0, 0);
        scene->setPos(100, 0);
        scene->setPos(100-sX, 100-sY);
        scene->setColour(Qt::green);
        scene->setPos(100, 100);
        scene->setPos(0, 100);
        break;
    default:
        break;
    }
    spinSquareEdgePos(sX, sY, time);
    scene->setPos(sX, sY);
    scene->setColour(Qt::blue);
    scene->setPos(100-sX, 100-sY);
    scene->setBlanking(true);
}
