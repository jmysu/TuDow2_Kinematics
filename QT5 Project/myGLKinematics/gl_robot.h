#ifndef ROBOT_H
#define ROBOT_H
#include <QtOpenGL>
#include <qglviewer.h>
#include <manipulatedCameraFrame.h>

using namespace qglviewer;
using namespace std;

#define ROBOT_SEGMENTS 4

enum ROBOTS_SEGMENT {E0_BASE, E1_ARM1, E2_ARM2, E3_CLAW, E4_NONE};

class GL_Robot {
public:
  GL_Robot();

  qglviewer::Frame *frame[ROBOT_SEGMENTS];
  unsigned short idSelected             = E4_NONE; //None selected

  //The fSegLen[] defined each segment length
  const qreal fSegLen[ROBOT_SEGMENTS]   = { 0.2,    //Base Height
                                            0.5,    //Arm1
                                            0.3,    //Arm2
                                            0.2 };  //Claw
  //The vecTranslte[] defined each translation vector
  const Vec vecTranslate[ROBOT_SEGMENTS]= { Vec(0.0 ,0.0, 0),
                                            Vec(0.0, 0.0, fSegLen[E0_BASE]),            //Arm1 h= 0+Base
                                            Vec(0.0, 0.0, fSegLen[E1_ARM1]),            //Arm2 h= Base+Arm1
                                            Vec(0.0, 0.0, fSegLen[E2_ARM2]) };          //Claw h= Base+Arm1+Arm2
  //The radRotation[] defined each rotation degrees in radian
  const qreal radRotation[ROBOT_SEGMENTS]={ -M_PI_2,  //Base, Rotate on Z @-90°
                                            M_PI_2/3, //Arm1, on Z @30°
                                            -M_PI_4,  //Arm2, on X @-45°
                                            0 };      //Claw, on Y @0°
  //const qreal radRotation[ROBOT_SEGMENTS]={ 0,  //Base, Rotate on Z @-90°
  //                                          M_PI/3, //Arm1, on Z @180/3=60°
  //                                          -M_PI/6,  //Arm2, on X @180/6=30°
  //                                          0 };      //Claw, on Y @0°

  //const qreal radRotation[ROBOT_SEGMENTS]={     0.0,        //Base
  //                                              0.0,        //Arm1
  //                                              0.0,        //Arm2
  //                                              0.0 };      //Claw
  //The vecRotation[] defined each rotation vector
  const Vec  vecRotation[ROBOT_SEGMENTS]={Vec(0.0, 0.0, 1.0), //Z
                                          Vec(1.0, 0.0, 0.0), //Zx
                                          Vec(1.0, 0.0, 0.0), //X
                                          Vec(0.0, 1.0, 0.0)};//Y

  void setColor(unsigned short nb);
  void draw(bool names = false);

private:

  qglviewer::AxisPlaneConstraint *constraints[3];
  unsigned short activeConstraint;

  void drawCone(float zMin, float zMax, float r1, float r2, int nbSub);
  void drawBase();
  void drawArm1();
  void drawArm2();
  void drawJoint();
  void drawHead();
  void drawClaw();
};

#endif //ROBOT_H
