#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    exit(0);
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
