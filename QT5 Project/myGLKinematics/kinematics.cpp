
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
 */
qreal lawOfCosines(qreal a, qreal b, qreal c)
{
    return qAcos((a*a + b*b - c*c) / (2 * a * b));
}

qreal IKanglesA1(qreal x, qreal y, qreal len1, qreal len2)
{
    //get the length of line dist.
    qreal dist = distance(x, y);
    //Calculating angle D1 is trivial. Atan2 is a modified arctan() function that returns unambiguous results.
    qreal D1 = qAtan2(y, x);
    //D2 can be calculated using the law of cosines where a = dist, b = len1, and c = len2.
    qreal D2 = lawOfCosines(dist, len1, len2);

    //Then A1 is simply the sum of D1 and D2.
    qreal A1 = D1 + D2;

    //A2 can also be calculated with the law of cosine, but this time with a = len1, b = len2, and c = dist.
    //qreal A2 = lawOfCosines(len1, len2, dist);

    return A1;
}

qreal IKanglesA2(qreal x, qreal y, qreal len1, qreal len2)
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
    qreal A2 = lawOfCosines(len1, len2, dist);

    return A2;
}

qreal len1=10;
qreal len2=10;
void testXY(qreal x, qreal y)
{
    qDebug() <<"\t"<< QString("Move to (%1,%2) => A1=%3° A2=%4°")
                .arg(x).arg(y).arg(deg(IKanglesA1(x,y,len1,len2))).arg(deg(IKanglesA2(x,y,len1,len2)));
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


