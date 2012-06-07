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
        while (*angle < -360)
            *angle += 360;
        while (*angle > 360)
            *angle -= 360;
    }

    static qreal norm(qreal min, qreal max, qreal val) {
        qreal dist=max-min;
        qreal norm=(val-min)/dist;
        return norm;
    }

    static qreal deNorm(qreal min, qreal max, qreal val) {
        qreal dist=max-min;
        qreal deNorm=min+val*dist;
        return deNorm;
    }
    static qreal sinGrad( qreal grad) { return sin(grad*PI/180.0f); }
    static qreal cosGrad( qreal grad) { return cos(grad*PI/180.0f); }

    static const qreal MAXQREAL;
    static const qreal MINQREAL;

    static const quint32 NUMBER_OF_RANDOMS=300;
    static const qreal randoms[NUMBER_OF_RANDOMS];

    static const char* const XmlModeAttrib;
    static const char* const XmlNameAttrib;
    static const char* const XmlIdAttrib;
    static const char* const XmlXAttrib;
    static const char* const XmlYAttrib;
    static const char* const XmlZAttrib;
    static const char* const XmlAngleVarAttrib;
    static const char* const XmlAngleAttrib;
    static const char* const XmlDistanceAttrib;
    static const char* const XmlGroupsAttrib;
    static const char* const XmlGroupRatioAttrib;
    static const char* const XmlNumberAttrib;
    static const char* const XmlNoiseAttrib;
    static const char* const XmlNoiseFreqAttrib;
    static const char* const XmlHeightVarAttrib;
    static const char* const XmlHeightAttrib;
    static const char* const XmlRadiusAttrib;

    static const char* const XmlScaleTag;
    static const char* const XmlRotationTag;
    static const char* const XmlTranslationTag;
    static const char* const XmlRestRotationTag;
    static const char* const XmlRestTranslationTag;
    static const char* const XmlSphereTag;
    static const char* const XmlBoxTag;

    static const char* const FuzzAndModeMin;
    static const char* const FuzzAndModeProduct;

    static const char* const FuzzOrModeMax;
    static const char* const FuzzOrModeSum;

    static const char* const FuzzTimerModeIfStopped;
    static const char* const FuzzTimerModeAlways;

    static const char* const FuzzFuzzModeTrapezoid;
    static const char* const FuzzFuzzInterpolationSine;

    static const char* const  ChannelName_Sound_x;
    static const char* const  ChannelName_Sound_ox;
    static const char* const  ChannelName_Sound_d;
    static const char* const  ChannelName_Sound_a;
    static const char* const  ChannelName_Sound_f;

    static qreal getRand( quint32 index ) { return randoms[index%NUMBER_OF_RANDOMS]; }
};
#endif // BRAINIACGLOBALS_H
