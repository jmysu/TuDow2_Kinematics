#ifndef KINEMATICS_H
#define KINEMATICS_H
#include <QtMath>
#include <QDebug>

qreal deg(qreal r);
qreal distance(qreal x, qreal y);

//Kinematics
qreal KheightRad(qreal len, qreal rad);
qreal KheightDeg(qreal len, qreal deg);
qreal KposXfromAnglesDeg(qreal t0, qreal t1, qreal l0, qreal l1);
qreal KposYfromAnglesDeg(qreal t0, qreal t1, qreal l0, qreal l1);
qreal KposXfromAnglesRad(qreal t0, qreal t1, qreal l0, qreal l1);
qreal KposYfromAnglesRad(qreal t0, qreal t1, qreal l0, qreal l1);
//IK
qreal CosinLaw(qreal a, qreal b, qreal c);
qreal IKradA1(qreal x, qreal y, qreal len1, qreal len2);
qreal IKradA2(qreal x, qreal y, qreal len1, qreal len2);


void testXY(qreal x, qreal y);
void testIK();
//bool validateIK(qreal x, qreal y, qreal z, qreal z0, qreal L1, qreal L2);
void XYZ2AlphaBetaGamma(qreal x, qreal y, qreal z, qreal L1, qreal L2, qreal *Alpha, qreal *Beta, qreal *Gamma);

#endif // KINEMATICS_H
