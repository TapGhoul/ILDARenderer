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
    scene->offset = 1;

    if (rotateTime->elapsed()/10 > 256)
        rotateTime->restart();

    int rotate = rotateTime->elapsed()/10;

    ui->statusBar->showMessage(QString("Time: %1").arg(rotate));

    LaserFunctions::cycleColours(scene, rotate);
    scene->offset = 103;
    LaserFunctions::spinSquareEdges(scene, rotate);
}
