#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "kinematics.h"
#include "qglviewer.h"

using namespace qglviewer;

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

/*
 * FKabr2XYZ()
 *
 *  Forward Kinetics, Update Robot XYZ from Alpha,Beta,Gamma
 *
 *  Input: rRobotAlpha, rRobotBeta, rRobotBeta
 *
 *  Output: iRobotX, iRobotY, iRobotZ (-999~999)
 *
 */
void MainWindow::FKabr2XYZ()
{
qDebug() << Q_FUNC_INFO;
    //qreal dBase= ui->sliderAlpha->value();
    qreal rBase= qDegreesToRadians(rRobotAlpha);
    //qreal dT0  = ui->sliderBeta->value();
    qreal rT0  = qDegreesToRadians(rRobotBeta);
    //qreal dT1  = ui->sliderGamma->value();
    qreal rT1  = qDegreesToRadians(rRobotGamma);

    qreal l0   = viewer.myRobot.fSegLen[E1_ARM1];
    qreal l1   = viewer.myRobot.fSegLen[E2_ARM2];

    //Calc position on ZY-plane
    qreal z    = KposXfromAnglesRad(rT0, rT1, l0, l1);
    qreal y    = KposYfromAnglesRad(rT0, rT1, l0, l1);

    //Calc x1/y1 after Base rotation
    qreal xx = y*qSin(rBase);
    qreal yy = -y*qCos(rBase);
    qreal zz = z + viewer.myRobot.fSegLen[E0_BASE];

    //QString text;
    //text.sprintf("F-K:(%+06.3f,%+06.3f,%+06.3f)", xx, yy, zz);
    //ui->groupBoxFK->setTitle(text);

    //Update teapot position
    //float fscale = viewer.myTeapot.fScale;
    //viewer.myTeapot.frameTeapot->setPosition(xx/fscale, yy/fscale, zz/fscale);

    //Save X,Y,Z
    iRobotX = qRound(xx*1000);
    iRobotY = qRound(yy*1000);
    iRobotZ = qRound(zz*1000);
//qDebug() << "XYZ:" << iRobotX << iRobotY << iRobotZ;
}

/*
 * Inverse Kinematics, Calc Alpha,Beta from (x,y)
 *
 *  Input: (x,y), len1, len2
 *
 *  Output: *A1, *A2
 *
 *
void MainWindow::IKxy2A1A2(qreal x, qreal y, int *A1, int *A2)
{
    //Calc Inverse-Kinematics on ZY-plane
    qreal l1   = viewer.myRobot.fSegLen[E1_ARM1];
    qreal l2   = viewer.myRobot.fSegLen[E2_ARM2];

    qreal rA1  = IKradA1(x, y, l1, l2);
    qreal rA2  = IKradA2(x, y, l1, l2);
    qreal dA1  = qRadiansToDegrees(rA1);
    qreal dA2  = qRadiansToDegrees(rA2);
    int iA1    = qRound(dA1);
    //int iA2    = qRound(dA2 -180);
    int iA2    = qRound(dA2);

    QString t;
    t.sprintf("IK: (x=%-6.3f, y=%-6.3f)=> A1=%4d° A2=%4d°", x, y, iA1, iA2);
    qDebug() << t;

    *A1 = iA1;
    *A2 = iA2;

qDebug() << "A1A2:" << iA1 << iA2;
}

void MainWindow::slotUpdateRobot()
{
    qDebug() << Q_FUNC_INFO;

    qreal rBase = qDegreesToRadians(rRobotAlpha);
    viewer.myRobot.frame[E0_BASE]->setRotation( Quaternion(viewer.myRobot.vecRotation[E0_BASE], rBase) );
    qreal rArm1 = qDegreesToRadians(rRobotBeta);
    viewer.myRobot.frame[E1_ARM1]->setRotation( Quaternion(viewer.myRobot.vecRotation[E1_ARM1], rArm1) );
    qreal rArm2 = qDegreesToRadians(rRobotGamma);
    viewer.myRobot.frame[E2_ARM2]->setRotation( Quaternion(viewer.myRobot.vecRotation[E2_ARM2], rArm2) );
    //viewer.update();

    FKabr2XYZ();    //Calc XYZ by AlphaBetaGamma

    qreal xx = iRobotX/1000.0;
    qreal yy = iRobotY/1000.0;
    qreal zz = iRobotZ/1000.0;
    float fscale = viewer.myTeapot.fScale;
    viewer.myTeapot.frameTeapot->setPosition(xx/fscale, yy/fscale, zz/fscale);
    viewer.update();

    //update UI
    updateGroupboxFK();
    updateGroupboxIK();
}
*/
/*
 * Alpha,Beta,Gamma in radians
 *
 */
void MainWindow::updateRobotByAlphaBetaGamma(qreal Alpha, qreal Beta, qreal Gamma)
{
    qDebug() << Q_FUNC_INFO;

    qreal rBase = qDegreesToRadians(rRobotAlpha);
    viewer.myRobot.frame[E0_BASE]->setRotation( Quaternion(viewer.myRobot.vecRotation[E0_BASE], rBase) );
    qreal rArm1 = qDegreesToRadians(rRobotBeta);
    viewer.myRobot.frame[E1_ARM1]->setRotation( Quaternion(viewer.myRobot.vecRotation[E1_ARM1], rArm1) );
    qreal rArm2 = qDegreesToRadians(rRobotGamma);
    viewer.myRobot.frame[E2_ARM2]->setRotation( Quaternion(viewer.myRobot.vecRotation[E2_ARM2], rArm2) );
    viewer.update();
}

/*
 * X,Y,Z in -999~999
 *
 */
void MainWindow::updateTeapotByXYZ(qreal X, qreal Y, qreal Z)
{
    qDebug() << Q_FUNC_INFO << X << Y << Z;

    float f = 1000.0f * viewer.myTeapot.fScale;
    if (viewer.isValid()) {
        //viewer.myTeapot.frameTeapot->setPosition(X, Y, Z); //scaled position
        //Shift myTeapot position at frameTeapot. (for displaying old positions)
        qreal xx=X/f; qreal yy=Y/f; qreal zz=Z/f;
        viewer.vPosScaledOldTeapot[2] =  viewer.vPosScaledOldTeapot[1];
        viewer.vPosScaledOldTeapot[1] =  viewer.vPosScaledOldTeapot[0];
        viewer.vPosScaledOldTeapot[0] =  Vec(xx,yy,zz); //Scaled position for Teapot
        //Reset particals' position
        for (int i = 0; i < viewer.iParticleParts; i++)
            viewer.particle[i]->setPosition(xx,yy,zz); //Scaled position w/ Teapot
        //Add Info
        QString s;
        //s.sprintf("Target:(%+6.3f,%+6.3f,%+6.3f)",xx,yy,zz);
        s.sprintf("Target:(%+6.3f,%+6.3f,%+6.3f)",xx*viewer.myTeapot.fScale,yy*viewer.myTeapot.fScale,zz*viewer.myTeapot.fScale); //Normalized position
        viewer.addInfo(s);

        viewer.update();
        }
}

void MainWindow::demoRobotAlphaBetaGamma()
{
static int iBdiff    = 15;
static int iArm1diff = 10;
static int iArm2diff = 5;

    //Alpha
    int iBase = ui->sliderAlpha->value();
    if (ui->checkBoxCrazy->isChecked())
        iBase = qrand()%360 -180;
    else
        iBase += iBdiff;
    if ( (iBase > ui->sliderAlpha->maximum())
      || (iBase < ui->sliderAlpha->minimum()) )
        iBdiff = -iBdiff;
    on_sliderAlpha_sliderMoved(iBase);
    //Beta
    int iArm1 = ui->sliderBeta->value();
    if (ui->checkBoxCrazy->isChecked())
        iArm1 = qrand()%180-90;
    else
        iArm1 += iArm1diff;
    if ((iArm1 > 90) || (iArm1 < -90) )
        iArm1diff = -iArm1diff;
    on_sliderBeta_sliderMoved(iArm1);
    //Gamma
    int iArm2 = ui->sliderGamma->value();
    if (ui->checkBoxCrazy->isChecked())
        iArm2 = qrand()%180-90;
    else
        iArm2 += iArm2diff;
    if ((iArm2 > 90) || (iArm2 < -90) )
        iArm2diff = -iArm2diff;
    on_sliderGamma_sliderMoved(iArm2);
}

/*
 * Running XYZ demo, some values are hard coded!
 *
 */
void MainWindow::demoRobotXYZ()
{
 static int iX=100; //init value from iRobot
 static int iY=100; //init value from iRobot
 static int iZ=555; //init value from iRobot
 static int iXdir=1;
 static int iYdir=1;
 static int iZdir=-1;
 qreal d;

    iX += iXdir*100;
    if ( (iX>800) || (iX<-800) ) {
        iXdir = -iXdir;
        iX += iXdir*100;

        iY+= iYdir*100;
        if (iY==0) iY+=iYdir*100; //Don't cross over myself
        if ( (iY>800) || (iY<-800)){
            iYdir = -iYdir;
            iY+= iYdir*100;

            iZ += iZdir*100;
            if ( (iZ<0) || (iZ>800) ) {
                iZdir = -iZdir;
                }
            }
        }
    qreal r1    = distance(iX, iY);
    qreal r2    = iZ-200;
    qreal r3    = distance(r1, r2);
    if (r3>800) {iX+=iXdir*555; return;} //Unreachable! L1=500, L2=300, try next iY by adding a big X

    //on_sliderIK_X_sliderMoved(iX);
    //on_sliderIK_Y_sliderMoved(iY);
    //on_sliderIK_Z_sliderMoved(iZ);o
    iRobotX = iX;
    iRobotY = iY;
    iRobotZ = iZ;
    on_XYZ_Changed();
}

/*
 * goRobotXYZ()
 *
 *  sweep from current position(v0) to new XYZ(v1)
 *
 */
void MainWindow::runRobotXYZ()
{
static int iRun=0;

    if (iRun++>10) {
        iRun=0; //Reset iRun
        //swap v0,v1
        Vec vTemp = v0;
        v0= v1;
        v1= vTemp;
        }
    int ixdir = (v1.x>v0.x)?1:-1;
    int iydir = (v1.y>v0.y)?1:-1;
    int izdir = (v1.z>v0.z)?1:-1;

    qreal  step;
    step = qAbs(v1.x-v0.x)/10;
    int iX = v0.x + ixdir*step*iRun;
    step = qAbs(v1.y-v0.y)/10;
    int iY = v0.y + iydir*step*iRun;
    step = qAbs(v1.z-v0.z)/10;
    int iZ = v0.z + izdir*step*iRun;

    iRobotX = iX;
    iRobotY = iY;
    iRobotZ = iZ;
    on_XYZ_Changed();
}


void MainWindow::slotDemoTimeout()
{
    if (ui->radioButtonFK->isChecked()) {
        ui->checkBoxCrazy->setEnabled(true);
        demoRobotAlphaBetaGamma();
        }
    else {
        ui->checkBoxCrazy->setEnabled(false);
        demoRobotXYZ();
        }
    //Update framerate info at left lower conner!
    if (ui->checkBoxCrazy->isChecked())
        viewer.displayMessage(QString("DEMO Most-Crazy @%1Hz").arg(1000/timerDemo.interval()) );
    else
        viewer.displayMessage(QString("DEMO @%1Hz").arg(1000/timerDemo.interval()) );
}



