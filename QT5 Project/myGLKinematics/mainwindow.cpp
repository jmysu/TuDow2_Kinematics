#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "kinematics.h"
#include "qglviewer.h"

using namespace qglviewer;
//using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("土豆2號 仿生動力學 (TuDow2 Kinematics)");

    viewer = new Viewer();

    QLayout *layout = ui->horizontalLayoutQGL;
    layout->addWidget(viewer);

    viewer->setGridIsDrawn();
    viewer->setAxisIsDrawn();

    //viewer->camera()->frame()->setOrientation(Quaternion(qglviewer::Vec(1, 0.2, 0.2), M_PI/4));
    Quaternion qc = viewer->camera()->frame()->rotation();
    qDebug() << QString("Camera Rotation:(%1,%2,%3) %4°")
             .arg(qc.axis().x)
             .arg(qc.axis().y)
             .arg(qc.axis().z)
             .arg(qRadiansToDegrees(qc.angle()));

    viewer->camera()->setSceneRadius(5);
    //viewer->showEntireScene();
    viewer->camera()->fitSphere(Vec(0, 0, 2), 5);

    viewer->show();

    qreal rBase = viewer->myRobot->radRotation[E0_BASE];
    qreal dBase = qRadiansToDegrees(rBase);
    ui->horizontalSlider1->setRange(-180,180);
    ui->horizontalSlider1->setValue(dBase);

    qreal rArm1 = viewer->myRobot->radRotation[E1_ARM1];
    qreal dArm1 = qRadiansToDegrees(rArm1);
    ui->horizontalSlider2->setRange(-180,180);
    ui->horizontalSlider2->setValue(dArm1);

    qreal rArm2 = viewer->myRobot->radRotation[E2_ARM2];
    qreal dArm2 = qRadiansToDegrees(rArm2);
    ui->horizontalSlider3->setRange(-180,180);
    ui->horizontalSlider3->setValue(dArm2);
qDebug() << "Init slider123" << dBase << dArm1 << dArm2;
on_horizontalSlider1_sliderMoved(dBase);
on_horizontalSlider2_sliderMoved(dArm1);
on_horizontalSlider3_sliderMoved(dArm2);

    ui->radioButtonFK->setChecked(true);
    ui->radioButtonIK->setChecked(false);

    ui->groupBoxFK->setEnabled(true);
    ui->groupBoxIK->setEnabled(false);

    //force teapot upward to World Zaxis
    viewer->myTeapot->frameTeapot->setRotation(Quaternion(Vec(1.0, 0.0, 0.0), M_PI_2)); //on Xaxis @pi/2
    viewer->update();

    //Unselect id when slider released
    connect(ui->horizontalSlider1, &QSlider::sliderReleased, [=](){
        viewer->myRobot->idSelected = E4_NONE;
        viewer->update();
        });
    connect(ui->horizontalSlider2, &QSlider::sliderReleased, [=](){
        viewer->myRobot->idSelected = E4_NONE;
        viewer->update();
        });
    connect(ui->horizontalSlider3, &QSlider::sliderReleased, [=](){
        viewer->myRobot->idSelected = E4_NONE;
        viewer->update();
        });

    Quaternion q = viewer->myTeapot->frameTeapot->rotation();
    qDebug() << QString("Teapot Rotation:(%1,%2,%3) %4°")
             .arg(q.axis().x)
             .arg(q.axis().y)
             .arg(q.axis().z)
             .arg(qRadiansToDegrees(q.angle()));

    //Do some fun--------------------------------
    timer = new QTimer();
    timer->start(100);
    connect(timer, &QTimer::timeout, [=](){
        //Change Axis
        qglviewer::Vec axis;
        //if (ui->radioButtonX->isChecked())
        //    axis = qglviewer::Vec(1.0, 0.0, 0.0);
        //else if (ui->radioButtonY->isChecked())
        //    axis = qglviewer::Vec(0.0, 1.0, 0.0);
        //else if (ui->radioButtonZ->isChecked())
        //    axis = qglviewer::Vec(0.0, 0.0, 1.0);
        axis = qglviewer::Vec(0.0, 1.0, 0.0);
        //Rotate on Axis
        viewer->myTeapot->frameTeapot->rotate(Quaternion(axis, -M_PI/10)); //- pi/10 per step
        viewer->update();
        });
    //-----------------
    //testIK();
    viewer->myRobot->idSelected = E4_NONE;
    updateGroupboxKtitle();

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * Tramsform from XY-plane to ZY-plane
 *
 *       Y                    X
 *       |                    |
 *       |                    |
 *   ----+----X    =>     ----+----Z
 *      /|                   /|
 *     / |                  / |
 *    Z  |                 Y  |
 *     (x,y,z)     =>       (z,x,y)
 *     (0,0,1)     =>       (1,0,0)
 *     (0,1,0)     =>       (0,0,1)
 *     (1,0,0)     =>       (0,1,0)
 *
 */
void MainWindow::updateGroupboxKtitle()
{
qDebug() << Q_FUNC_INFO;
    qreal dBase= ui->horizontalSlider1->value();
    qreal rBase= qDegreesToRadians(dBase);
    qreal dT0  = ui->horizontalSlider2->value();
    qreal rT0  = qDegreesToRadians(dT0);
    qreal dT1  = ui->horizontalSlider3->value();
    qreal rT1  = qDegreesToRadians(dT1);

    qreal l0   = viewer->myRobot->fSegLen[E1_ARM1];
    qreal l1   = viewer->myRobot->fSegLen[E2_ARM2];

    //Calc position on ZY-plane
    qreal z    = KposXfromAnglesRad(rT0, rT1, l0, l1);
    qreal y    = KposYfromAnglesRad(rT0, rT1, l0, l1);

    //Calc x1/y1 after Base rotation
    qreal xx = y*qSin(rBase);
    qreal yy = -y*qCos(rBase);
    qreal zz = z + viewer->myRobot->fSegLen[E0_BASE];

    QString text;
    text.sprintf("Forward:(%+06.3f,%+06.3f,%+06.3f)", xx, yy, zz);
    ui->groupBoxFK->setTitle(text);

    //Update teapot position
    float fscale = viewer->myTeapot->fScale;
    viewer->myTeapot->frameTeapot->setPosition(xx/fscale, yy/fscale, zz/fscale);
}

/*
 *  Base on AxisZ
 *
Letter   Description  Escape-Sequence β, Γ γ
-------------------------------------
A  α     Alpha        \u0391
B  β     Beta         \u0392
Γ  γ     Gamma        \u0393
Δ        Delta        \u0394
Ε        Epsilon      \u0395
Ζ        Zeta         \u0396
Η        Eta          \u0397
Θ        Theta        \u0398
Ι        Iota         \u0399
Κ        Kappa        \u039A
Λ        Lambda       \u039B
Μ        Mu           \u039C
Ν        Nu           \u039D
Ξ        Xi           \u039E
Ο        Omicron      \u039F
Π        Pi           \u03A0
Ρ        Rho          \u03A1
Σ        Sigma        \u03A3
Τ        Tau          \u03A4
Υ        Upsilon      \u03A5
Φ        Phi          \u03A6
Χ        Chi          \u03A7
Ψ        Psi          \u03A8
Ω        Omega        \u03A9
 */

void MainWindow::on_horizontalSlider1_sliderMoved(int position)
{
    int deg = position;
    qDebug() << Q_FUNC_INFO << deg;
    if (viewer) {
        //teapot
        //viewer->mfRotationRad[0] = fRad;
        //viewer->myTeapot->frameTeapot->setRotation(Quaternion(qglviewer::Vec(1.0, 0.0, 0.0),fRad));

        //update robot
        qreal rBase = qDegreesToRadians((qreal)deg);
        viewer->myRobot->frame[E0_BASE]->setRotation( Quaternion(viewer->myRobot->vecRotation[E0_BASE], rBase) );

        if (ui->checkBoxDemo->isChecked())
            viewer->myRobot->idSelected = E3_CLAW;
        else
            viewer->myRobot->idSelected = E0_BASE;
        viewer->update();

        //viewer->tudow.mfRotationRad[E0_TUDOW_BASE] = fRad;
        //viewer->tudow.frame(E0_TUDOW_BASE)->setRotation(Quaternion(qglviewer::Vec(0.0, 0.0, 1.0), viewer->tudow.mfRotationRad[E0_TUDOW_BASE]));   //Z
        //viewer->tudow.mSelectedId = E0_TUDOW_BASE;
        //viewer->tudow.setColor(E0_TUDOW_BASE);

        //update UI
        ui->horizontalSlider1->setValue(deg);
        QString text;
        text.sprintf("%+06.3f,%+4d", rBase, deg);

        ui->labelBase->setText(QString("Base-α(Π,Φ):%1°").arg(text) );

        updateGroupboxKtitle();
        }
}
/*
 * Arm1 on AxisX
 */
void MainWindow::on_horizontalSlider2_sliderMoved(int position)
{
    int deg = position;
    qDebug() << Q_FUNC_INFO << deg;
    if (viewer) {
        //float fRad = (value/180.0f)*3.14f;
        //viewer->mfRotationRad[0] = fRad;
        //viewer->myTeapot->frameTeapot->setRotation(Quaternion(qglviewer::Vec(0.0, 1.0, 0.0), fRad));
        //update robot
        qreal rArm1 = qDegreesToRadians((qreal)deg);
        viewer->myRobot->frame[E1_ARM1]->setRotation( Quaternion(viewer->myRobot->vecRotation[E1_ARM1], rArm1) );

        if (ui->checkBoxDemo->isChecked())
            viewer->myRobot->idSelected = E3_CLAW;
        else
            viewer->myRobot->idSelected = E1_ARM1;
        viewer->update();

        //update UI
        ui->horizontalSlider2->setValue(deg);
        QString text;
        text.sprintf("%+06.3f,%+4d", rArm1, deg);
        ui->labelArm1->setText(QString("Arm1-β(Π,Φ):%1°").arg(text) );
        updateGroupboxKtitle();
        }
}
/*
 * Arm2 on AxisX
 */
void MainWindow::on_horizontalSlider3_sliderMoved(int position)
{
    int deg = position;
    qDebug() << Q_FUNC_INFO << deg;
    if (viewer) {
        //float fRad = (value/180.0f)*3.14f;
        //viewer->mfRotationRad[0] = fRad;
        //viewer->myTeapot->frameTeapot->setRotation(Quaternion(qglviewer::Vec(0.0, 0.0, 1.0), fRad));
        //update robot
        qreal rArm2 = qDegreesToRadians((qreal)deg);
        viewer->myRobot->frame[E2_ARM2]->setRotation( Quaternion(viewer->myRobot->vecRotation[E2_ARM2], rArm2) );

        if (ui->checkBoxDemo->isChecked())
            viewer->myRobot->idSelected = E3_CLAW;
        else
            viewer->myRobot->idSelected = E2_ARM2;
        viewer->update();

        //update UI
        ui->horizontalSlider3->setValue(deg);
        QString text;
        text.sprintf("%+06.3f,%+4d", rArm2, deg);
        ui->labelArm2->setText(QString("Arm2-γ(Π,Φ):%1°").arg(text) );
        updateGroupboxKtitle();
        }
}


void MainWindow::on_radioButtonFK_toggled(bool checked)
{
    if (checked) {
        ui->groupBoxFK->setEnabled(true);
        ui->groupBoxIK->setEnabled(false);
        }
}

void MainWindow::on_radioButtonIK_toggled(bool checked)
{
    if (checked) {
        ui->groupBoxFK->setEnabled(false);
        ui->groupBoxIK->setEnabled(true);
        }
}


/*
 *  Have Fun with TuDow2
 *
 *      The DEMO mode will sweeping for every angle
 *      The Crazy mode; you know; should be looks funny!
 *
 */
void MainWindow::on_checkBoxDemo_clicked()
{
    if (ui->checkBoxDemo->isChecked()) {
        if (!timerDemo) {
            timerDemo = new QTimer;
            }
        timerDemo->start(200);

        connect(timerDemo, &QTimer::timeout, [=](){
            static int iBdiff    = 15;
            static int iArm1diff = 10;
            static int iArm2diff = 10;
            int iBase = ui->horizontalSlider1->value();
            if (ui->checkBoxCrazy->isChecked())
                iBase = qrand()%360 -180;
            else
                iBase += iBdiff;

            if ( (iBase > ui->horizontalSlider1->maximum())
              || (iBase < ui->horizontalSlider1->minimum()) )
                iBdiff = -iBdiff;

            on_horizontalSlider1_sliderMoved(iBase);

            int iArm1 = ui->horizontalSlider2->value();
            if (ui->checkBoxCrazy->isChecked())
                iArm1 = qrand()%180-90;
            else
                iArm1 += iArm1diff;
            if ((iArm1 > 90) || (iArm1 < -90) )
                iArm1diff = -iArm1diff;
            on_horizontalSlider2_sliderMoved(iArm1);

            int iArm2 = ui->horizontalSlider3->value();
            if (ui->checkBoxCrazy->isChecked())
                iArm2 = qrand()%180-90;
            else
                iArm2 += iArm2diff;
            if ((iArm2 > 90) || (iArm2 < -90) )
                iArm2diff = -iArm2diff;
            on_horizontalSlider3_sliderMoved(iArm2);
            });
        }
    else {
        //disconnect(timerDemo);
        if (timerDemo) {
            timerDemo->stop();
            }

        viewer->myRobot->idSelected = E4_NONE;
        viewer->update();
        ui->checkBoxCrazy->setCheckState(Qt::Unchecked);
        }
}
