#ifndef GL_TEAPOT_H
#define GL_TEAPOT_H
#include <QtOpenGL>
#include <qglviewer.h>
#include "gl_teapotdata.h"

class GL_Teapot
{
public:
    GL_Teapot();

    void draw();
    void animate();
    qglviewer::Frame *frameTeapot;

    const float fScale = 0.2f;
    bool bShow = true;
    bool bSelected = false;

private:
    float shininess         = 5.0f;                         //Not very shinny as teapot should be
    float diffuseColor[3]   = {0.60f, 0.30f, 0.25f};        //Teapot color
    float specularColor[4]  = {0.65f, 0.45f, 0.30f, 0.5f};  //Specular color
};

#endif // GL_TEAPOT_H
