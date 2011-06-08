#ifndef BRAINIACGLOBALS_H
#define BRAINIACGLOBALS_H

#include<QtCore>
#include<QColor>

class BrainiacGlobals
{
public:
    static const qreal PI=3.14159265;
    enum ItemType {AND, OR, FUZZ, DEFUZZ, OUTPUT, INPUT, NOISE, TIMER, SPHERE, CUBE, GROUP, AGENT};
    enum RotationOrder {XYZ, XZY, YXZ, YZX, ZXY, ZYX };
    static QColor getColorFromBrainiacColorValue(qreal colVal);
    static void normalizeAngle(qreal *angle)
    {
        while (*angle < 0)
            *angle += 360 * 16;
        while (*angle > 360 * 16)
            *angle -= 360 * 16;
    }
    static qreal sinGrad( qreal grad) { return sin(grad*PI/180.0f); }
    static qreal cosGrad( qreal grad) { return cos(grad*PI/180.0f); }

    static const quint32 NUMBER_OF_RANDOMS=300;
    static const qreal randoms[NUMBER_OF_RANDOMS];

    static qreal getRand( quint32 index ) { return randoms[index%NUMBER_OF_RANDOMS]; }
};
#endif // BRAINIACGLOBALS_H
