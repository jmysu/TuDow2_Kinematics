/****************************************************************************

 Copyright (C) 2002-2014 Gilles Debunne. All rights reserved.

 This file is part of the QGLViewer library version 2.7.1.

 http://www.libqglviewer.com - contact@libqglviewer.com

 This file may be used under the terms of the GNU General Public License 
 versions 2.0 or 3.0 as published by the Free Software Foundation and
 appearing in the LICENSE file included in the packaging of this file.
 In addition, as a special exception, Gilles Debunne gives you certain 
 additional rights, described in the file GPL_EXCEPTION in this package.

 libQGLViewer uses dual licensing. Commercial/proprietary software must
 purchase a libQGLViewer Commercial License.

 This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************************/

#include "gl_robot.h"
#include <math.h>
#include <QDebug>

using namespace qglviewer;
using namespace std;


//////////////////////////////////  Robots  ///////////////////////////////////////////

GL_Robot::GL_Robot() {
  for (unsigned short i = 0; i < 4; ++i) {// Creates a hierarchy of frames.
    frame[i] = new ManipulatedFrame();
    if (i > 0)
      frame[i]->setReferenceFrame(frame[i - 1]);
    }

  // Initialize frames
  frame[E0_BASE]->setRotation(Quaternion(vecRotation[E0_BASE], radRotation[E0_BASE]));

  frame[E1_ARM1]->setRotation(Quaternion(vecRotation[E1_ARM1], radRotation[E1_ARM1]));
  frame[E1_ARM1]->setTranslation(vecTranslate[E1_ARM1]);

  frame[E2_ARM2]->setRotation(Quaternion(vecRotation[E2_ARM2], radRotation[E2_ARM2]));
  frame[E2_ARM2]->setTranslation(vecTranslate[E2_ARM2]);

  frame[E3_CLAW]->setRotation(Quaternion(vecRotation[E3_CLAW], radRotation[E3_CLAW]));
  frame[E3_CLAW]->setTranslation(vecTranslate[E3_CLAW]);

  // Set frame constraints, World first, E0_Base constraint to Z-axis
  WorldConstraint *ZAxis = new WorldConstraint();
  ZAxis->setTranslationConstraint(AxisPlaneConstraint::PLANE, Vec(0.0, 0.0, 1.0));
  ZAxis->setRotationConstraint(AxisPlaneConstraint::AXIS, Vec(0.0, 0.0, 1.0));
  frame[E0_BASE]->setConstraint(ZAxis); //Base constraint to Z-axis

  //Arm1, Arm2 contraint to itself on X-axis
  LocalConstraint *XAxis = new LocalConstraint();
  XAxis->setTranslationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(0.0, 0.0, 0.0));
  XAxis->setRotationConstraint(AxisPlaneConstraint::AXIS, Vec(1.0, 0.0, 0.0));
  frame[E1_ARM1]->setConstraint(XAxis); //X-axis
  frame[E2_ARM2]->setConstraint(XAxis); //X-axis

  //Claw contraint to itself on Y-axis
  LocalConstraint *YAxis = new LocalConstraint();
  YAxis->setTranslationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(0.0, 0.0, 0.0));
  YAxis->setRotationConstraint(AxisPlaneConstraint::AXIS, Vec(0.0, 1.0, 0.0));
  frame[E3_CLAW]->setConstraint(YAxis);
}

void GL_Robot::draw(bool names) {

  // Robots' local frame
  glPushMatrix();
  glMultMatrixd(frame[E0_BASE]->matrix());


  const float scale = 5.0f;
  glScalef(scale, scale, scale);
  //--------------------------------0
  //Base
  if (names)
    glPushName(0);
  setColor(0);
  drawBase();
  if (names)
    glPopName();
  //--------------------------------1
  //Arm1
  if (names)
    glPushName(1);
  glMultMatrixd(frame[E1_ARM1]->matrix());
  setColor(1);
  drawJoint();
  drawArm1();
  if (names)
    glPopName();
  //--------------------------------2
  //Arm2
  if (names)
    glPushName(2);
  glMultMatrixd(frame[E2_ARM2]->matrix());
  setColor(2);
  drawJoint();
  drawArm2();
  if (names)
    glPopName();
  //--------------------------------3
  //Head
/*  if (names)
    glPushName(3);
  glMultMatrixd(frame(3)->matrix());
  setColor(3);
  drawHead();
  if (names)
    glPopName();
*/
  //--------------------------------3
  //Claw
  if (names)
    glPushName(3);
  glMultMatrixd(frame[E3_CLAW]->matrix());
  setColor(3);
  drawClaw();
  if (names)
    glPopName();
/*
  // Add light
  const GLfloat pos[4] = {0.0, 0.0, 0.0, 1.0};
  glLightfv(GL_LIGHT1, GL_POSITION, pos);
  const GLfloat spot_dir[3] = {0.0, 0.0, 1.0};
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_dir);
*/
  glPopMatrix();
}

/*
    // Draws truncated cones aligned with the Z axis.
    drawCone(zMin, zMax, r1, r2, int nbSub)

 */
void GL_Robot::drawBase() {
  float fLen = fSegLen[E0_BASE];

  drawCone(      0.0, fLen* 6/10, 0.1f,  0.1f,  32); //base cylinder
  drawCone(fLen*6/10, fLen* 8/10, 0.1f,  0.05f, 32);
  drawCone(fLen*8/10, fLen*10/10, 0.05f, 0.01f, 32);
}

void GL_Robot::drawArm1() {
  float fLen = fSegLen[E1_ARM1];
  glTranslatef(0.02f, 0.0, 0.0);
  //drawCone(0.0, 0.5f, 0.01f, 0.01f, 10);
  drawCone(0.0, fLen, 0.036f, 0.03f, 8); //arm-left
  glTranslatef(-0.04f, 0.0, 0.0);
  //drawCone(0.0, 0.5f, 0.01f, 0.01f, 10);
  drawCone(0.0, fLen, 0.036f, 0.03f, 8); //arm-right
  glTranslatef(0.02f, 0.0, 0.0);
}
void GL_Robot::drawArm2() {
  float fLen = fSegLen[E2_ARM2];
  glTranslatef(0.02f, 0.0, 0.0);
  //drawCone(0.0, 0.5f, 0.01f, 0.01f, 10);
  drawCone(0.0, fLen, 0.035f, 0.02f, 4); //arm-left
  glTranslatef(-0.04f, 0.0, 0.0);
  //drawCone(0.0, 0.5f, 0.01f, 0.01f, 10);
  drawCone(0.0, fLen, 0.035f, 0.02f, 4); //arm-right
  glTranslatef(0.02f, 0.0, 0.0);
}
void GL_Robot::drawHead() {
  drawCone(-0.025f,-0.02f, 0.01f, 0.04f, 30);
  drawCone(-0.02f,  0.06f, 0.04f, 0.04f, 30);
  drawCone( 0.06f,  0.15f, 0.04f, 0.17f, 30);
  drawCone( 0.15f,  0.17f, 0.17f, 0.17f, 30);
}
void GL_Robot::drawClaw() {
  float fLen = fSegLen[E3_CLAW];

  drawCone(-0.025f,-0.02f, 0.01f, 0.05f, 10);
  drawCone(-0.02f,  0.03f, 0.05f, 0.05f, 10);
  drawCone( 0.03f,  0.03f, 0.01f, 0.05f, 10);

  glRotatef(-90, 1,0,0); //rotate 180deg on X
  glTranslatef(0.02f, 0.0, 0.0);
  drawCone(0.0, fLen, 0.02f, 0.01f, 3); //arm-left
  glTranslatef(-0.04f, 0.0, 0.0);
  drawCone(0.0, fLen, 0.02f, 0.01f, 3); //arm-right
  glTranslatef(0.02f, 0.0, 0.0);


}
void GL_Robot::drawJoint() {
  glPushMatrix();
  glRotatef(90, 0.0, 1.0, 0.0);                  //Yaxis @90degree
  float fWidth = 0.05f;
  drawCone(-0.055f, -0.05f,  0.0f,  fWidth, 32);  //left side
  drawCone(-0.05f,   0.05f,  fWidth,fWidth, 32);  //cylinder
  drawCone( 0.05f,   0.055f, 0.0f,  fWidth, 32);  //right side
  glPopMatrix();
}

void GL_Robot::setColor(unsigned short nb) {
  if (nb == idSelected)
    glColor3f(212.0f/255.0f, 175.0f/255.0f, 55.0f/255.0f); //Gold
  else
    //glColor3f(59.0f/255.0f, 122.0f/255.0f, 87.0f/255.0f); //SeaGreen(Amazon)
    glColor3f(29.0f/255.0f,  92.0f/255.0f, 57.0f/255.0f); //SeaGreen(Amazon)
}

// Draws a truncated cone aligned with the Z axis.
void GL_Robot::drawCone(float zMin, float zMax, float r1, float r2, int nbSub) {
  static GLUquadric *quadric = gluNewQuadric();

  glTranslatef(0.0, 0.0, zMin);
  gluCylinder(quadric, r1, r2, zMax - zMin, nbSub, 1);
  glTranslatef(0.0, 0.0, -zMin);
}
