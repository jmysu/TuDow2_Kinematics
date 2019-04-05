#include "gl_teapot.h"

using namespace qglviewer;

GL_Teapot::GL_Teapot()
{
    frameTeapot = new qglviewer::Frame();
    frameTeapot->setPosition(Vec(-2,-2,0)); //init teapot position
    draw();
}

/**********************************************************************************
 *
 * Drawing teapot
 */
void GL_Teapot::draw()
{
    if (!bShow) return ; //No show!

    glPushMatrix();
    glMultMatrixd(frameTeapot->matrix());

    glScalef(fScale, fScale, fScale); //Scale size
    glEnable(GL_NORMALIZE);//To preventing lighting change after scaled!
    //------------------------------------------------------------------
    if (bSelected) {
        glColor3f(212.0f/255.0f, 175.0f/255.0f, 55.0f/255.0f); //Gold
        }
    else {
        //Set specular & shininess using glMaterial (gold)
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);
        //Set ambient & diffuse color using glColorMaterial (gold)
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glColor3fv(diffuseColor);
        }
    /*
     * Start to render polygons
     */
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glNormalPointer(GL_FLOAT, 0, teapotNormals);
    glVertexPointer(3, GL_FLOAT, 0, teapotVertices);

    glDrawElements(GL_TRIANGLE_STRIP, 12, GL_UNSIGNED_SHORT, &teapotIndices[0]);
    glDrawElements(GL_TRIANGLE_STRIP, 78, GL_UNSIGNED_SHORT, &teapotIndices[12]);
    glDrawElements(GL_TRIANGLE_STRIP, 35, GL_UNSIGNED_SHORT, &teapotIndices[90]);
    glDrawElements(GL_TRIANGLE_STRIP, 70, GL_UNSIGNED_SHORT, &teapotIndices[125]);
    glDrawElements(GL_TRIANGLE_STRIP, 65, GL_UNSIGNED_SHORT, &teapotIndices[195]);
    glDrawElements(GL_TRIANGLE_STRIP, 37, GL_UNSIGNED_SHORT, &teapotIndices[260]);
    glDrawElements(GL_TRIANGLE_STRIP, 35, GL_UNSIGNED_SHORT, &teapotIndices[297]);
    glDrawElements(GL_TRIANGLE_STRIP, 32, GL_UNSIGNED_SHORT, &teapotIndices[332]);
    glDrawElements(GL_TRIANGLE_STRIP, 56, GL_UNSIGNED_SHORT, &teapotIndices[364]);
    glDrawElements(GL_TRIANGLE_STRIP, 45, GL_UNSIGNED_SHORT, &teapotIndices[420]);
    glDrawElements(GL_TRIANGLE_STRIP, 41, GL_UNSIGNED_SHORT, &teapotIndices[465]);
    glDrawElements(GL_TRIANGLE_STRIP, 37, GL_UNSIGNED_SHORT, &teapotIndices[506]);
    glDrawElements(GL_TRIANGLE_STRIP, 33, GL_UNSIGNED_SHORT, &teapotIndices[543]);
    glDrawElements(GL_TRIANGLE_STRIP, 29, GL_UNSIGNED_SHORT, &teapotIndices[576]);
    glDrawElements(GL_TRIANGLE_STRIP, 25, GL_UNSIGNED_SHORT, &teapotIndices[605]);
    glDrawElements(GL_TRIANGLE_STRIP, 21, GL_UNSIGNED_SHORT, &teapotIndices[630]);
    glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, &teapotIndices[651]);
    glDrawElements(GL_TRIANGLE_STRIP, 13, GL_UNSIGNED_SHORT, &teapotIndices[668]);
    glDrawElements(GL_TRIANGLE_STRIP, 9, GL_UNSIGNED_SHORT, &teapotIndices[681]);
    glDrawElements(GL_TRIANGLE_STRIP, 27, GL_UNSIGNED_SHORT, &teapotIndices[690]);
    glDrawElements(GL_TRIANGLE_STRIP, 16, GL_UNSIGNED_SHORT, &teapotIndices[717]);
    glDrawElements(GL_TRIANGLE_STRIP, 22, GL_UNSIGNED_SHORT, &teapotIndices[733]);
    glDrawElements(GL_TRIANGLE_STRIP, 50, GL_UNSIGNED_SHORT, &teapotIndices[755]);
    glDrawElements(GL_TRIANGLE_STRIP, 42, GL_UNSIGNED_SHORT, &teapotIndices[805]);
    glDrawElements(GL_TRIANGLE_STRIP, 43, GL_UNSIGNED_SHORT, &teapotIndices[847]);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, &teapotIndices[890]);
    glDrawElements(GL_TRIANGLE_STRIP, 143, GL_UNSIGNED_SHORT, &teapotIndices[894]);
    glDrawElements(GL_TRIANGLE_STRIP, 234, GL_UNSIGNED_SHORT, &teapotIndices[1037]);
    glDrawElements(GL_TRIANGLE_STRIP, 224, GL_UNSIGNED_SHORT, &teapotIndices[1271]);
    glDrawElements(GL_TRIANGLE_STRIP, 71, GL_UNSIGNED_SHORT, &teapotIndices[1495]);
    glDrawElements(GL_TRIANGLE_STRIP, 69, GL_UNSIGNED_SHORT, &teapotIndices[1566]);
    glDrawElements(GL_TRIANGLE_STRIP, 67, GL_UNSIGNED_SHORT, &teapotIndices[1635]);
    glDrawElements(GL_TRIANGLE_STRIP, 65, GL_UNSIGNED_SHORT, &teapotIndices[1702]);
    glDrawElements(GL_TRIANGLE_STRIP, 63, GL_UNSIGNED_SHORT, &teapotIndices[1767]);
    glDrawElements(GL_TRIANGLE_STRIP, 61, GL_UNSIGNED_SHORT, &teapotIndices[1830]);
    glDrawElements(GL_TRIANGLE_STRIP, 59, GL_UNSIGNED_SHORT, &teapotIndices[1891]);
    glDrawElements(GL_TRIANGLE_STRIP, 57, GL_UNSIGNED_SHORT, &teapotIndices[1950]);
    glDrawElements(GL_TRIANGLE_STRIP, 55, GL_UNSIGNED_SHORT, &teapotIndices[2007]);
    glDrawElements(GL_TRIANGLE_STRIP, 53, GL_UNSIGNED_SHORT, &teapotIndices[2062]);
    glDrawElements(GL_TRIANGLE_STRIP, 51, GL_UNSIGNED_SHORT, &teapotIndices[2115]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[2166]);
    glDrawElements(GL_TRIANGLE_STRIP, 50, GL_UNSIGNED_SHORT, &teapotIndices[2169]);
    glDrawElements(GL_TRIANGLE_STRIP, 48, GL_UNSIGNED_SHORT, &teapotIndices[2219]);
    glDrawElements(GL_TRIANGLE_STRIP, 46, GL_UNSIGNED_SHORT, &teapotIndices[2267]);
    glDrawElements(GL_TRIANGLE_STRIP, 44, GL_UNSIGNED_SHORT, &teapotIndices[2313]);
    glDrawElements(GL_TRIANGLE_STRIP, 42, GL_UNSIGNED_SHORT, &teapotIndices[2357]);
    glDrawElements(GL_TRIANGLE_STRIP, 40, GL_UNSIGNED_SHORT, &teapotIndices[2399]);
    glDrawElements(GL_TRIANGLE_STRIP, 38, GL_UNSIGNED_SHORT, &teapotIndices[2439]);
    glDrawElements(GL_TRIANGLE_STRIP, 36, GL_UNSIGNED_SHORT, &teapotIndices[2477]);
    glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, &teapotIndices[2513]);
    glDrawElements(GL_TRIANGLE_STRIP, 32, GL_UNSIGNED_SHORT, &teapotIndices[2547]);
    glDrawElements(GL_TRIANGLE_STRIP, 30, GL_UNSIGNED_SHORT, &teapotIndices[2579]);
    glDrawElements(GL_TRIANGLE_STRIP, 28, GL_UNSIGNED_SHORT, &teapotIndices[2609]);
    glDrawElements(GL_TRIANGLE_STRIP, 26, GL_UNSIGNED_SHORT, &teapotIndices[2637]);
    glDrawElements(GL_TRIANGLE_STRIP, 24, GL_UNSIGNED_SHORT, &teapotIndices[2663]);
    glDrawElements(GL_TRIANGLE_STRIP, 22, GL_UNSIGNED_SHORT, &teapotIndices[2687]);
    glDrawElements(GL_TRIANGLE_STRIP, 20, GL_UNSIGNED_SHORT, &teapotIndices[2709]);
    glDrawElements(GL_TRIANGLE_STRIP, 18, GL_UNSIGNED_SHORT, &teapotIndices[2729]);
    glDrawElements(GL_TRIANGLE_STRIP, 16, GL_UNSIGNED_SHORT, &teapotIndices[2747]);
    glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_SHORT, &teapotIndices[2763]);
    glDrawElements(GL_TRIANGLE_STRIP, 12, GL_UNSIGNED_SHORT, &teapotIndices[2777]);
    glDrawElements(GL_TRIANGLE_STRIP, 10, GL_UNSIGNED_SHORT, &teapotIndices[2789]);
    glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, &teapotIndices[2799]);
    glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_SHORT, &teapotIndices[2807]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[2813]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[2816]);
    glDrawElements(GL_TRIANGLE_STRIP, 200, GL_UNSIGNED_SHORT, &teapotIndices[2819]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[3019]);
    glDrawElements(GL_TRIANGLE_STRIP, 66, GL_UNSIGNED_SHORT, &teapotIndices[3022]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[3088]);
    glDrawElements(GL_TRIANGLE_STRIP, 209, GL_UNSIGNED_SHORT, &teapotIndices[3091]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[3300]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[3303]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[3306]);
    glDrawElements(GL_TRIANGLE_STRIP, 38, GL_UNSIGNED_SHORT, &teapotIndices[3309]);
    glDrawElements(GL_TRIANGLE_STRIP, 15, GL_UNSIGNED_SHORT, &teapotIndices[3347]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[3362]);
    glDrawElements(GL_TRIANGLE_STRIP, 26, GL_UNSIGNED_SHORT, &teapotIndices[3365]);
    glDrawElements(GL_TRIANGLE_STRIP, 9, GL_UNSIGNED_SHORT, &teapotIndices[3391]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[3400]);
    glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_SHORT, &teapotIndices[3403]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[3417]);
    glDrawElements(GL_TRIANGLE_STRIP, 115, GL_UNSIGNED_SHORT, &teapotIndices[3420]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[3535]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[3538]);
    glDrawElements(GL_TRIANGLE_STRIP, 39, GL_UNSIGNED_SHORT, &teapotIndices[3541]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[3580]);
    glDrawElements(GL_TRIANGLE_STRIP, 91, GL_UNSIGNED_SHORT, &teapotIndices[3583]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[3674]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[3677]);
    glDrawElements(GL_TRIANGLE_STRIP, 31, GL_UNSIGNED_SHORT, &teapotIndices[3680]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[3711]);
    glDrawElements(GL_TRIANGLE_STRIP, 67, GL_UNSIGNED_SHORT, &teapotIndices[3714]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[3781]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[3784]);
    glDrawElements(GL_TRIANGLE_STRIP, 23, GL_UNSIGNED_SHORT, &teapotIndices[3787]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[3810]);
    glDrawElements(GL_TRIANGLE_STRIP, 45, GL_UNSIGNED_SHORT, &teapotIndices[3813]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[3858]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[3861]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[3864]);
    glDrawElements(GL_TRIANGLE_STRIP, 32, GL_UNSIGNED_SHORT, &teapotIndices[3867]);
    glDrawElements(GL_TRIANGLE_STRIP, 38, GL_UNSIGNED_SHORT, &teapotIndices[3899]);
    glDrawElements(GL_TRIANGLE_STRIP, 15, GL_UNSIGNED_SHORT, &teapotIndices[3937]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[3952]);
    glDrawElements(GL_TRIANGLE_STRIP, 26, GL_UNSIGNED_SHORT, &teapotIndices[3955]);
    glDrawElements(GL_TRIANGLE_STRIP, 9, GL_UNSIGNED_SHORT, &teapotIndices[3981]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[3990]);
    glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_SHORT, &teapotIndices[3993]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[4007]);
    glDrawElements(GL_TRIANGLE_STRIP, 135, GL_UNSIGNED_SHORT, &teapotIndices[4010]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[4145]);
    glDrawElements(GL_TRIANGLE_STRIP, 76, GL_UNSIGNED_SHORT, &teapotIndices[4148]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[4224]);
    glDrawElements(GL_TRIANGLE_STRIP, 60, GL_UNSIGNED_SHORT, &teapotIndices[4227]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[4287]);
    glDrawElements(GL_TRIANGLE_STRIP, 23, GL_UNSIGNED_SHORT, &teapotIndices[4290]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[4313]);
    glDrawElements(GL_TRIANGLE_STRIP, 26, GL_UNSIGNED_SHORT, &teapotIndices[4316]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[4342]);
    glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_SHORT, &teapotIndices[4345]);
    glDrawElements(GL_TRIANGLE_STRIP, 947, GL_UNSIGNED_SHORT, &teapotIndices[4351]);
    glDrawElements(GL_TRIANGLE_STRIP, 35, GL_UNSIGNED_SHORT, &teapotIndices[5298]);
    glDrawElements(GL_TRIANGLE_STRIP, 31, GL_UNSIGNED_SHORT, &teapotIndices[5333]);
    glDrawElements(GL_TRIANGLE_STRIP, 27, GL_UNSIGNED_SHORT, &teapotIndices[5364]);
    glDrawElements(GL_TRIANGLE_STRIP, 23, GL_UNSIGNED_SHORT, &teapotIndices[5391]);
    glDrawElements(GL_TRIANGLE_STRIP, 20, GL_UNSIGNED_SHORT, &teapotIndices[5414]);
    glDrawElements(GL_TRIANGLE_STRIP, 24, GL_UNSIGNED_SHORT, &teapotIndices[5434]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[5458]);
    glDrawElements(GL_TRIANGLE_STRIP, 28, GL_UNSIGNED_SHORT, &teapotIndices[5461]);
    glDrawElements(GL_TRIANGLE_STRIP, 32, GL_UNSIGNED_SHORT, &teapotIndices[5489]);
    glDrawElements(GL_TRIANGLE_STRIP, 36, GL_UNSIGNED_SHORT, &teapotIndices[5521]);
    glDrawElements(GL_TRIANGLE_STRIP, 76, GL_UNSIGNED_SHORT, &teapotIndices[5557]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[5633]);
    glDrawElements(GL_TRIANGLE_STRIP, 67, GL_UNSIGNED_SHORT, &teapotIndices[5636]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[5703]);
    glDrawElements(GL_TRIANGLE_STRIP, 59, GL_UNSIGNED_SHORT, &teapotIndices[5706]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[5765]);
    glDrawElements(GL_TRIANGLE_STRIP, 51, GL_UNSIGNED_SHORT, &teapotIndices[5768]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[5819]);
    glDrawElements(GL_TRIANGLE_STRIP, 43, GL_UNSIGNED_SHORT, &teapotIndices[5822]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[5865]);
    glDrawElements(GL_TRIANGLE_STRIP, 35, GL_UNSIGNED_SHORT, &teapotIndices[5868]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[5903]);
    glDrawElements(GL_TRIANGLE_STRIP, 27, GL_UNSIGNED_SHORT, &teapotIndices[5906]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[5933]);
    glDrawElements(GL_TRIANGLE_STRIP, 19, GL_UNSIGNED_SHORT, &teapotIndices[5936]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[5955]);
    glDrawElements(GL_TRIANGLE_STRIP, 11, GL_UNSIGNED_SHORT, &teapotIndices[5958]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[5969]);
    glDrawElements(GL_TRIANGLE_STRIP, 30, GL_UNSIGNED_SHORT, &teapotIndices[5972]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[6002]);
    glDrawElements(GL_TRIANGLE_STRIP, 11, GL_UNSIGNED_SHORT, &teapotIndices[6005]);
    glDrawElements(GL_TRIANGLE_STRIP, 18, GL_UNSIGNED_SHORT, &teapotIndices[6016]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[6034]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &teapotIndices[6037]);
    glDrawElements(GL_TRIANGLE_STRIP, 5, GL_UNSIGNED_SHORT, &teapotIndices[6040]);
    glDrawElements(GL_TRIANGLE_STRIP, 122, GL_UNSIGNED_SHORT, &teapotIndices[6045]);
    glDrawElements(GL_TRIANGLE_STRIP, 75, GL_UNSIGNED_SHORT, &teapotIndices[6167]);
    glDrawElements(GL_TRIANGLE_STRIP, 71, GL_UNSIGNED_SHORT, &teapotIndices[6242]);
    glDrawElements(GL_TRIANGLE_STRIP, 67, GL_UNSIGNED_SHORT, &teapotIndices[6313]);
    glDrawElements(GL_TRIANGLE_STRIP, 63, GL_UNSIGNED_SHORT, &teapotIndices[6380]);
    glDrawElements(GL_TRIANGLE_STRIP, 59, GL_UNSIGNED_SHORT, &teapotIndices[6443]);
    glDrawElements(GL_TRIANGLE_STRIP, 55, GL_UNSIGNED_SHORT, &teapotIndices[6502]);
    glDrawElements(GL_TRIANGLE_STRIP, 51, GL_UNSIGNED_SHORT, &teapotIndices[6557]);
    glDrawElements(GL_TRIANGLE_STRIP, 47, GL_UNSIGNED_SHORT, &teapotIndices[6608]);
    glDrawElements(GL_TRIANGLE_STRIP, 43, GL_UNSIGNED_SHORT, &teapotIndices[6655]);
    glDrawElements(GL_TRIANGLE_STRIP, 39, GL_UNSIGNED_SHORT, &teapotIndices[6698]);
    glDrawElements(GL_TRIANGLE_STRIP, 35, GL_UNSIGNED_SHORT, &teapotIndices[6737]);
    glDrawElements(GL_TRIANGLE_STRIP, 31, GL_UNSIGNED_SHORT, &teapotIndices[6772]);
    glDrawElements(GL_TRIANGLE_STRIP, 27, GL_UNSIGNED_SHORT, &teapotIndices[6803]);
    glDrawElements(GL_TRIANGLE_STRIP, 23, GL_UNSIGNED_SHORT, &teapotIndices[6830]);
    glDrawElements(GL_TRIANGLE_STRIP, 19, GL_UNSIGNED_SHORT, &teapotIndices[6853]);
    glDrawElements(GL_TRIANGLE_STRIP, 15, GL_UNSIGNED_SHORT, &teapotIndices[6872]);
    glDrawElements(GL_TRIANGLE_STRIP, 11, GL_UNSIGNED_SHORT, &teapotIndices[6887]);
    glDrawElements(GL_TRIANGLE_STRIP, 7, GL_UNSIGNED_SHORT, &teapotIndices[6898]);

    /*
     * Disable vertex arrays
     */
    glDisableClientState(GL_VERTEX_ARRAY);
    /*
     * Disable normal arrays
     */
    glDisableClientState(GL_NORMAL_ARRAY);

    //------------------------------------------------------------------------------
    // Flush buffers to screen
    glFlush();
    glPopMatrix();
}
