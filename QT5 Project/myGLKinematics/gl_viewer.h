﻿/****************************************************************************

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

#include <qglviewer.h>
#include <manipulatedCameraFrame.h>
#include "gl_robot.h"
#include "gl_teapot.h"
#include "gl_particle.h"

class Viewer : public QGLViewer {
public:
    GL_Robot myRobot;
    GL_Teapot myTeapot;
    qglviewer::Vec vPosScaledOldTeapot[3]; //3 old positions

    const static int iParticleParts=100;
    Particle *particle[iParticleParts];

    void addInfo(QString s);
    void clearInfo();
    bool bShowInfo=false;
    void keyPressEvent(QKeyEvent *e);

protected:
  virtual void draw();
  virtual void init();
  virtual void animate();
  virtual void drawWithNames();
  virtual void postSelection(const QPoint &point);
  virtual QString helpString() const;

private:
  const static int iInfoLines=32;
  qglviewer::Vec orig, dir, selectedPoint; //For mouse click, selection
  void displayText();

  QString sInfo[iInfoLines];
};
