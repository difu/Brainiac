// Brainiac is a free and open source tool for the creation of crowd simulation

// Copyright (C) 2012  Dirk Fuchs dirkfux@googlemail.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#ifndef BRAINIACGLOBALS_H
#define BRAINIACGLOBALS_H

#include<QtCore>
#include<QColor>
#include "gui/coordcrossdrawable.h"
#ifdef BRAINIAC_SUPPRESS_THIRD_PARTY_WARNINGS
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-parameter"
#endif
#include "cml/cml.h"
#include "cml/mathlib/matrix_rotation.h"
#ifdef BRAINIAC_SUPPRESS_THIRD_PARTY_WARNINGS
    #pragma clang diagnostic pop
#endif

/**
 * @brief Global data used by several components of Brainiac
 *
 */
class BrainiacGlobals
{
public:
    static const qreal PI;
    enum Direction {NORTH, SOUTH, EAST, WEST};
    enum ItemType {AND, OR, FUZZ, DEFUZZ, OUTPUT, INPUT, NOISE, TIMER, SPHERE, CUBE, TUBE, GROUP, AGENT, ACTION, TRANSITION};
    enum SegmentType {SPHERESEGMENT=BrainiacGlobals::SPHERE, BOXSEGMENT=BrainiacGlobals::CUBE, TUBESEGMENT=BrainiacGlobals::TUBE, NOPRIMITIVE};
    enum RotTrans {RX, RY, RZ, TX, TY, TZ };
    enum AnimationType{ STATIC, LOCOMOTION, RAMP, TURNING, NOTYPE };
    static QColor getColorFromBrainiacColorValue(qreal colVal);
    static const QList<QColor> EditorColors;
    static void normalizeAngle(qreal *angle)
    {
        while (*angle < -360.0)
            *angle += 360.0;
        while (*angle > 360.0)
            *angle -= 360.0;
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

    static void decomposeMatrix(const cml::matrix33d_r& matrix, const QList<RotTrans> &order, qreal *rx, qreal *ry, qreal *rz, bool reverse=false);
    static cml::EulerOrder getCmlOrderFromBrainiacOrder(const QList<RotTrans> &order, bool reverse=false);

    static qreal grad2rad(qreal grad) { return grad*PI/180.0; }
    static qreal rad2grad(qreal rad) { return rad*180.0/PI; }

    static qreal sinGrad( qreal grad) { return sin(grad*PI/180.0); }
    static qreal cosGrad( qreal grad) { return cos(grad*PI/180.0); }

    static const qreal MAXQREAL;
    static const qreal MINQREAL;
    static float SegmentTesselationDetailRatio;

    static const quint32 NUMBER_OF_RANDOMS=300;
    static const qreal randoms[NUMBER_OF_RANDOMS];

    static const qreal ActionTriggerValue; /**< value that triggers the action */
    static const qreal ActionForceTriggerValue; /**< value that forces the action (do not check for latch etc.) */

    static const char* const XmlColorAttrib;
    static const char* const XmlEnabledAttrib;
    static const char* const XmlEndFrameAttrib; /**< end frame of simulation */
    static const char* const XmlModeAttrib;
    static const char* const XmlNameAttrib;
    static const char* const XmlIdAttrib;
    static const char* const XmlXAttrib;
    static const char* const XmlYAttrib;
    static const char* const XmlZAttrib;
    static const char* const XmlAngleVarAttrib;
    static const char* const XmlAngleAttrib;
    static const char* const XmlDistanceAttrib;
    static const char* const XmlFileNameAttrib;
    static const char* const XmlFpsAttrib; /**< Frames per second of scene/simulation */
    static const char* const XmlGroupsAttrib;
    static const char* const XmlGroupRatioAttrib;
    static const char* const XmlInputAttrib;
    static const char* const XmlNumberAttrib;
    static const char* const XmlNoiseAttrib;
    static const char* const XmlNoiseFreqAttrib;
    static const char* const XmlOutputAttrib;
    static const char* const XmlHeightVarAttrib;
    static const char* const XmlHeightAttrib;
    static const char* const XmlRadiusAttrib;
    static const char* const XmlEditorXPosAttrib; /**< X Position in an editor */
    static const char* const XmlEditorYPosAttrib; /**< Y Position in an editor */
    static const char* const XmlStartFrameAttrib; /**< start frame of simulation */
    static const char* const XmlTreeDefaultActionAttrib; /**< contains the name of the tree default action */
    static const char* const XmlTreeActionAttrib;
    static const char* const XmlTreeTransitionAttrib;
    static const char* const XmlTriggersAttrib;

    static const char* const XmlDirectoryTag;
    static const char* const XmlGroupsTag;
    static const char* const XmlInputTag;
    static const char* const XmlOutputTag;
    static const char* const XmlSizeTag;
    static const char* const XmlRotationTag;
    static const char* const XmlTranslationTag;
    static const char* const XmlRestRotationTag;
    static const char* const XmlRestTranslationTag;
    static const char* const XmlSphereTag;
    static const char* const XmlBoxTag;
    static const char* const XmlTubeTag;
    static const char* const XmlRotTransOrderTag;
    static const char* const XmlMotionTreesTag;
    static const char* const XmlMotionTreeTag;
    static const char* const XmlPlaceTag;
    static const char* const XmlSceneTag;
    static const char* const XmlSimulationTag;
    static const char* const XmlSimulationDirectoryOutputTag;
    static const char* const XmlTreeTransitionActionConnectionTag;
    static const char* const XmlTreeActionTransitionConnectionTag;
    static const char* const XmlTreeActionTag;
    static const char* const XmlTreeTransitionTag;
    static const char* const XmlTriggerTag;

    static const char* const FuzzAndModeMin;
    static const char* const FuzzAndModeProduct;

    static const char* const FuzzOrModeMax;
    static const char* const FuzzOrModeSum;

    static const char* const FuzzTimerModeIfStopped;
    static const char* const FuzzTimerModeAlways;

    static const char* const FuzzFuzzModeTrapezoid;
    static const char* const FuzzFuzzInterpolationSine;

    static const QString ChannelName_Sound_x;
    static const QString ChannelName_Sound_ox;
    static const QString ChannelName_Sound_d;
    static const QString ChannelName_Sound_a;
    static const QString ChannelName_Sound_f;

    static const char* const DefaultLatchName;

    static const QList<QString> ChannelNames_Latches;
    static const QList<QString> ChannelNames_Phases;

    static const QList<RotTrans> AgentDefaultRotTrans;

    static const QString ChannelPhaseOffsetSuffix;
    static const QString ChannelRunningSuffix;

    static QColor defaultXColor;
    static QColor defaultYColor;
    static QColor defaultZColor;
    static QColor defaultRadiusColor;
    static QColor defaultLengthColor;
    static QColor defaultDiameterColor;
    static QColor DefaultLatchColor;

    static Qt::Key AnimationOneFrameForwardKey; /**< default key to go one frame further */
    static Qt::Key AnimationOneFrameBackwardKey; /**< default key to go one frame back */
    static Qt::Key AnimationPauseKey; /**< default key to pause */
    static Qt::Key ToogleFollowAgentKey; /**< default key to toggle to follow an agent */
    static Qt::Key ToggleSegmentCoordCrossesKey; /**< default key to toggle an segments pivot point */
    static Qt::Key ToggleAgentPositionMarkerKey; /**< default key to toggle an agents position and orientation marker */

    static const quint32 animationFileFormatMagicNumber=0xA1B2C3D4; /**< Magic number to identify the Brainiac Animation Fileformat */
    static const qint32 animationFileFormatVersion=100; /**< Version of Brainiac Animation File Format */

    static qreal getRand( quint32 index ) { return randoms[index%NUMBER_OF_RANDOMS]; }

    static osg::ref_ptr<CoordCrossDrawable> CoordCross;
};
#endif // BRAINIACGLOBALS_H
