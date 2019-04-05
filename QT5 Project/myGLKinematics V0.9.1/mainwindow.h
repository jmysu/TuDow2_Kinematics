#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QTimer>
#include <QtMath>
#include "gl_viewer.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Viewer viewer;

    //Forward Kenematics related,
    //Robot Alpha, Beta, Gamma
    void updateGroupboxFK();
    void FKabr2XYZ();
    qreal rRobotAlpha, rRobotBeta, rRobotGamma;
    void updateRobotByAlphaBetaGamma(qreal Alpha, qreal Beta, qreal Gamma);
    void demoRobotAlphaBetaGamma();

    //Inverse Kenematics related,
    //Robot X,Y,Z (End-Effector Teapot postion)
    void updateGroupboxIK();
    void IKxy2A1A2(qreal x, qreal y, int *A1, int *A2);
    int iRobotX, iRobotY, iRobotZ;
    void updateTeapotByXYZ(qreal x, qreal y, qreal z);
    void demoRobotXYZ();
    bool bIKerror=false;

    QTimer timerTeapot, timerDemo;

public slots:
    void slotDemoTimeout();

private:
    Ui::MainWindow *ui;

private slots:
    void on_sliderAlpha_sliderMoved(int position);
    void on_sliderBeta_sliderMoved(int position);
    void on_sliderGamma_sliderMoved(int position);

    void on_radioButtonFK_toggled(bool checked);
    void on_radioButtonIK_toggled(bool checked);


    void on_checkBoxDemo_clicked();
    void on_checkBoxTeapot_toggled(bool checked);
    void on_sliderIK_X_sliderMoved(int position);
    void on_sliderIK_Y_sliderMoved(int position);
    void on_sliderIK_Z_sliderMoved(int position);

    void on_pushButtonIKgo_clicked();
};

#endif // MAINWINDOW_H
