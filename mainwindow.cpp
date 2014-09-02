#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "laserfunctionsilda.h"
#include "ildaserializer.h"
#include "modeldata.h"
#include <vector>
#include <iostream>
#include <fstream>
//#include <QFileDialog>
#include <QMessageBox>

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
    ExportILDA();
}

void MainWindow::on_pushButton_clicked()
{
    ExportILDA();
}

void MainWindow::ExportILDA() {
    vector<vector<coordinate_data>> allData(256);
    for (int i = 0; i < 255; i++) {
        allData[i] = LaserFunctionsILDA::spinCubeYZ(i);
    }
    vector<char> chrsVec = ILDASerializer::coordinates(allData);
    vector<char> colsVec = ILDASerializer::colourTable();
    //QString filename = QFileDialog::getSaveFileName(this, "Save File", QDir::currentPath(), "ILDA v5.1 (*.ild)");
    //string fn = "012356789";
    //fn.substr(fn.length() - 3);
    //filename = filename.isNull() ? "Null!" : filename.endsWith(".ild", Qt::CaseInsensitive) ? filename : filename + ".ild";
    //QMessageBox::information(this, "Title", filename, QMessageBox::Ok | QMessageBox::Close);
    ofstream out("out.ild", std::ofstream::binary);
    out.write((const char*)colsVec.data(), colsVec.size());
    out.write((const char*)chrsVec.data(), chrsVec.size());
    out.close();
    QMessageBox::information(this, "Success", "Successfully exported ILDA file!", QMessageBox::Ok);
}

void MainWindow::on_action_Import_triggered()
{
    ModelData md;
    md.processData("/home/silvea/Documents/Laser Shit/Objs/Cube.obj");
    for (int i=0; i < md.vertices.size(); i++) {
        cout << md.vertices[i].pos.x << "," << md.vertices[i].pos.y << "," << md.vertices[i].pos.z << endl;
    }
}
