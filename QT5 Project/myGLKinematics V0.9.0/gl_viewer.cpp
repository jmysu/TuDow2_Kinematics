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


  //Create particles
  iParticleParts = 100;
  for (int i = 0; i < 100; ++i) {
    particle[i] = new Particle();
    particle[i]->setPosition(0,0,1);
    }
  glPointSize(3.0);

  startAnimation();
}

void Viewer::draw() {

    myRobot.draw();

if (0) {//draw old teapot positions
    Vec v2= vPosOldTeapot[2];
    myTeapot.frameTeapot->setPosition(v2.x, v2.y, v2.z); //scaled position
    myTeapot.draw();

    Vec v1= vPosOldTeapot[1];
    myTeapot.frameTeapot->setPosition(v1.x, v1.y, v1.z); //scaled position
    myTeapot.draw();
    }
    Vec v0= vPosOldTeapot[0];
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




