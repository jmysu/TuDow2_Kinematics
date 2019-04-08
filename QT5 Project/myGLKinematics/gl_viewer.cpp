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

#include "gl_teapot.h"
#include "gl_viewer.h"
#include <math.h>
#include <QDebug>

using namespace qglviewer;
using namespace std;

//////////////////////////////////  V i e w e r  ///////////////////////////////////////////

void Viewer::init() {


  restoreStateFromFile();

  // Make camera the default manipulated frame.
  setManipulatedFrame(camera()->frame());

  setMouseBinding(Qt::AltModifier, Qt::LeftButton, QGLViewer::CAMERA,
                  QGLViewer::ROTATE);
  setMouseBinding(Qt::AltModifier, Qt::RightButton, QGLViewer::CAMERA,
                  QGLViewer::TRANSLATE);
  setMouseBinding(Qt::AltModifier, Qt::MidButton, QGLViewer::CAMERA,
                  QGLViewer::ZOOM);
  setWheelBinding(Qt::AltModifier, QGLViewer::CAMERA, QGLViewer::ZOOM);

  setMouseBinding(Qt::NoModifier, Qt::LeftButton, QGLViewer::FRAME,
                  QGLViewer::ROTATE);
  setMouseBinding(Qt::NoModifier, Qt::RightButton, QGLViewer::FRAME,
                  QGLViewer::TRANSLATE);
  setMouseBinding(Qt::NoModifier, Qt::MidButton, QGLViewer::FRAME,
                  QGLViewer::ZOOM);
  setWheelBinding(Qt::NoModifier, QGLViewer::FRAME, QGLViewer::ZOOM);


  //Create particles first (Windows bug workaround at Mainwindow setupUI)
  for (int i = 0; i < iParticleParts; ++i) {
    particle[i] = new Particle();
    //particle[i]->setPosition(0,0,1);
    }

  startAnimation();
  //---------------

}

void Viewer::addInfo(QString s)
{
    for (int i=0;i<iInfoLines-1;i++) sInfo[i] = sInfo[i+1];
    sInfo[iInfoLines-1] = s;
}

void Viewer::clearInfo()
{
    for (int i=0;i<iInfoLines;i++) sInfo[i]="";
}

void Viewer::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
        case Qt::Key_I : bShowInfo=!bShowInfo; break;

        // Default calls the original method to handle standard keys
        default: QGLViewer::keyPressEvent(e);
        }
}

void Viewer::draw() {

    //------------------------------------
    myRobot.draw();

if (0) {//draw old teapot positions
    Vec v2= vPosScaledOldTeapot[2];
    myTeapot.frameTeapot->setPosition(v2.x, v2.y, v2.z); //scaled position
    myTeapot.draw();

    Vec v1= vPosScaledOldTeapot[1];
    myTeapot.frameTeapot->setPosition(v1.x, v1.y, v1.z); //scaled position
    myTeapot.draw();
    }
    Vec v0= vPosScaledOldTeapot[0];
    myTeapot.frameTeapot->setPosition(v0.x, v0.y, v0.z); //scaled position
    myTeapot.draw();


if (1) {
    //Particle-----------------

    glDisable(GL_LIGHTING);
    glBegin(GL_POINTS);
    for (int i = 0; i < iParticleParts; i++) {
        particle[i]->draw();
        }
    glEnd();
    glEnable(GL_LIGHTING);

    }
// Draw (approximated) intersection point on selected object
if (selectedName() >= 0) {
    glColor3f(0.9f, 0.2f, 0.1f);
    glBegin(GL_POINTS);
    glVertex3fv(selectedPoint);
    glEnd();
    }

    //Show Info
if (bShowInfo) {
    //Save OpenGL state
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glColor3f(0.8f, 0.8f, 0.3f);
    for (int i=0;i<iInfoLines;i++) {
        if (!sInfo[i].isEmpty())
            drawText(10, 48+16*i, sInfo[i]);
        }
    // Restore GL state
    glPopAttrib();
    }
    /*
    //draw indicator tiny arrow
    glColor3f(0.75f, 0.75f, 0.25f);
    Vec vFrom = myTeapot.frameTeapot->position();
    Vec vTo   = vFrom;
    vFrom.z   = 0.25;
    vTo.z     = 0.0;
    drawArrow(vFrom, vTo, 0.05);
    */
    //---------------------------
    //Draw crosshair
    glDisable(GL_LIGHTING);
    Vec v = myTeapot.frameTeapot->position();

    glPushAttrib(GL_ENABLE_BIT);
      glColor3f(0.75f, 0.75f, 0.25f);
      //crosshair
      glLineWidth(4);
      glBegin(GL_LINES);
      glVertex2d(v.x-0.25, v.y-0.25);
      glVertex2d(v.x+0.25, v.y+0.25);
      glVertex2d(v.x-0.25, v.y+0.25);
      glVertex2d(v.x+0.25, v.y-0.25);
      glEnd();
      //dotted crosslines
      glLineStipple(1, 0xA0A0);
      glEnable(GL_LINE_STIPPLE);
      glLineWidth(2);
      glBegin(GL_LINES);
      glVertex2d( -5, v.y);
      glVertex2d(  5, v.y);
      glVertex2d(v.x, -5);
      glVertex2d(v.x,  5);
      glEnd();
    glPopAttrib();
    glEnable(GL_LIGHTING);
}

void Viewer::drawWithNames() {
  // Render scene with objects ids
  myRobot.draw(true);
  myTeapot.draw(true);
}

void Viewer::animate() {
  //Animate particles
  for (int i = 0; i < iParticleParts; i++)
    particle[i]->animate();
  //Animate teapot
  qglviewer::Vec axis;
  axis = qglviewer::Vec(0.0, 1.0, 0.0);
  myTeapot.frameTeapot->rotate(Quaternion(axis, -M_PI/10)); //- pi/10 per step
}


void Viewer::postSelection(const QPoint &point) {
  // Find the selectedPoint coordinates, using camera()->pointUnderPixel().
  bool found;
  selectedPoint = camera()->pointUnderPixel(point, found);
  if (found) {
    qDebug() << endl << "Selected id:" << selectedName();
    qDebug() << "Mouse:" << point;
    qDebug() << "Screen:" << selectedPoint.x << selectedPoint.y << selectedPoint.z;
    qDebug() << "XYZ:" << selectedPoint.x*myTeapot.fScale << selectedPoint.y*myTeapot.fScale << selectedPoint.z*myTeapot.fScale;
    QString s;
    s.sprintf("Selected ID:%d", selectedName());
    addInfo(s);
    s.sprintf("Screen:(%+6.3f,%+6.3f,%+6.3f)",selectedPoint.x, selectedPoint.y, selectedPoint.z);
    addInfo(s);
    s.sprintf("XYZ   :(%+6.3f,%+6.3f,%+6.3f)",selectedPoint.x*myTeapot.fScale, selectedPoint.y*myTeapot.fScale, selectedPoint.z*myTeapot.fScale);
    addInfo(s);
    }
  Vec v= camera()->position();
  qDebug() << endl << "Camera position:" << v.x << v.y << v.z;

  v= myTeapot.frameTeapot->position();
  qDebug() <<   "Teapot position @World: " << v.x << v.y << v.z;
  qDebug() <<           "Teapot position @Normal:" << v.x/5 << v.y/5 << v.z/5;
  }


QString Viewer::helpString() const {
  QString sTitle= APP_NAME+QString(" V")+APP_VERSION;
  QString text("<h2>" +sTitle+ "</h2>");
  text += "Use Keyboard <i>I</i> to Enable/Disable showing the<br>";
  text += "target/teapot positions! <br><br>";
  text += "Press <b>Return</b> to start/stop the animation.";
  return text;
}
