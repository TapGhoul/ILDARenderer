#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QTimer>
#include "laserscene.h"
#include "laserfunctions.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionQuit_triggered();
    void redraw();

    void on_action_Export_triggered();

private:
    Ui::MainWindow *ui;
    LaserScene *scene;
    QTimer *redrawTimer;
    QTime *rotateTime;

};

#endif // MAINWINDOW_H
