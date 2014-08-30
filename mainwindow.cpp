#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "laserfunctionsilda.h"
#include "ildaserializer.h"
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new LaserScene)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);

    rotateTime = new QTime();
    rotateTime->start();

    redrawTimer = new QTimer(this);
    connect(redrawTimer, SIGNAL(timeout()), this, SLOT(redraw()));
    redrawTimer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    exit(0);
}

void MainWindow::redraw() {
    // Prepare for new frame
    scene->clearScene();
    scene->offsetX = 1;
    scene->offsetY = 1;

    int rotate = rotateTime->elapsed()/10;

    if (rotateTime->elapsed()/10 > 255) {
        rotateTime->restart();
        rotate = 0;
    }

    ui->statusBar->showMessage(QString("Time: %1").arg(rotate));

    LaserFunctions::cycleColours(scene, rotate);
    scene->offsetX = 103;
    scene->offsetY = 103;
    LaserFunctions::spinSquareEdges(scene, rotate);
    scene->offsetX = 51;
    scene->offsetY = 108;
    LaserFunctions::spinCubeZ(scene, rotate);
    scene->offsetX = 103;
    scene->offsetY = 51;
    LaserFunctions::spinCubeY(scene, rotate);
    scene->offsetX = 51;
    scene->offsetY = 244;
    LaserFunctions::spinCubeYZ(scene, rotate);
}

void MainWindow::on_action_Export_triggered()
{
    vector<vector<coordinate_data>> allData(256);
    for (int i = 0; i < 255; i++) {
        allData[i] = LaserFunctionsILDA::spinCubeYZ(i);
    }
    vector<char> chrsVec = ILDASerializer::coordinates(allData);
    vector<char> colsVec = ILDASerializer::colourTable();
    ofstream out("out.ild", std::ofstream::binary);
    for (int i=0; i < colsVec.size(); i++) {
        out.write((const char*)&colsVec.data()[i], 1);
    }
    for (int i=0; i < chrsVec.size(); i++) {
        out.write((const char*)&chrsVec.data()[i], 1);
    }
    out.close();
}
