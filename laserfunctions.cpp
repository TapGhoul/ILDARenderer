#define SPINYZCENTER true
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
    scene->setColour(QColor(time, 255-time, 0));
    scene->setPos(100, 100);
    scene->setBlanking(true);
    scene->setPos(0, 100);
    scene->setColour(QColor(255-time, time, 0));
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

void LaserFunctions::spinCubeZ(LaserScene *scene, int time)
{
    double angle = (time*M_PI)/128.0f-M_PI;
    int sX = sin(angle)*50;
    int sY = cos(angle)*50;
    int height = 75;

    scene->setBlanking(true);
    scene->setColour(Qt::white);
    scene->setPos(sX, 0);
    scene->setBlanking(false);
    scene->setPos(sY, 10);
    scene->setPos(sY, height+10);
    scene->setPos(-sX, height+10);
    scene->setPos(-sX, 10);
    scene->setPos(-sY, 0);
    scene->setPos(-sY, height);
    scene->setPos(sX, height);
    scene->setPos(sX, 0);
    scene->setPos(-sY, 0);
    scene->setBlanking(true);
    scene->setPos(-sY, height);
    scene->setBlanking(false);
    scene->setPos(-sX, height+10);
    scene->setBlanking(true);
    scene->setPos(-sX, 10);
    scene->setBlanking(false);
    scene->setPos(sY, 10);
    scene->setBlanking(true);
    scene->setPos(sY, height+10);
    scene->setBlanking(false);
    scene->setPos(sX, height);
    scene->setBlanking(true);
}

void LaserFunctions::spinCubeY(LaserScene *scene, int time)
{
    double angle = (time*M_PI)/128.0f-M_PI;
    int sX = sin(angle)*50;
    int sY = cos(angle)*50;
    int width = 75;
    scene->setBlanking(true);
    scene->setColour(Qt::white);
    scene->setPos(0, sX);
    scene->setBlanking(false);
    scene->setPos(10, sY);
    scene->setPos(width+10, sY);
    scene->setPos(width+10, -sX);
    scene->setPos(10, -sX);
    scene->setPos(0, -sY);
    scene->setPos(width, -sY);
    scene->setPos(width, sX);
    scene->setPos(0, sX);
    scene->setPos(0, -sY);
    scene->setBlanking(true);
    scene->setPos(width, -sY);
    scene->setBlanking(false);
    scene->setPos(width+10, -sX);
    scene->setBlanking(true);
    scene->setPos(10, -sX);
    scene->setBlanking(false);
    scene->setPos(10, sY);
    scene->setBlanking(true);
    scene->setPos(width+10, sY);
    scene->setBlanking(false);
    scene->setPos(width, sX);
    scene->setBlanking(true);
}

void LaserFunctions::spinCubeYZ(LaserScene *scene, int time)
{
    double angle = (time*M_PI)/128.0f-M_PI;
    int sX = sin(angle)*50;
    int sY = cos(angle)*50;
    int scale = 25;

    bool doBlank = true;

    scene->setBlanking(true);
    scene->setColour(Qt::white);
    scene->setPos(sX, sY);
    scene->setBlanking(false);
    scene->setPos(sY, -sX);
    scene->setPos(scale+sY, scale-sX);
    scene->setPos(scale-sX, scale-sY);
    scene->setPos(-sX, -sY);
    scene->setPos(-sY, sX);
    scene->setPos(scale-sY, scale+sX);
    scene->setPos(scale+sX, scale+sY);
    scene->setPos(sX, sY);
    scene->setPos(-sY, sX);
#if SPINYZCENTER
    scene->setColour(Qt::red);
    scene->setPos(scale/2, scale/2);
#else
    scene->setBlanking(true);
#endif
    scene->setPos(scale-sY, scale+sX);
#if SPINYZCENTER
    scene->setColour(Qt::white);
#else
    scene->setBlanking(false);
#endif
    scene->setPos(scale-sX, scale-sY);
#if SPINYZCENTER
    scene->setColour(Qt::green);
    scene->setPos(scale/2, scale/2);
#else
    scene->setBlanking(true);
#endif
    scene->setPos(-sX, -sY);
#if SPINYZCENTER
    scene->setColour(Qt::white);
#else
    scene->setBlanking(false);
#endif
    scene->setPos(sY, -sX);
#if SPINYZCENTER
    scene->setColour(Qt::blue);
    scene->setPos(scale/2, scale/2);
#else
    scene->setBlanking(true);
#endif
    scene->setPos(scale+sY, scale-sX);
#if SPINYZCENTER
    scene->setColour(Qt::white);
#else
    scene->setBlanking(false);
#endif
    scene->setPos(scale+sX, scale+sY);
#if SPINYZCENTER
    scene->setColour(QColor(255, 0, 255));
    scene->setPos(scale/2, scale/2);
    scene->setPos(sX, sY);
#endif
    scene->setBlanking(true);
}
