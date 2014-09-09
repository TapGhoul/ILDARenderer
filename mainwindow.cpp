#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "laserfunctionsilda.h"
#include "ildaserializer.h"
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
    rotAngle = 0;
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

bool canDraw(vector3d pos, vector<vector<vector3d>> points) {
    for (vector<vector<vector3d>>::iterator it = points.begin(); it != points.end(); it++) {
        vector<vector3d> f = *it;
        for (vector<vector3d>::iterator it1 = f.begin(); it1 != f.end(); it1++) {
            vector3d pt1 = *it1;
            vector3d pt2 = *(it1 + 1);
            if (it1 + 1 == f.end()) {
                pt2 = *f.begin();
            }

            if (pos.y > pt1.y == pos.y > pt2.y)
                break;
            if (pos.x >= pt1.x != pos.x >= pt2.x)
                return false;
        }
    }
    return true;
}

void MainWindow::on_action_Import_triggered()
{
    if (md.vertices.size() == 0) {
        md.processData("/home/silvea/Documents/Laser Shit/Objs/Monkey.obj");
        for (int i=0; i < md.vertices.size(); i++) {
            cout << md.vertices[i].pos.x << "," << md.vertices[i].pos.y << "," << md.vertices[i].pos.z << endl;
        }
        disconnect(redrawTimer, SIGNAL(timeout()), this, SLOT(redraw()));
        connect(redrawTimer, SIGNAL(timeout()), this, SLOT(spinImport()));
    }
}

void MainWindow::spinImport()
{
    scene->clearScene();
    scene->offsetX = 0;
    scene->offsetY = 0;

    if (rotateTime->elapsed()/10 > 511) {
        rotateTime->restart();
    }

    double rotDeg = rotateTime->elapsed()*M_1_PI/256;

    ui->statusBar->showMessage(QString("Time: %1").arg(rotateTime->elapsed()/10));

    scene->setColour(Qt::white);
    vector<vector<vector3d>> points;
    if (ui->spinObject->isChecked())
    {
        vector3d rotAng;
        rotAng.x = 0.05;
        rotAng.y = 0.03;
        rotAng.z = 0.02;
        md.rotate(rotAng);
    }
    vector<face *> facesToDraw = md.filterVisible(ui->allowedOverlaps->value());
    for (int i=facesToDraw.size()-1; i > -1; i--) {
        face * f = facesToDraw[i];
        scene->setBlanking(true);
        vector<vector3d> pointVec;
        for (int j=0; j < f->verts.size()+1; j++) {
            vertex v = *f->verts[j != f->verts.size() ? j : 0];
            vector3d vNorm;
            vector3d vPoint = v.pos;
            if (f->canDraw && f->canDraw1)
                scene->setColour(QColor(127, 255, 255, 255));
            else
                if (ui->hiddenLineVisible->isChecked())
                    scene->setColour(QColor(255, 0, 255, (vPoint.z + 1) * 96));
            else
                    continue;
#if FALSE
            >> YO COMPILER IS A DUM DUM <<
            vNorm.x = v.pos.x - md.center.x;
            vNorm.y = v.pos.y - md.center.y;
            vNorm.z = v.pos.z - md.center.z;

            //vPoint.x = vNorm.x * sin(rotDeg) + vNorm.y * -cos(rotDeg);
            //vPoint.y = vNorm.y * sin(rotDeg) + vNorm.z * -cos(rotDeg);
            //vPoint.z = vNorm.z * sin(rotDeg) + vNorm.x * -cos(rotDeg);


            // Euler rotation, order XYZ
            vPoint.x = vNorm.x;
            vPoint.y = vNorm.y * cos(rotDeg) - vNorm.z * sin(rotDeg);
            vPoint.z = vNorm.z * cos(rotDeg) + vNorm.y * sin(rotDeg);

            vNorm = vPoint;

            vPoint.x = vNorm.x * cos(rotDeg) + vNorm.z * sin(rotDeg);
            vPoint.z = vNorm.z * cos(rotDeg) - vNorm.x * sin(rotDeg);

            vNorm = vPoint;

            vPoint.x = vNorm.x * cos(rotDeg) - vNorm.y * sin(rotDeg);
            vPoint.y = vNorm.y * cos(rotDeg) + vNorm.x * sin(rotDeg);

            //if (!canDraw(vPoint, points))
                //break;
#endif
            //if (!canDraw(vPoint, points))
                //break;

            scene->setPos(vPoint.x * -100 + 100, vPoint.y * -100 + 100);
            if (j == 0) {
                scene->setBlanking(false);
            } else {
                pointVec.insert(pointVec.end(), vPoint);
            }
        }
        if (pointVec.size() > 0)
            points.insert(points.end(), pointVec);
    }

    /*vector3d vPoint, vNorm;
    vNorm.x = 1;
    vNorm.y = 1;
    vNorm.z = 1;

    // Euler rotation, order XYZ
    vPoint.x = vNorm.x;
    vPoint.y = vNorm.y * cos(rotDeg) - vNorm.z * sin(rotDeg);
    vPoint.z = vNorm.z * cos(rotDeg) + vNorm.y * sin(rotDeg);

    vNorm = vPoint;

    vPoint.x = vNorm.x * cos(rotDeg) + vNorm.z * sin(rotDeg);
    vPoint.z = vNorm.z * cos(rotDeg) - vNorm.x * sin(rotDeg);

    vNorm = vPoint;

    vPoint.x = vNorm.x * cos(rotDeg) - vNorm.y * sin(rotDeg);
    vPoint.y = vNorm.y * cos(rotDeg) + vNorm.x * sin(rotDeg);


    scene->setBlanking(true);
    scene->setColour(Qt::red);
    //scene->setPos(sin(rotDeg)*100 + 100, 100);
    scene->setPos(vPoint.y*100 + 100, vPoint.z*100 + 100);
    scene->setBlanking(false);
    scene->setPos(100, 100);
    scene->setColour(Qt::green);
    //scene->setPos(100, -cos(rotDeg)*100 + 100);
    scene->setPos(vPoint.x*100 + 100, vPoint.z*100 + 100);
    scene->setBlanking(true);
    scene->setPos(100, 100);
    scene->setColour(Qt::blue);
    scene->setBlanking(false);
    //scene->setPos(cos(rotDeg)*100 + 100, -sin(rotDeg)*100 + 100);
    scene->setPos(vPoint.x*100 + 100, vPoint.y*100 + 100);*/
    scene->setBlanking(true);
}

void MainWindow::on_stepSpin_clicked()
{
    if (!ui->spinObject->isChecked())
    {
        vector3d rotAng;
        rotAng.x = 0.05;
        rotAng.y = 0.03;
        rotAng.z = 0.02;
        md.rotate(rotAng);
    }
}

void MainWindow::on_spinObject_toggled(bool checked)
{
    ui->stepSpin->setEnabled(!checked);
}
