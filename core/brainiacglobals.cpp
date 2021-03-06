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


#include "brainiacglobals.h"
#include <osgDB/Registry>
#include <limits>

cml::EulerOrder BrainiacGlobals::getCmlOrderFromBrainiacOrder(const QList<RotTrans> &order, bool reverse)
{
    cml::EulerOrder cmlEulerOrder=cml::euler_order_xyz;
    QList<BrainiacGlobals::RotTrans> rotList;
    foreach(BrainiacGlobals::RotTrans rot,order) {
        switch( rot ) {
        case BrainiacGlobals::RX:
        case BrainiacGlobals::RY:
        case BrainiacGlobals::RZ:
            if(reverse)
                rotList.prepend(rot);
            else
                rotList.append(rot);
        break;
        default:
            continue;
        }
    }
    Q_ASSERT(rotList.count()==3);

    if(rotList.first()==BrainiacGlobals::RX && rotList.at(1)==BrainiacGlobals::RY && rotList.last()==BrainiacGlobals::RZ) {
        cmlEulerOrder=cml::euler_order_zyx;
    } else if(rotList.first()==BrainiacGlobals::RX && rotList.at(1)==BrainiacGlobals::RZ && rotList.last()==BrainiacGlobals::RY) {
        cmlEulerOrder=cml::euler_order_yzx;
    } else if(rotList.first()==BrainiacGlobals::RZ && rotList.at(1)==BrainiacGlobals::RY && rotList.last()==BrainiacGlobals::RX) {
        cmlEulerOrder=cml::euler_order_xyz;
    } else if(rotList.first()==BrainiacGlobals::RZ && rotList.at(1)==BrainiacGlobals::RX && rotList.last()==BrainiacGlobals::RY) {
        cmlEulerOrder=cml::euler_order_yxz;
    } else if(rotList.first()==BrainiacGlobals::RY && rotList.at(1)==BrainiacGlobals::RZ && rotList.last()==BrainiacGlobals::RX) {
        cmlEulerOrder=cml::euler_order_xzy;
    } else if(rotList.first()==BrainiacGlobals::RY && rotList.at(1)==BrainiacGlobals::RX && rotList.last()==BrainiacGlobals::RZ) {
        cmlEulerOrder=cml::euler_order_zxy;
    } else {
        qCritical() << __PRETTY_FUNCTION__ << "This never should happen!";
    }
    return cmlEulerOrder;
}

void BrainiacGlobals::decomposeMatrix( const cml::matrix33d_r& matrix, const QList<RotTrans> &order, qreal *rx, qreal *ry, qreal *rz, bool reverse)
{
    cml::EulerOrder cmlEulerOrder=BrainiacGlobals::getCmlOrderFromBrainiacOrder(order,reverse);
    cml::matrix_to_euler(matrix,*rx,*ry,*rz,cmlEulerOrder);

}

QColor BrainiacGlobals::getColorFromBrainiacColorValue(qreal colVal)
{
    qreal val=qBound(0.0,colVal,1.0);
    if( val<0.1) {
        return QColor::fromHsvF(val,10*val,10*val);
    } else if( val<0.9 ) {
        return QColor::fromHsvF(val,1,1);
    } else {
        return QColor::fromHsvF(val,1-(val-0.9)*10,1);
    }
}

const qreal BrainiacGlobals::randoms[]={
    0.939368802662106,0.752327802448661,0.830756714373035,0.318679197781371,0.403914761625021,0.0328672565374148,0.606305462677792,0.772753571899635,0.967847833401184,0.48746581055525,
    0.973149274026394,0.47382440091085,0.497166866699434,0.542175122152699,0.268041429648743,0.368381039792546,0.213589666848581,0.251000168848833,0.6741012826737,0.943798379114931,
    0.403317017082276,0.820647306089583,0.793727857329625,0.812770945467626,0.495446422996086,0.867636279492501,0.30677570202543,0.641434908747705,0.0830424485391639,0.947434022469977,
    0.277297182179435,0.109298123033277,0.592518979920559,0.695747959338945,0.180322665509049,0.86800228894851,0.572754897608963,0.201173083097373,0.98692327637124,0.151644603013857,
    0.526006268684828,0.627621848776357,0.105750389151961,0.652052915969399,0.968078104665086,0.281605480161389,0.770067106142161,0.0168275199596266,0.943384776063287,0.1963311922174,
    0.671799935308695,0.255548286274582,0.567593146871804,0.320208646249441,0.572289060370419,0.990337043555222,0.69076420235783,0.755822242184365,0.0102732921229567,0.791625883489964,
    0.409670204551208,0.416450174868672,0.531406318520052,0.369801569554788,0.679773034069743,0.436148101443649,0.483584544115171,0.610288466780908,0.533838176976435,0.687241347181256,
    0.965018939593929,0.946136469371901,0.582085603693177,0.592386380142671,0.0368742217861318,0.351460340736345,0.303026376106789,0.84939637315621,0.582123666235699,0.944921759652395,
    0.717710021395821,0.763628607402353,0.922847230627394,0.339269249337356,0.0341453392803892,0.16837981784083,0.418479750621284,0.125793115658233,0.742414175852421,0.737530899318301,
    0.129565072429028,0.297076599088349,0.00185661437994966,0.201351245954882,0.320586421280549,0.683919714790527,0.385174813996745,0.176266947823791,0.119743384275363,0.199692778888966,

    0.678719515287415,0.564993860756175,0.661831642073718,0.119134743555101,0.918304115196481,0.264963149777017,0.173171011867762,0.455281711690155,0.43455595592096,0.107087258541242,
    0.852346570480034,0.638518439829092,0.523293946899674,0.422980245517252,0.506573444538386,0.939132462575905,0.1870538789961,0.188795792752927,0.198902437061815,0.370796239775707,
    0.729350492658245,0.194262373308945,0.673428548536005,0.300147741316458,0.199058019250717,0.31517612287854,0.314980501869382,0.364902688792007,0.945416063921403,0.366509246785725,
    0.394095457096963,0.826118580750681,0.676816166382768,0.813771682469991,0.856250368545044,0.759134589435298,0.682290519786608,0.899320286637803,0.181094661046668,0.173414694508402,
    0.825265959951171,0.13954458798969,0.297378395206049,0.112188075750602,0.684546270060867,0.325497703316515,0.330091167149586,0.527199884620352,0.755651892101415,0.0364369192995184,
    0.16883995817177,0.651471116370097,0.972711108217045,0.691367257045997,0.774644326686282,0.243757472626633,0.331318465381489,0.522144623745987,0.533185031425756,0.383057901462657,
    0.0285374777608247,0.77271947834511,0.266302703398136,0.0213429905118616,0.0580332654137727,0.398038858294711,0.133526857203275,0.719565631111614,0.454809588023938,0.553950694762747,
    0.198067662440362,0.498520430295081,0.551951520688508,0.00277114666732814,0.956593795594966,0.525398285198868,0.446022607899284,0.990209772029644,0.401944674483953,0.0627274136025946,
    0.951341534244481,0.22595029093398,0.918504753557226,0.253216017373468,0.317400978533573,0.885833040329814,0.420246641607175,0.566853705051834,0.87744818450253,0.37738053951449,

    0.189971156957657,0.688658077127428,0.43407778701852,0.1759705916947,0.799495236843615,0.110916410558261,0.0450687952309998,0.404608412453932,0.0358009627001685,0.620850262756676,
    0.253785730821431,0.167998892711218,0.875746100472487,0.101180145132972,0.835996309834169,0.435126657219477,0.514517236006956,0.555804986622235,0.269119964743776,0.16074712023056,
    0.548026608836057,0.760526754242662,0.536586756155568,0.597359648421978,0.853083669135433,0.41176402784032,0.470583538386659,0.341493630684944,0.988348226215567,0.562912360212366,
    0.446491564794822,0.0524275814979589,0.0717205249280859,0.938489142716747,0.75048698556817,0.0603653657918315,0.355589730165509,0.9952572455487,0.208548914565622,0.766796910799904,
    0.672003931713366,0.998756473592834,0.125069235206336,0.7004374268814,0.685224714574918,0.560311435644408,0.370079390845714,0.8365582572062,0.427295386031293,0.956480329334369,
    0.666622375046312,0.715108077995986,0.939316922623693,0.568537958790031,0.0579483141539434,0.543814339229606,0.361347410449387,0.138049589416767,0.925069731122431,0.777115632839617,
    0.449602013149548,0.455616566695838,0.228973108094973,0.191595163952393,0.0214391262345579,0.0688115460595498,0.27176703834574,0.695477743155784,0.0851071764412445,0.613147261227127,
    0.813699388312298,0.616256776616261,0.57915331314658,0.203239561363262,0.907866790107658,0.999797668611542,0.480642827973714,0.752359535556039,0.0842647665901914,0.160108930195172,
    0.124914341118771,0.165172478901951,0.245406167290366,0.865796170766096,0.573633421100698,0.634109800432995,0.746014676745201,0.802056098779211,0.7616641710915,0.0936152917243476,
    0.991349259574559,0.36159173620198,0.514137232220538,0.593176028780888,0.577729841447816,0.090325321097616,0.748561434764159,0.549942131216852,0.543134604435192,0.831165896396961
};

const qreal BrainiacGlobals::PI=3.14159265;

const qreal BrainiacGlobals::MAXQREAL=std::numeric_limits<qreal>::max();
const qreal BrainiacGlobals::MINQREAL=-std::numeric_limits<qreal>::max();

float BrainiacGlobals::SegmentTesselationDetailRatio=0.25f;

const qreal BrainiacGlobals::ActionTriggerValue=1.0;
const qreal BrainiacGlobals::ActionForceTriggerValue=2.0;

const char* const BrainiacGlobals::XmlColorAttrib="color\0";
const char* const BrainiacGlobals::XmlEnabledAttrib="enabled\0";
const char* const BrainiacGlobals::XmlEndFrameAttrib="endFrame\0";
const char* const BrainiacGlobals::XmlModeAttrib="mode\0";
const char* const BrainiacGlobals::XmlNameAttrib="name\0";
const char* const BrainiacGlobals::XmlXAttrib="x\0";
const char* const BrainiacGlobals::XmlYAttrib="y\0";
const char* const BrainiacGlobals::XmlZAttrib="z\0";
const char* const BrainiacGlobals::XmlAngleVarAttrib="anglevariation\0";
const char* const BrainiacGlobals::XmlAngleAttrib="angle\0";
const char* const BrainiacGlobals::XmlDistanceAttrib="distance\0";
const char* const BrainiacGlobals::XmlGroupsAttrib="groups\0";
const char* const BrainiacGlobals::XmlGroupRatioAttrib="groupratio\0";
const char* const BrainiacGlobals::XmlInputAttrib="input\0";
const char* const BrainiacGlobals::XmlFileNameAttrib="fileName\0";
const char* const BrainiacGlobals::XmlFpsAttrib="fps\0";
const char* const BrainiacGlobals::XmlNumberAttrib="number\0";
const char* const BrainiacGlobals::XmlNoiseAttrib="noise\0";
const char* const BrainiacGlobals::XmlNoiseFreqAttrib="noisefreq\0";
const char* const BrainiacGlobals::XmlOutputAttrib="output\0";
const char* const BrainiacGlobals::XmlHeightVarAttrib="heightvariation\0";
const char* const BrainiacGlobals::XmlHeightAttrib="height\0";
const char* const BrainiacGlobals::XmlIdAttrib="id\0";
const char* const BrainiacGlobals::XmlRadiusAttrib="radius\0";
const char* const BrainiacGlobals::XmlEditorXPosAttrib="editorX\0";
const char* const BrainiacGlobals::XmlEditorYPosAttrib="editorY\0";
const char* const BrainiacGlobals::XmlStartFrameAttrib="startFrame\0";
const char* const BrainiacGlobals::XmlTreeDefaultActionAttrib="defaultaction\0";
const char* const BrainiacGlobals::XmlTreeActionAttrib="action\0";
const char* const BrainiacGlobals::XmlTreeTransitionAttrib="transition\0";
const char* const BrainiacGlobals::XmlTriggersAttrib="triggers\0";

const char* const BrainiacGlobals::XmlDirectoryTag="Directory";
const char* const BrainiacGlobals::XmlGroupsTag="Groups\0";
const char* const BrainiacGlobals::XmlInputTag="Input\0";
const char* const BrainiacGlobals::XmlOutputTag="Output\0";
const char* const BrainiacGlobals::XmlSizeTag="Size\0";
const char* const BrainiacGlobals::XmlTranslationTag="Translation\0";
const char* const BrainiacGlobals::XmlRotationTag="Rotation\0";
const char* const BrainiacGlobals::XmlRestRotationTag="RestRotation\0";
const char* const BrainiacGlobals::XmlRestTranslationTag="RestTranslation\0";
const char* const BrainiacGlobals::XmlSphereTag="Sphere\0";
const char* const BrainiacGlobals::XmlBoxTag="Box\0";
const char* const BrainiacGlobals::XmlTubeTag="Tube\0";
const char* const BrainiacGlobals::XmlRotTransOrderTag="RotTransOrder\0";
const char* const BrainiacGlobals::XmlMotionTreesTag="MotionTrees\0";
const char* const BrainiacGlobals::XmlMotionTreeTag="MotionTree\0";
const char* const BrainiacGlobals::XmlPlaceTag="Place\0";
const char* const BrainiacGlobals::XmlSceneTag="Scene\0";
const char* const BrainiacGlobals::XmlSimulationTag="Simulation\0";
const char* const BrainiacGlobals::XmlSimulationDirectoryOutputTag="SimOutDir\0";


const char* const BrainiacGlobals::XmlTreeActionTransitionConnectionTag="ActionTransitionConnection\0";
const char* const BrainiacGlobals::XmlTreeTransitionActionConnectionTag="TransitionActionConnection\0";
const char* const BrainiacGlobals::XmlTreeActionTag="Action\0";
const char* const BrainiacGlobals::XmlTreeTransitionTag="Transition\0";
const char* const BrainiacGlobals::XmlTriggerTag="Trigger\0";

const QList<QString> BrainiacGlobals::ChannelNames_Phases=(QList<QString>()) << QString("phase") << QString("phase2") << QString("phase3") << QString("phase4") <<
                                                                                QString("phase5") << QString("phase6") << QString("phase7") << QString("phase8");

const QList<QString> BrainiacGlobals::ChannelNames_Latches=(QList<QString>()) << QString("latch") << QString("latch2") << QString("latch3") << QString("latch4") <<
                                                                                QString("latch5") << QString("latch6") << QString("latch7") << QString("latch8");

const QList<QColor> BrainiacGlobals::EditorColors=(QList<QColor>()) << QColor(Qt::darkBlue) << QColor(Qt::darkCyan) << QColor(Qt::darkGray) << QColor(Qt::darkGreen);// << QColor;// QColor(Qt::darkBlue));// << QColor(Qt::darkCyan);

const QList<BrainiacGlobals::RotTrans> BrainiacGlobals::AgentDefaultRotTrans=(QList<BrainiacGlobals::RotTrans>()) << BrainiacGlobals::RX << BrainiacGlobals::RY <<  BrainiacGlobals::RZ << BrainiacGlobals::TZ << BrainiacGlobals::TY << BrainiacGlobals::TX;

const char* const BrainiacGlobals::DefaultLatchName="Latch\0";

const char* const BrainiacGlobals::FuzzAndModeMin="min\0";
const char* const BrainiacGlobals::FuzzAndModeProduct="prod\0";

const char* const BrainiacGlobals::FuzzFuzzModeTrapezoid="trapezoid\0";
const char* const BrainiacGlobals::FuzzFuzzInterpolationSine="sine\0";

const char* const BrainiacGlobals::FuzzOrModeMax="max\0";
const char* const BrainiacGlobals::FuzzOrModeSum="sum\0";

const char* const BrainiacGlobals::FuzzTimerModeIfStopped="ifstopped\0";
const char* const BrainiacGlobals::FuzzTimerModeAlways="always\0";

const QString BrainiacGlobals::ChannelName_Sound_x="sound.x\0";
const QString BrainiacGlobals::ChannelName_Sound_ox="sound.ox\0";
const QString BrainiacGlobals::ChannelName_Sound_d="sound.d\0";
const QString BrainiacGlobals::ChannelName_Sound_a="sound.a\0";
const QString BrainiacGlobals::ChannelName_Sound_f="sound.f\0";
const QString BrainiacGlobals::ChannelPhaseOffsetSuffix=":phase_offset\0";
const QString BrainiacGlobals::ChannelRunningSuffix=":running\0";

const QString BrainiacGlobals::DropTextPrefix="AddLogicType";

Qt::Key BrainiacGlobals::ToggleSegmentCoordCrossesKey=Qt::Key_C;
Qt::Key BrainiacGlobals::ToggleAgentPositionMarkerKey=Qt::Key_A;
Qt::Key BrainiacGlobals::ToggleAgentSoundEmissionKey=Qt::Key_E;
Qt::Key BrainiacGlobals::ToggleFollowAgentKey=Qt::Key_F;
Qt::Key BrainiacGlobals::ToggleSimulationKey=Qt::Key_Space;
Qt::Key BrainiacGlobals::AnimationPauseKey=Qt::Key_Down;
Qt::Key BrainiacGlobals::AnimationOneFrameBackwardKey=Qt::Key_Right;
Qt::Key BrainiacGlobals::AnimationOneFrameForwardKey=Qt::Key_Left;
#ifdef __APPLE__
    Qt::Modifier BrainiacGlobals::ModifierKey=Qt::META;  //! \bug alt key does not work under os x?
#else
    Qt::Modifier BrainiacGlobals::ModifierKey=Qt::ALT;
#endif
QKeySequence BrainiacGlobals::KeySequenceToggleAgentAxis=QKeySequence(BrainiacGlobals::ModifierKey + BrainiacGlobals::ToggleAgentPositionMarkerKey);
QKeySequence BrainiacGlobals::KeySequenceToggleAgentSoundEmission=QKeySequence(BrainiacGlobals::ModifierKey + BrainiacGlobals::ToggleAgentSoundEmissionKey);
QKeySequence BrainiacGlobals::KeySequenceToggleSimulationStartStop=QKeySequence(BrainiacGlobals::ModifierKey + BrainiacGlobals::ToggleSimulationKey);

QColor BrainiacGlobals::defaultXColor=QColor(200,0,0);
QColor BrainiacGlobals::defaultYColor=QColor(0,200,0);
QColor BrainiacGlobals::defaultZColor=QColor(200,200,0);
QColor BrainiacGlobals::defaultDiameterColor=QColor(50,50,200);
QColor BrainiacGlobals::defaultLengthColor=QColor(10,10,250);
QColor BrainiacGlobals::defaultRadiusColor=QColor(50,50,200);
QColor BrainiacGlobals::DefaultLatchColor=QColor(150,150,250);

osg::ref_ptr<CoordCrossDrawable> BrainiacGlobals::CoordCross=new CoordCrossDrawable;

osg::ref_ptr<osg::Node> BrainiacGlobals::loadOsgNodeFromQFile(QFile &file){
    QFileInfo info(file);
    osg::ref_ptr<osg::Node> loadedNode=0;
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data=file.readAll();
        osgDB::ReaderWriter* plugin=osgDB::Registry::instance()->getReaderWriterForExtension(info.completeSuffix().toUpper().toStdString());
        std::stringstream stream;
        stream.write(data.constData(), data.size());
        osgDB::ReaderWriter::ReadResult result=plugin->readNode(stream);
        loadedNode=result.getNode();
    }
    return loadedNode;
}
