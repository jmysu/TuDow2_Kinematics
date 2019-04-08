/* kinematics.cpp
 *
 * See  https://github.com/jmysu/TuDow2_Kinematics
 *      https://github.com/jmysu/TuDow2_Kinematics/tree/master/IK
 *
 */
#include "kinematics.h"

qreal rad(qreal d)
{
    return qDegreesToRadians(d);
}

qreal deg(qreal r)
{
    return qRadiansToDegrees(r);
}

qreal distance(qreal x, qreal y)
{
    return qSqrt(x*x + y*y);
}

qreal KheightRad(qreal len, qreal rad)
{
    qreal h = len*qSin(rad);

    QString text;
    text.sprintf("Kheight:len=% 6.3f rad=%+06.3f => height=% 6.3f", len, rad, h);
    qDebug() << text;

    return h;
}
qreal KheightDeg(qreal len, qreal deg)
{
    qreal h = len*qSin(qDegreesToRadians(deg));

    QString text;
    text.sprintf("Kheight:len=% 6.3f deg=%+06.3f° => height=% 6.3f", len, deg, h);
    qDebug() << text;

    return h;
}
/*
 * Kinematics utility
 * ref:https://pythonrobotics.readthedocs.io/en/latest/modules/arm_navigation.html
 *
 *
 *                       Wrist
 *                        (*)   .
 *                        *|  .
 *                len l1 *T1.
 *                      * .|
 *                     *.  |
 *                   (*)---+--- Elbow
 *                  * |    |
 *      len l0    *   |    |
 *              *     |    |
 *            *T0     |    |
 *          (*)-------+----+--- Shoulder
 */
qreal KposXfromAnglesDeg(qreal dt0, qreal dt1, qreal l0, qreal l1)
{
    qreal rt0 = qDegreesToRadians(dt0);
    qreal rt1 = qDegreesToRadians(dt1);

    qreal Ex = l0*qCos(rt0);
    qreal Ey = l0*qSin(rt0);
    qreal Wx = Ex + l1*qCos(rt0+rt1);
    qreal Wy = Ey + l1*qSin(rt0+rt1);

    QString text;
    text.sprintf("(%+06.3f,%+06.3f)", Wx, Wy);
    qDebug() << QString("d0=%1°,d1=%2°,l0=%3,l1=%4 =>(%5").arg(dt0).arg(dt1).arg(l0).arg(l1).arg(text);

    return Wx;
}
qreal KposXfromAnglesRad(qreal rt0, qreal rt1, qreal l0, qreal l1)
{
    qreal Ex = l0*qCos(rt0);
    qreal Ey = l0*qSin(rt0);
    qreal Wx = Ex + l1*qCos(rt0+rt1);
    qreal Wy = Ey + l1*qSin(rt0+rt1);

    QString text;
    text.sprintf("Kpos:r0=%+06.3f,r1=%+06.3f, l0=% 5.1f, l1=% 5.1f =>(%+06.3f,%+06.3f)", rt0, rt1, l0, l1, Wx, Wy);
    qDebug() << text;
    return Wx;
}
qreal KposYfromAnglesDeg(qreal dt0, qreal dt1, qreal l0, qreal l1)
{
    qreal rt0 = qDegreesToRadians(dt0);
    qreal rt1 = qDegreesToRadians(dt1);

    qreal Ex = l0*qCos(rt0);
    qreal Ey = l0*qSin(rt0);
    qreal Wx = Ex + l1*qCos(rt0+rt1);
    qreal Wy = Ey + l1*qSin(rt0+rt1);

    //QString text;
    //text.sprintf("(%+06.3f,%+06.3f)", Wx, Wy);
    //qDebug() << QString("d0=%1°,d1=%2°,l0=%3,l1=%4 =>(%5").arg(dt0).arg(dt1).arg(l0).arg(l1).arg(text);

    return Wy;
}
qreal KposYfromAnglesRad(qreal rt0, qreal rt1, qreal l0, qreal l1)
{
    qreal Ex = l0*qCos(rt0);
    qreal Ey = l0*qSin(rt0);
    qreal Wx = Ex + l1*qCos(rt0+rt1);
    qreal Wy = Ey + l1*qSin(rt0+rt1);

    //QString text;
    //text.sprintf("r0=%+06.3f,r1=%+06.3f, l0=% 5.1f, l1=% 5.1f =>(%+06.3f,%+06.3f)", rt0, rt1, l0, l1, Wx, Wy);
    //qDebug() << text;

    return Wy;
}
/*
 * Inverse-Kinematics
 * ref:https://appliedgo.net/roboticarm/
 *
 *
 *                    *
 *                  *A2*
 *      len1=a    *     *  len2=b
 *              *        *
 *            *A1         *
 *          **************** (x,y)
 *               dist=c
 *
 * It will have two positive solutions if b sin γ < c < b,
 * only one positive solution if c = b sin γ,
 * and no solution if c < b sin γ or c ≥ b.
 */
qreal CosinLaw(qreal a, qreal b, qreal c)
{
    qreal r = (a*a + b*b - c*c) / (2 * a * b);
    qreal rr= qAcos(r);
    //qDebug() << "lawOfCosine" << a << b << c;
    //qDebug() << " a^2+b^2-c^2/2ab" << r;
    //qDebug() << " ACos()" << rr;
    QString t;
    t.sprintf("ConsinLaw(%-6.3f,%-6.3f,%-6.3f)=%-6.3f", a,b,c, rr);
    qDebug() << t;

    return rr;
}

qreal IKradA1(qreal x, qreal y, qreal len1, qreal len2)
{
//qDebug() << Q_FUNC_INFO << x << y << len1 << len2;
    //get the length of line dist.
    qreal dist = distance(x, y);

    //Calculating angle D1 is trivial. Atan2 is a modified arctan() function that returns unambiguous results.
    qreal D1 = qAtan2(y, x);
    //D2 can be calculated using the law of cosines where a = dist, b = len1, and c = len2.
    qreal D2 = CosinLaw(dist, len1, len2);

    //Then A1 is simply the sum of D1 and D2.
    qreal A1 = D1 + D2;

    //A2 can also be calculated with the law of cosine, but this time with a = len1, b = len2, and c = dist.
    qreal A2 = CosinLaw(len1, len2, dist);

//QString t;
//t.sprintf("x,y distance (%6.3f,%6.3f) %6.3f", x,y, dist);
//qDebug() << t;
//t.sprintf("D1:%6.3f D2:%6.3f, A1=%6.3f A2=%6.3f", qRadiansToDegrees(D1),qRadiansToDegrees(D2), qRadiansToDegrees(A1),  qRadiansToDegrees(A2));
//qDebug() << t;

    return A1;
}

qreal IKradA2(qreal x, qreal y, qreal len1, qreal len2)
{
    //get the length of line dist.
    qreal dist = distance(x, y);
    //Calculating angle D1 is trivial. Atan2 is a modified arctan() function that returns unambiguous results.
    //qreal D1 = qAtan2(y, x);
    //D2 can be calculated using the law of cosines where a = dist, b = len1, and c = len2.
    //qreal D2 = lawOfCosines(dist, len1, len2);

    //Then A1 is simply the sum of D1 and D2.
    //qreal A1 = D1 + D2;

    //A2 can also be calculated with the law of cosine, but this time with a = len1, b = len2, and c = dist.
    qreal A2 = CosinLaw(len1, len2, dist);

    return A2;
}

qreal len1=10;
qreal len2=10;
void testXY(qreal x, qreal y)
{
    qDebug() <<"\t"<< QString("Move to (%1,%2) => A1=%3° A2=%4°")
                .arg(x).arg(y).arg(deg(IKradA1(x,y,len1,len2))).arg(deg(IKradA2(x,y,len1,len2)));
}
void testTheta01(qreal t0, qreal t1)
{
    qDebug() <<"\t"<< QString("Rotate to T0=%1° T1=%2° => (%3, %4)")
                .arg(t0).arg(t1).arg((int)KposXfromAnglesDeg(t0,t1,len1,len2)).arg((int)KposYfromAnglesDeg(t0,t1,len1,len2));
}
void testIK()
{
    qDebug() << "\nLet's rotate 90/0, 0/90, 90/90degrees";
    testTheta01(90,0);
    testTheta01(0,90);
    testTheta01(90,90);


    qDebug() << "\nMove to (10,10)";
    testXY(10,10);


    qDebug() << "\nTry 45/90 degree";
    testTheta01(45,90);
    testXY(0,14);

    qDebug() << "\nIf y is 0 and x = Sqrt(10^2 + 10^2),\nthen alpha should become 45 degrees and beta should become 90 degrees.";
    testXY(qSqrt(200),0);
    qDebug() << "The other side.";
    testXY(0,qSqrt(200));


    qDebug() << "\nAn extreme case: (20,0). The arm needs to stretch along the y axis.";
    testTheta01(0,0);
    testXY(20,0);

    qDebug() << "\nAnd : (0,20).";
    testTheta01(90,90);
    testXY(0,20);

    testXY(0,0);
    testXY(20,20);
}


/* XYZ2AlphaBetaGamma()
 *
 * Input: XYZ
 * Output: Alpha Beta Gamma
 *
 * https://github.com/jmysu/TuDow2_Kinematics/blob/master/IK/readme.md
 */
void XYZ2AlphaBetaGamma(qreal x, qreal y, qreal z, qreal L1, qreal L2, qreal *Alpha, qreal *Beta, qreal *Gamma)
{
    qDebug() << Q_FUNC_INFO;

    qreal alpha = qAtan2(y,x);
    *Alpha      = qRadiansToDegrees(alpha);

    qreal r1    = distance(x, y);
    qreal r2    = z;
    qreal r3    = distance(r1, r2);
    qreal A1    = CosinLaw(r3, L1, L2);
    qreal A2    = CosinLaw(L1, L2, r3);
    qreal Cr2   = qAtan2(r1, r2);   //qAtan2 got r,r2 swapped vs Excel Atan2(r2, r1)
                                    //QTBUG-12515 qAtan2 calls atan2/atan2f with arguments in wrong order
    *Beta       = qRadiansToDegrees(Cr2-A1);
    *Gamma      = 180.0 - qRadiansToDegrees(A2);
}
/*
bool validateIK(qreal x, qreal y, qreal z, qreal z0, qreal L1, qreal L2)
{
    qDebug() << "\n   " << Q_FUNC_INFO << x << y << z;
    QString t;

    qreal alpha = qAtan2(y, x);

    qreal r1 = distance(x, y);
    qreal r2 = z - z0;
    qreal r3 = distance(r1, r2);
    t.sprintf("r1,r2,r3: %6.3f, %6.3f, %6.3f", r1, r2, r3);
    qDebug() << t;

    qreal A1= CosinLaw(r3, L1, L2);
    qDebug() << "A1:" << A1 << qRadiansToDegrees(A1);

    qreal A2= CosinLaw(L1, L2, r3);
    qDebug() << "A2:" << A2 << qRadiansToDegrees(A2);

    qreal degAlpha= qRadiansToDegrees(alpha);
    t.sprintf("Alpha=%6.1f°", degAlpha);
    qDebug() << t;

    qreal PR2= qAtan2(r1, r2); //***!!!
    qreal beta= PR2-A1;
    qreal degBeta = qRadiansToDegrees(beta);
    t.sprintf("Beta =%6.1f°", degBeta );
    qDebug() << t;

    qreal degGamma= 180.0-qRadiansToDegrees(A2);
    t.sprintf("Gamma=%6.1f°", degGamma);
    qDebug() << t;
    t.sprintf("Adjusted: α=%4d° β=%4d° γ=%4d°",
                //qRound(90.0+degAlpha), qRound(90.0-degBeta), qRound(-degGamma));
                qRound(90.0+degAlpha), qRound(degBeta), qRound(degGamma));
    qDebug() << t;
    return true;
}
*/
