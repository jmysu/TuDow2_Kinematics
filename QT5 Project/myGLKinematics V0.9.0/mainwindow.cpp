/*
 *  土豆2號仿生動力學 (TuDow2 Kinematics)
 *
 *  by Jimmy.Su, Apr.1, 2019
 *
 *  See https://github.com/jmysu/TuDow2_Kinematics
 *
 */
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
    //this->setWindowTitle("土豆2號仿生動力學 (TuDow2 Kinematics)");
    this->setWindowTitle(APP_NAME+QString(" V")+APP_VERSION);

    QLayout *layout = ui->horizontalLayoutQGL;
    layout->addWidget(&viewer); //Add QGLViewer widget to layout

    viewer.setGridIsDrawn();
    viewer.setAxisIsDrawn();

    //Adjust cammera range, orientation
    viewer.camera()->frame()->setOrientation(Quaternion(qglviewer::Vec(1, 0.2, 0.2), M_PI/4)); //Init viewer in iso orientation
    viewer.camera()->setSceneRadius(5);
    viewer.camera()->fitSphere(Vec(0, 0, 2), 5);
    viewer.show();

    //Unselect Robot segment id
    viewer.myRobot.idSelected = E4_NONE;
    //Unselect id when FK slider released
    connect(ui->sliderAlpha, &QSlider::sliderReleased, [=](){
        viewer.myRobot.idSelected = E4_NONE;
        viewer.myTeapot.bSelected = false;
        viewer.update();
        });
    connect(ui->sliderBeta, &QSlider::sliderReleased, [=](){
        viewer.myRobot.idSelected = E4_NONE;
        viewer.myTeapot.bSelected = false;
        viewer.update();
        });
    connect(ui->sliderGamma, &QSlider::sliderReleased, [=](){
        viewer.myRobot.idSelected = E4_NONE;
        viewer.myTeapot.bSelected = false;
        viewer.update();
        });
    /*
    //Unselect teapot when IK slider released
    connect(ui->sliderIK_X, &QSlider::sliderReleased, [=](){
        viewer.myTeapot.bSelected = false;
        viewer.update();
        });
    connect(ui->sliderIK_Y, &QSlider::sliderReleased, [=](){
        viewer.myTeapot.bSelected = false;
        viewer.update();
        });
    connect(ui->sliderIK_Z, &QSlider::sliderReleased, [=](){
        viewer.myTeapot.bSelected = false;
        viewer.update();
        });
    */
    qreal rBase = viewer.myRobot.radRotation[E0_BASE];
    qreal dBase = qRadiansToDegrees(rBase);
    rRobotAlpha = qRound(dBase);
    ui->sliderAlpha->setRange(-180,180);
    ui->sliderAlpha->setValue(static_cast<int>(rRobotAlpha));

    qreal rArm1 = viewer.myRobot.radRotation[E1_ARM1];
    qreal dArm1 = qRadiansToDegrees(rArm1);
    rRobotBeta  = qRound(dArm1);
    ui->sliderBeta->setRange(-180,180);
    ui->sliderBeta->setValue(static_cast<int>(rRobotBeta));

    qreal rArm2 = viewer.myRobot.radRotation[E2_ARM2];
    qreal dArm2 = qRadiansToDegrees(rArm2);
    rRobotGamma= qRound(dArm2);
    ui->sliderGamma->setRange(-180,180);
    ui->sliderGamma->setValue(static_cast<int>(rRobotGamma));

    ui->radioButtonFK->setChecked(true);
    ui->radioButtonIK->setChecked(false);
    on_radioButtonFK_toggled(true);
    on_radioButtonFK_toggled(false);

    //Force teapot upward to World Zaxis
    viewer.myTeapot.frameTeapot->setRotation(Quaternion(Vec(1.0, 0.0, 0.0), M_PI_2)); //on Xaxis @pi/2
    viewer.update();

    updateGroupboxFK();
    FKabr2XYZ();        //Convert Alpha/Beta/Gamma to XYZ, update iRobotX,iRobotY,iRobtZ
    updateGroupboxIK();
    viewer.myTeapot.bSelected = false;
    updateTeapotByXYZ(iRobotX, iRobotY, iRobotZ);

    //Connect timerDemo singal/slot
    connect(&timerDemo, SIGNAL(timeout()), this, SLOT(slotDemoTimeout()));

    //-------------------
    //Validate Kinematics
    //testIK();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateGroupboxFK()
{
qDebug() << Q_FUNC_INFO;

    int iAlpha = static_cast<int>(rRobotAlpha);
    int iBeta  = static_cast<int>(rRobotBeta);
    int iGamma = static_cast<int>(rRobotGamma);

    //Update F-K sliders
    if (ui->sliderAlpha->value() != iAlpha)
        ui->sliderAlpha->setValue(iAlpha);
    if (ui->sliderBeta->value() != iBeta)
        ui->sliderBeta->setValue(iBeta);
    if (ui->sliderGamma->value() != iGamma)
        ui->sliderGamma->setValue(iGamma);

    qreal alpha = rRobotAlpha/1000.0;
    qreal beta  = rRobotBeta/1000.0;
    qreal gamma = rRobotGamma/1000.0;

    QString text;
    text.sprintf("%+06.3f,%+4d", alpha, iAlpha);
    ui->labelBase->setText(QString("Base-α(Π,Φ):%1°").arg(text) );
    text.sprintf("%+06.3f,%+4d", beta, iBeta);
    ui->labelArm1->setText(QString("Arm1-β(Π,Φ):%1°").arg(text) );
    text.sprintf("%+06.3f,%+4d", gamma, iGamma);
    ui->labelArm2->setText(QString("Arm2-γ(Π,Φ):%1°").arg(text) );

    //Update IK title with Alpha,Beta,Gamma
    text.sprintf("I-K:α:%4d° β:%4d° γ:%4d°", iAlpha, iBeta, iGamma);
    ui->groupBoxIK->setTitle(text);
}

void MainWindow::updateGroupboxIK()
{
qDebug() << Q_FUNC_INFO;

    //Update I-K sliders
    if (ui->sliderIK_X->value() != iRobotX)
        ui->sliderIK_X->setValue(iRobotX);
    if (ui->sliderIK_Y->value() != iRobotY)
        ui->sliderIK_Y->setValue(iRobotY);
    if (ui->sliderIK_Z->value() != iRobotZ)
        ui->sliderIK_Z->setValue(iRobotZ);
//qDebug() << "X" << iRobotX << ui->sliderIK_X->value();

    qreal xx = iRobotX/1000.0;
    qreal yy = iRobotY/1000.0;
    qreal zz = iRobotZ/1000.0;

    QString text;
    text.sprintf("X:%+06.3f ", xx);
    ui->labelIK_X->setText(text);
    text.sprintf("Y:%+06.3f ", yy);
    ui->labelIK_Y->setText(text);
    text.sprintf("Z:%+06.3f ", zz);
    ui->labelIK_Z->setText(text);

    //Update FK title with XYZ
    text.sprintf("F-K:(%+06.3f, %+06.3f, %+06.3f)", xx, yy, zz);
    ui->groupBoxFK->setTitle(text);

    //Update spinbox
    ui->spinBoxX->setValue(iRobotX);
    ui->spinBoxY->setValue(iRobotY);
    ui->spinBoxZ->setValue(iRobotZ);
}



/*
 * GroupFK sliders movements
 *      sliderAlpha on robot base
 *
 */
void MainWindow::on_sliderAlpha_sliderMoved(int position)
{
    qreal deg = position;
    if ((ui->radioButtonFK->isChecked()) && (qApp->mouseButtons()==Qt::LeftButton) ) { //jump to step when LeftButton pressed
        int iStep = ui->sliderAlpha->singleStep();
        deg = (position/iStep)*iStep;
        }
    if (rRobotAlpha == deg) return;
    qDebug() << Q_FUNC_INFO << deg;


    //update robot Base rotation
    rRobotAlpha = deg;
    if (ui->checkBoxDemo->isChecked())
        viewer.myRobot.idSelected = E3_CLAW;
    else
        viewer.myRobot.idSelected = E0_BASE;

    //rRobotAlpha, rRobotBeta, rRobotGamma changed
    updateGroupboxFK();
    updateRobotByAlphaBetaGamma(rRobotAlpha, rRobotBeta, rRobotGamma);
    //Find XYZ by Forward Kinematics
    FKabr2XYZ();
    //iRobotX, iRobotY, iRobotZ changed!
    updateGroupboxIK();
    updateTeapotByXYZ(iRobotX, iRobotY, iRobotZ);
}
/*
 * GroupFK sliders movements
 *      sliderBeat on Arm1 @AxisX
 */
void MainWindow::on_sliderBeta_sliderMoved(int position)
{
    qreal deg = position;
    if ((ui->radioButtonFK->isChecked()) && (qApp->mouseButtons()==Qt::LeftButton) ) { //jump to step when LeftButton pressed
        int iStep = ui->sliderAlpha->singleStep();
        deg = (position/iStep)*iStep;
        }
    if (rRobotBeta == deg) return;
    qDebug() << Q_FUNC_INFO << deg;


    //update robot
    rRobotBeta = deg;
    //qreal rArm1 = qDegreesToRadians(rRobotBeta);
    //viewer.myRobot.frame[E1_ARM1]->setRotation( Quaternion(viewer.myRobot.vecRotation[E1_ARM1], rArm1) );
    //update selection
    if (ui->checkBoxDemo->isChecked())
        viewer.myRobot.idSelected = E3_CLAW;
    else
        viewer.myRobot.idSelected = E1_ARM1;

    //rRobotAlpha, rRobotBeta, rRobotGamma changed
    updateGroupboxFK();
    updateRobotByAlphaBetaGamma(rRobotAlpha, rRobotBeta, rRobotGamma);
    //Find XYZ by Forward Kinematics
    FKabr2XYZ();
    //iRobotX, iRobotY, iRobotZ changed!
    updateGroupboxIK();
    updateTeapotByXYZ(iRobotX, iRobotY, iRobotZ);
}
/*
 * GroupFK sliders movements
 *      sliderGamma on Arm2 @AxisX
 */
void MainWindow::on_sliderGamma_sliderMoved(int position)
{
    qreal deg = position;
    if ((ui->radioButtonFK->isChecked()) && (qApp->mouseButtons()==Qt::LeftButton) ) { //jump to step when LeftButton pressed
        int iStep = ui->sliderAlpha->singleStep();
        deg = (position/iStep)*iStep;
        }
    if (rRobotGamma == deg) return;
    qDebug() << Q_FUNC_INFO << deg;


    //update robot
    rRobotGamma = deg;
    //qreal rArm2 = qDegreesToRadians(rRobotGamma);
    //viewer.myRobot.frame[E2_ARM2]->setRotation( Quaternion(viewer.myRobot.vecRotation[E2_ARM2], rArm2) );
    //update selection
    if (ui->checkBoxDemo->isChecked())
        viewer.myRobot.idSelected = E3_CLAW;
    else
        viewer.myRobot.idSelected = E2_ARM2;

    //rRobotAlpha, rRobotBeta, rRobotGamma changed
    updateGroupboxFK();
    updateRobotByAlphaBetaGamma(rRobotAlpha, rRobotBeta, rRobotGamma);
    //Find XYZ by Forward Kinematics
    FKabr2XYZ();
    //iRobotX, iRobotY, iRobotZ changed!
    updateGroupboxIK();
    updateTeapotByXYZ(iRobotX, iRobotY, iRobotZ);
}


void MainWindow::on_radioButtonFK_toggled(bool checked)
{
    if (checked) {
        ui->groupBoxFK->setEnabled(true);
        ui->groupBoxFK->setStyleSheet("QGroupBox{font:10pt\"Courier\";color:black};");
        ui->groupBoxFK->setStyleSheet("QLabel{font:10pt\"Courier\";color:black};");

        ui->groupBoxIK->setEnabled(false);
        ui->groupBoxIK->setStyleSheet("QGroupBox{font:8pt\"Courier\";color:grey};");
        ui->groupBoxIK->setStyleSheet("QLabel{font:8pt\"Courier\";color:grey};");

        updateGroupboxFK();
        viewer.myTeapot.bSelected = false;
        viewer.update();
        }
}

void MainWindow::on_radioButtonIK_toggled(bool checked)
{
    if (checked) {
        ui->groupBoxIK->setEnabled(true);
        ui->groupBoxIK->setStyleSheet("QGroupBox{font:10pt\"Courier\";color:black};");
        ui->groupBoxIK->setStyleSheet("QLabel{font:10pt\"Courier\";color:black};");

        ui->groupBoxFK->setEnabled(false);
        ui->groupBoxFK->setStyleSheet("QGroupBox{font:8pt\"Courier\";color:grey};");
        ui->groupBoxFK->setStyleSheet("QLabel{font:8pt\"Courier\";color:grey};");

        ui->pushButtonIKgo->setEnabled(true);
        updateGroupboxIK();

        viewer.myTeapot.bSelected = true;
        viewer.update();
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
        timerDemo.stop();
        if (ui->checkBoxCrazy->isChecked())
            timerDemo.start(1000.0/viewer.currentFPS()); //Max Herz
        else
            timerDemo.start(1000/8); //8Hz

        }
  else {
        ui->checkBoxCrazy->setCheckState(Qt::Unchecked);
        timerDemo.stop();

        viewer.myRobot.idSelected = E4_NONE;
        viewer.update();
        viewer.displayMessage("");
        }
}

void MainWindow::on_checkBoxTeapot_toggled(bool checked)
{
    if (checked)
        viewer.myTeapot.bShow = true;
    else
        viewer.myTeapot.bShow = false;
    viewer.update(); //imediatly updates
}

/*
 * IK Position Moved X(-999,999) Y(-999,999) Z(0,999)
 *
 *
 */
void MainWindow::on_sliderIK_X_sliderMoved(int position)
{
    if (iRobotX == position) return;
    qDebug() << Q_FUNC_INFO << position;


    iRobotX = position;
    ui->spinBoxX->setValue(iRobotX);

    qreal x = iRobotX/1000.0;
    qreal y = iRobotY/1000.0;
    qreal z = iRobotZ/1000.0;
    qreal z0= viewer.myRobot.fSegLen[E0_BASE];  //0.2
    qreal L1= viewer.myRobot.fSegLen[E1_ARM1];  //0.5
    qreal L2= viewer.myRobot.fSegLen[E2_ARM2];  //0.3
    if (distance(x, y) > (L1+L2))  { //Unreachable! Distance of (x,y) longer than L1+L2 !!!
        ui->labelIKstatus->setText("!!! Unreachable >_< !!!");
        return;
        }
    ui->labelIKstatus->setText("");

    qreal Alpha, Beta, Gamma;
    XYZ2AlphaBetaGamma(x, y, z-z0, L1, L2, &Alpha, &Beta, &Gamma);
    if (qIsNaN(Alpha) || qIsNaN(Beta) || qIsNaN(Gamma)) { bIKerror=true; return;} //CosinLaw return NaN!!! Bye-Bye!
    bIKerror=false;

    rRobotAlpha = 90+Alpha; //YZ plane back to XYZ, Need 90degree rotation on Alpha
    rRobotBeta  = Beta;
    rRobotGamma = Gamma;
    //rRobotAlpha, rRobotBeta, rRobotGamma changed
    updateGroupboxFK();
    updateRobotByAlphaBetaGamma(rRobotAlpha, rRobotBeta, rRobotGamma);

    //iRobotX, iRobotY, iRobotZ changed!
    viewer.myTeapot.bSelected = true;
    updateGroupboxIK();
    updateTeapotByXYZ(iRobotX, iRobotY, iRobotZ);
}

void MainWindow::on_sliderIK_Y_sliderMoved(int position)
{
    if (iRobotY == position) return;
    qDebug() << Q_FUNC_INFO << position;


    iRobotY = position;
    ui->spinBoxY->setValue(iRobotY);

    qreal x = iRobotX/1000.0;
    qreal y = iRobotY/1000.0;
    qreal z = iRobotZ/1000.0;
    qreal z0= viewer.myRobot.fSegLen[E0_BASE];
    qreal L1= viewer.myRobot.fSegLen[E1_ARM1];
    qreal L2= viewer.myRobot.fSegLen[E2_ARM2];
    if (distance(x, y) > (L1+L2))  { //Unreachable! Distance of (x,y) longer than L1+L2 !!!
        ui->labelIKstatus->setText("!!! Unreachable >_< !!!");
        return;
        }
    ui->labelIKstatus->setText("");

    qreal Alpha, Beta, Gamma;
    XYZ2AlphaBetaGamma(x, y, z-z0, L1, L2, &Alpha, &Beta, &Gamma);
    if (qIsNaN(Alpha) || qIsNaN(Beta) || qIsNaN(Gamma)) {bIKerror=true; return;} //CosinLaw return NaN!!! Bye-Bye!
    bIKerror=false;

    rRobotAlpha = 90+Alpha; //YZ plane back to XYZ, Need 90degree rotation on Alpha
    rRobotBeta  = Beta;
    rRobotGamma = Gamma;
    //rRobotAlpha, rRobotBeta, rRobotGamma changed
    updateGroupboxFK();
    updateRobotByAlphaBetaGamma(rRobotAlpha, rRobotBeta, rRobotGamma);

    //iRobotX, iRobotY, iRobotZ changed!
    viewer.myTeapot.bSelected = true;
    updateGroupboxIK();
    updateTeapotByXYZ(iRobotX, iRobotY, iRobotZ);
    ui->spinBoxY->setValue(iRobotY);
}

void MainWindow::on_sliderIK_Z_sliderMoved(int position)
{
    if (iRobotZ == position) return;
    qDebug() << Q_FUNC_INFO << position;


    iRobotZ = position;
    ui->spinBoxZ->setValue(iRobotZ);

    qreal x = iRobotX/1000.0;
    qreal y = iRobotY/1000.0;
    qreal z = iRobotZ/1000.0;
    qreal z0= viewer.myRobot.fSegLen[E0_BASE];
    qreal L1= viewer.myRobot.fSegLen[E1_ARM1];
    qreal L2= viewer.myRobot.fSegLen[E2_ARM2];
    if (distance(x, y) > (L1+L2)) return; //Unreachable! Distance of (x,y) longer than L1+L2 !!!
    if (z<z0) return;   //Unreachable! z lower than base!!!

    qreal Alpha, Beta, Gamma;
    XYZ2AlphaBetaGamma(x, y, z-z0, L1, L2, &Alpha, &Beta, &Gamma);
    if (qIsNaN(Alpha) || qIsNaN(Beta) || qIsNaN(Gamma)) {bIKerror=true; return;} //CosinLaw return NaN!!! Bye-Bye!
    bIKerror=false;

    rRobotAlpha = 90+Alpha; //YZ plane back to XYZ, Need 90degree rotation on Alpha
    rRobotBeta  = Beta;
    rRobotGamma = Gamma;
    //rRobotAlpha, rRobotBeta, rRobotGamma changed
    updateGroupboxFK();
    updateRobotByAlphaBetaGamma(rRobotAlpha, rRobotBeta, rRobotGamma);

    //iRobotX, iRobotY, iRobotZ changed!
    viewer.myTeapot.bSelected = true;
    updateGroupboxIK();
    updateTeapotByXYZ(iRobotX, iRobotY, iRobotZ);

    ui->spinBoxZ->setValue(iRobotZ);
}

void MainWindow::on_pushButtonIKgo_clicked()
{
    int iX = ui->spinBoxX->value();
    int iY = ui->spinBoxY->value();
    int iZ = ui->spinBoxZ->value();
    if (  (iX != ui->sliderIK_X->value())
       || (iY != ui->sliderIK_Y->value())
       || (iZ != ui->sliderIK_Z->value()) ) {
        qreal d = distance(iX, iY);
        qreal L0   = viewer.myRobot.fSegLen[E1_ARM1]*1000;
        qreal L1   = viewer.myRobot.fSegLen[E2_ARM2]*1000;
        if (d > (L0+L1) ) {
            ui->labelIKstatus->setText("!!! Unreachable >_< !!!");
            }
        else {
            ui->labelIKstatus->setText("");
            on_sliderIK_X_sliderMoved(iX);
            on_sliderIK_Y_sliderMoved(iY);
            on_sliderIK_Z_sliderMoved(iZ);
            }
        }
}
