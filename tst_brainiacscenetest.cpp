#include <QtCore/QString>
#include <QtTest/QtTest>

#include <QTemporaryFile>

#include "core/scene.h"
#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"

#include "core/agent/brain/brain.h"

#include "core/agent/body/bodymanager.h"

#include "core/agent/body/animation/animation.h"
#include "core/agent/body/animation/animationcurve.h"
#include "core/agent/body/animation/latchcurve.h"

#include "core/group/group.h"


#include "core/agent/brain/fuzzyand.h"
#include "core/agent/brain/input.h"
#include "core/agent/brain/fuzzyor.h"

Q_DECLARE_METATYPE(FuzzyAnd::Mode)
Q_DECLARE_METATYPE(FuzzyOr::Mode)
Q_DECLARE_METATYPE(BrainiacGlobals::AnimationType)

class BrainiacSceneTest : public QObject
{
    Q_OBJECT

public:
    BrainiacSceneTest();

private Q_SLOTS:
    /**
     * @brief Sets some stuff that is needed for testcases
     *
     *
     */
    void initTestCase();

    /**
     * @brief Tests, if scene loading/saving is valid
     *
     * creates a scene (Scene1), adds some groups. The scene will be saved, loaded again and compared to the initial setup
     */
    void sceneCreateLoadSave();

    void parseBVH();

    void animation_data();
    void animation();

    void fuzzyAnd_data();
    void fuzzyAnd();
    void fuzzyOr_data();
    void fuzzyOr();
    void cleanupTestCase();


private:
    QString testInput1Name;
    QString testInput2Name;
    QString testInput3Name;
    QString testAndName;
    QString testOrName;
    int m_numOfTestSegments;
    void createBody(AgentManager *am, int var);
    void createBrain(AgentManager *am);

};

BrainiacSceneTest::BrainiacSceneTest()
{
    //m_numberOfTestGroups=2;
}

void BrainiacSceneTest::initTestCase()
{
    testInput1Name=QString("TESTINPUT1");
    testInput2Name=QString("TESTINPUT2");
    testInput3Name=QString("TESTINPUT3");
    testAndName=QString("TESTAND");
    testOrName=QString("TESTOR");
    m_numOfTestSegments=0;
}

void BrainiacSceneTest::parseBVH()
{
    int numOfSegments=0;
    QString bvhFileName=":animation/walk-cycle.bvh";
    QFile file(bvhFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        QFAIL("unable to open bvh file! ");

    while (!file.atEnd()) {
        QByteArray line = file.readLine().trimmed();
        if(line.startsWith("ROOT") || line.startsWith("JOINT")) {
            numOfSegments++;
        }
    }
    file.close();
    Scene *testScene1=new Scene();
    Group *grp=new Group(testScene1);
    grp->setId(1);
    AgentManager *am=grp->getAgentManager();
    QVERIFY2(am->loadSkeleton(bvhFileName),"Error loading skeleton");
    QString errorTextWrongNumber=QString("Wrong number of segments. Expected:");
    errorTextWrongNumber.append(QString::number(numOfSegments).append(QString(", got: "))).append(QString::number(am->getBodyManager()->getSegments().count()));
    QVERIFY2(am->getBodyManager()->getSegments().count()==numOfSegments,errorTextWrongNumber.toStdString().c_str());

    bool foundNonZeroTranslation=false;
    foreach(Segment* seg, am->getBodyManager()->getSegments()) {
        if(qFuzzyCompare(seg->getRestTranslation(),QVector3D())) {
            foundNonZeroTranslation=true;
        }
    }
    QVERIFY2(foundNonZeroTranslation==true,"BVH only has zero length segments!");

}

void BrainiacSceneTest::animation_data()
{
    QTest::addColumn<QString>("animationName");

    QTest::addColumn<QString>("curve1Name");
    QTest::addColumn<QVector2D>("c1kf1");
    QTest::addColumn<QVector2D>("c1kf2");
    QTest::addColumn<QVector2D>("c1kf3");
    QTest::addColumn<QVector2D>("c1kf4");
    QTest::addColumn<QVector2D>("c1kf5");

    QTest::addColumn<QString>("curve2Name");
    QTest::addColumn<QVector2D>("c2kf1");
    QTest::addColumn<QVector2D>("c2kf2");
    QTest::addColumn<QVector2D>("c2kf3");
    QTest::addColumn<QVector2D>("c2kf4");

    QTest::addColumn<qreal>("maxTime");

    QTest::addColumn<BrainiacGlobals::AnimationType>("animType");
    QTest::addColumn<bool>("isRetriggerable");
    QTest::addColumn<bool>("isLooped");

    QTest::addColumn<QString>("latchCurve1");
    QTest::addColumn<QVector2D>("l1_1");
    QTest::addColumn<QVector2D>("l1_2");
    QTest::addColumn<QVector2D>("l1_3");

    QTest::addColumn<QString>("latchCurve2");
    QTest::addColumn<QVector2D>("l2_1");
    QTest::addColumn<QVector2D>("l2_2");
    QTest::addColumn<QVector2D>("l2_3");

    QTest::newRow("Case1") << "anim1"\
                           << "curve1" << QVector2D(0.0,0.3) << QVector2D(0.1,0.3) << QVector2D(0.2,-50.3) << QVector2D(0.4,0.3) << QVector2D(1.0,0.3)\
                           << "curve2" << QVector2D(0.0,0.1) << QVector2D(0.12,0.2) << QVector2D(0.21,-50.3) << QVector2D(10.0,0.4)\
                           << 10.0\
                           << BrainiacGlobals::STATIC  << false << true\
                           << "latch1" << QVector2D(0.0,0.1) << QVector2D(0.2,0.1) << QVector2D(0.4,0.1)\
                           << "latch2" << QVector2D(0.0,0.11) << QVector2D(0.2,0.12) << QVector2D(0.4,0.13);
    QTest::newRow("Case2") << "aNimAtion"\
                           << "curve1" << QVector2D(0.0,0.3) << QVector2D(0.1,0.3) << QVector2D(0.25,-51.3) << QVector2D(0.4,0.3) << QVector2D(1.0,0.3)\
                           << "curve2" << QVector2D(0.0,0.1) << QVector2D(0.12,0.2) << QVector2D(0.21,-50.3) << QVector2D(10.5,0.4)\
                           << 10.5\
                           << BrainiacGlobals::LOCOMOTION << true << false\
                           << "latch1" << QVector2D(0.0,0.15) << QVector2D(0.2,0.15) << QVector2D(0.4,0.15)\
                           << "latch2" << QVector2D(0.0,0.111) << QVector2D(0.2,0.123) << QVector2D(0.4,0.134);


}

void BrainiacSceneTest::animation()
{
    QTemporaryFile animFile;

    QFETCH(QString, curve1Name);
    QFETCH(qreal, maxTime);
    AnimationCurve testCurve1;
    QFETCH(QVector2D, c1kf1);
    testCurve1.addKeyFrame(c1kf1);
    QFETCH(QVector2D, c1kf2);
    testCurve1.addKeyFrame(c1kf2);
    QFETCH(QVector2D, c1kf3);
    testCurve1.addKeyFrame(c1kf3);
    QFETCH(QVector2D, c1kf4);
    testCurve1.addKeyFrame(c1kf4);
    QFETCH(QVector2D, c1kf5);
    testCurve1.addKeyFrame(c1kf5);

    QFETCH(QString, curve2Name);
    AnimationCurve testCurve2;
    QFETCH(QVector2D, c2kf1);
    testCurve2.addKeyFrame(c2kf1);
    QFETCH(QVector2D, c2kf2);
    testCurve2.addKeyFrame(c2kf2);
    QFETCH(QVector2D, c2kf3);
    testCurve2.addKeyFrame(c2kf3);
    QFETCH(QVector2D, c2kf4);
    testCurve2.addKeyFrame(c2kf4);

    QFETCH(QString, animationName);
    Animation testAnim;
    testAnim.setName(animationName);
    testAnim.addCurve(curve1Name,testCurve1);
    testAnim.addCurve(curve2Name,testCurve2);

    QFETCH(BrainiacGlobals::AnimationType,animType);
    testAnim.setAnimationType(animType);

    QFETCH(bool,isRetriggerable);
    testAnim.setIsRetriggerable(isRetriggerable);

    QFETCH(bool,isLooped);
    testAnim.setIsLoopedAnimation(isLooped);

    QFETCH(QString,latchCurve1);
    testAnim.addLatchCurve(latchCurve1);
    QFETCH(QVector2D,l1_1);
    testAnim.addLatch(latchCurve1,l1_1);
    QFETCH(QVector2D,l1_2);
    testAnim.addLatch(latchCurve1,l1_2);
    QFETCH(QVector2D,l1_3);
    testAnim.addLatch(latchCurve1,l1_3);

    QFETCH(QString,latchCurve2);
    testAnim.addLatchCurve(latchCurve2);
    QFETCH(QVector2D,l2_1);
    testAnim.addLatch(latchCurve2,l2_1);
    QFETCH(QVector2D,l2_2);
    testAnim.addLatch(latchCurve2,l2_2);
    QFETCH(QVector2D,l2_3);
    testAnim.addLatch(latchCurve2,l2_3);

    if(!animFile.open()) {
        QFAIL("Failed creation of tempfile");
    }
    testAnim.setFileName(animFile.fileName());
    QVERIFY2(testAnim.saveAnimation(),"Error while saving animation");

    Animation* loadedAnimation;
    loadedAnimation=Animation::loadAnimation(animFile.fileName());

    QVERIFY2(qFuzzyCompare(testAnim.getLength(),maxTime),"Wrong length calculation!");

    QVERIFY2(loadedAnimation->name().compare(animationName)==0,"wrong animation name in loaded animation");

    QVERIFY2(loadedAnimation->animationType()==animType,"wrong animation type!");
    QVERIFY2(testAnim.animationType()==loadedAnimation->animationType(),"Loaded animation has different animytion type!");

    QVERIFY2(testAnim.isLoopedAnimation()==isLooped,"looped property differs!");
    QVERIFY2(testAnim.isLoopedAnimation()==loadedAnimation->isLoopedAnimation(),"looped property of loaded animation differs!");

    QVERIFY2(testAnim.isRetriggerable()==isRetriggerable,"is retriggerable differs");
    QVERIFY2(testAnim.isRetriggerable()==loadedAnimation->isRetriggerable(),"is retriggerable of loaded animation differs");

    bool checkKf12=qFuzzyCompare((qreal)loadedAnimation->getValue(curve1Name,c1kf2.x()),(qreal)c1kf2.y());
    QVERIFY2(checkKf12,"KF 2 in curve 1 differs");
    bool checkKf22=qFuzzyCompare((qreal)loadedAnimation->getValue(curve2Name,c2kf2.x()),(qreal)c2kf2.y());
    QVERIFY2(checkKf22,"KF 2 in curve 2 differs");

    if(testAnim.curves().count()!=loadedAnimation->curves().count()) {
        QFAIL("re-loaded animation has different curves count");
    }
    foreach( QString curveName , testAnim.curveNames()) {
        if(!testAnim.curves().contains(curveName)) {
            QFAIL("Curve name in orig animation not found!");
        }
        if(!loadedAnimation->curves().contains(curveName)) {
            QFAIL("Curve name in loaded animation not found!");
        }
        AnimationCurve *origCurve=testAnim.curves().value(curveName);
        AnimationCurve *loadedCurve=loadedAnimation->curves().value(curveName);
        if(origCurve->keyFrames().count()!=loadedCurve->keyFrames().count()) {
            QFAIL("Number of keyframes differ!");
        }
        foreach(QVector2D kf,origCurve->keyFrames()) {
            QVERIFY2(loadedCurve->getValue(kf.x())==kf.y(),"Keyframes differ");
        }
    }
    //Latches
    QVERIFY2(testAnim.latches().count()>0,"No latches found!");
    QVERIFY2(testAnim.latches().value(latchCurve1)->latches().at(0)==l1_1,"Latchcurve 1 latch 1 differs");
    QCOMPARE(testAnim.latches().count(),loadedAnimation->latches().count());
    QVERIFY2(loadedAnimation->latches().value(latchCurve1)->latches().at(0)==l1_1,"Latchcurve 1 latch 1 of loaded animation differs");
    QVERIFY2(loadedAnimation->latches().value(latchCurve2)->latches().at(0)==l2_1,"Latchcurve 1 latch 1 of loaded animation differs");

}

void BrainiacSceneTest::fuzzyAnd_data()
{
    QTest::addColumn<qreal>("in1");
    QTest::addColumn<qreal>("in2");
    QTest::addColumn<qreal>("in3");
    QTest::addColumn<FuzzyAnd::Mode>("mode");
    QTest::addColumn<qreal>("result");

    QTest::newRow("all low Mode MIN") << 0.0 << 0.0 << 0.0 << FuzzyAnd::MIN << 0.0;
    QTest::newRow("all high Mode MIN")<< 1.0 << 1.0 << 1.0 << FuzzyAnd::MIN << 1.0;
    QTest::newRow("mix Mode MIN") << 0.1 << 0.3 << 0.2 << FuzzyAnd::MIN << 0.1;

    QTest::newRow("all low Mode PROD") << 0.0 << 0.0 << 0.0 << FuzzyAnd::PRODUCT << 0.0;
    QTest::newRow("all high Mode PROD")<< 1.0 << 1.0 << 1.0 << FuzzyAnd::PRODUCT << 1.0;
    QTest::newRow("mix Mode PROD") << 0.1 << 0.3 << 0.2 << FuzzyAnd::PRODUCT << 0.1*0.2*0.3;
}

void BrainiacSceneTest::fuzzyAnd()
{
    Scene *testScene1=new Scene();
    Group *grp=new Group(testScene1);
    grp->setId(1);
    createBody(grp->getAgentManager(),1);
    createBrain(grp->getAgentManager());

    QVERIFY(testScene1->getAgents().count()==0);
    QVERIFY2(testScene1->getGroups().count()>0,"No group in scene!");
//    Group *grp=m_scene2Groups.begin().value();
    AgentManager *am=grp->getAgentManager();
    FuzzyBase *fuzz=am->getMasterAgent()->getBrain()->getFuzzyByName(testAndName);
    FuzzyAnd *testAnd=dynamic_cast<FuzzyAnd *>(fuzz);
    if(!testAnd) {
        QFAIL("No test AND found!");
    }
    FuzzyBase *fuzz1=am->getMasterAgent()->getBrain()->getFuzzyByName(testInput1Name);
    FuzzyBase *fuzz2=am->getMasterAgent()->getBrain()->getFuzzyByName(testInput2Name);
    FuzzyBase *fuzz3=am->getMasterAgent()->getBrain()->getFuzzyByName(testInput3Name);


    Input *input1=dynamic_cast<Input *>(fuzz1);
    Input *input2=dynamic_cast<Input *>(fuzz2);
    Input *input3=dynamic_cast<Input *>(fuzz3);

    if(!(input1&&input2&&input3)) {
        QFAIL("Parents of AND are not Inputs!");
    }

    QCOMPARE(testAnd->getParents().count(),3);

    QFETCH(qreal, in1);
    QFETCH(qreal, in2);
    QFETCH(qreal, in3);
    QFETCH(FuzzyAnd::Mode, mode);
    QFETCH(qreal, result);

    testAnd->setMode(mode);
    input1->setResult(in1);
    input2->setResult(in2);
    input3->setResult(in3);

    QCOMPARE(testAnd->getResult(), result);
}

void BrainiacSceneTest::fuzzyOr_data()
{
    QTest::addColumn<qreal>("in1");
    QTest::addColumn<qreal>("in2");
    QTest::addColumn<qreal>("in3");
    QTest::addColumn<FuzzyOr::Mode>("mode");
    QTest::addColumn<qreal>("result");

    QTest::newRow("all low Mode MAX") << 0.0 << 0.0 << 0.0 << FuzzyOr::MAX << 0.0;
    QTest::newRow("all high Mode MAX")<< 1.0 << 1.0 << 1.0 << FuzzyOr::MAX << 1.0;
    QTest::newRow("one high Mode MAX")<< 0.0 << 1.0 << 0.0 << FuzzyOr::MAX << 1.0;
    QTest::newRow("Mode MAX check upper bound")<< -1.0 << 1.5 << 3.0 << FuzzyOr::MAX << 1.0;
    QTest::newRow("Mode MAX check lower bound")<< -1.0 << 0.0 << 0.0 << FuzzyOr::MAX << 0.0;

    QTest::newRow("mix Mode MAX") << 0.1 << 0.3 << 0.2 << FuzzyOr::MAX << 0.3;

    QTest::newRow("all low Mode SUM") << 0.0 << 0.0 << 0.0 << FuzzyOr::SUM << 0.0;
    QTest::newRow("all high Mode SUM")<< 1.0 << 1.0 << 1.0 << FuzzyOr::SUM << 1.0;
    QTest::newRow("one high Mode SUM")<< 0.0 << 1.0 << 0.0 << FuzzyOr::SUM << 1.0;
    QTest::newRow("high Mode SUM check upper bound")<< 1.0 << 1.5 << 1.0 << FuzzyOr::SUM << 1.0;
    QTest::newRow("high Mode SUM check lower bound")<< -1.0 << 0.0<< 0.0 << FuzzyOr::SUM << 0.0;


    QTest::newRow("mix Mode SUM") << 0.1 << 0.3 << 0.2 << FuzzyOr::SUM << 0.1+0.3+0.2;
}


void BrainiacSceneTest::fuzzyOr()
{
    Scene *testScene1=new Scene();
    Group *grp=new Group(testScene1);
    grp->setId(1);
    createBody(grp->getAgentManager(),1);
    createBrain(grp->getAgentManager());

    QVERIFY(testScene1->getAgents().count()==0);
    QVERIFY2(testScene1->getGroups().count()>0,"No group in scene!");
//    Group *grp=m_scene2Groups.begin().value();
    AgentManager *am=grp->getAgentManager();
    FuzzyBase *fuzz=am->getMasterAgent()->getBrain()->getFuzzyByName(testOrName);
    FuzzyOr *testOr=dynamic_cast<FuzzyOr *>(fuzz);
    if(!testOr) {
        QFAIL("No test OR found!");
    }
    FuzzyBase *fuzz1=am->getMasterAgent()->getBrain()->getFuzzyByName(testInput1Name);
    FuzzyBase *fuzz2=am->getMasterAgent()->getBrain()->getFuzzyByName(testInput2Name);
    FuzzyBase *fuzz3=am->getMasterAgent()->getBrain()->getFuzzyByName(testInput3Name);

    Input *input1=dynamic_cast<Input *>(fuzz1);
    Input *input2=dynamic_cast<Input *>(fuzz2);
    Input *input3=dynamic_cast<Input *>(fuzz3);

    if(!(input1&&input2&&input3)) {
        QFAIL("Parents of OR are not Inputs!");
    }

    QCOMPARE(testOr->getParents().count(),3);

    QFETCH(qreal, in1);
    QFETCH(qreal, in2);
    QFETCH(qreal, in3);
    QFETCH(FuzzyOr::Mode, mode);
    QFETCH(qreal, result);

    testOr->setMode(mode);
    input1->setResult(in1);
    input2->setResult(in2);
    input3->setResult(in3);

    QCOMPARE(testOr->getResult(), result);
    testScene1->deleteLater();

}

void BrainiacSceneTest::cleanupTestCase()
{

}

void BrainiacSceneTest::sceneCreateLoadSave()
{
    Scene *testScene1;
    QTemporaryFile scene1File;

    QHash <quint32, Group *> scene1Groups;
    QHash <quint32,QTemporaryFile *> scene1GroupFiles;

    QHash <quint32, Group *> scene2Groups;
    QHash <quint32,QTemporaryFile *> scene2GroupFiles;

    Scene *testScene2;
    //QTemporaryFile m_scene2File;

    static const int numberOfTestGroups=5;
//

//    quint32 m_testAndId;
//    quint32 m_testOrId;
//    quint32 m_testInput1Id;
//    quint32 m_testInput2Id;
//    quint32 m_testInput3Id;
//
    testScene1=new Scene();
    for(int id=1;id<=numberOfTestGroups;id++) {
        Group *grp=new Group(testScene1);
        QString grpName=QString("Group")%QString::number(id);
        grp->setId(id);
        grp->setName(grpName);
        scene1Groups.insert(id,grp);
        createBody(grp->getAgentManager(),id);
        createBrain(grp->getAgentManager());
    }

    QVERIFY(testScene1->getAgents().count()==0);
    QVERIFY(testScene1->getGroups().count()==numberOfTestGroups);

    // Save scene 1 agents
    QString sceneFileNameTemplate=QDir::toNativeSeparators(QDir::tempPath()%"/BrainiacTestScene1XXXXXX.xml");
    QVERIFY2(scene1File.open(),"Temp file for scene 1 could not be opened");

    scene1File.setFileTemplate(sceneFileNameTemplate);

    for( int id=1; id <= numberOfTestGroups; id++) {
        QString agentFilename=QString("/BrainiacTestAgent")%QString::number(id)%QString("Scene1XXXXXX.xml");
        QString agentFileNameTemplate=QDir::toNativeSeparators(QDir::tempPath()%agentFilename);
        QTemporaryFile *tmpFile=new QTemporaryFile(agentFileNameTemplate);
        if(!tmpFile->open()) {
            QFAIL("Failed creation of tempfile");
        }
        scene1GroupFiles.insert(id,tmpFile);
        Group *grp=scene1Groups.value(id);
        grp->setAgentFileName(tmpFile->fileName());
        grp->getAgentManager()->setFileName(tmpFile->fileName());
        QVERIFY2(grp->getAgentManager()->saveConfig(),"Error saving agent");
    }

    // Save scene 1
    QVERIFY2(testScene1->saveConfig(scene1File.fileName()),"Error saving Scene 1");

    // Load scene 2
    testScene2=new Scene();
    QVERIFY2(testScene2->openConfig(testScene1->getFileName()),"Error loading scene 2 from scene 1 file");
    QVERIFY2(testScene2->getGroups().count()==numberOfTestGroups,"Number of groups in scene 2 is not equal");

    // Save agents of scene 2
    for( int id=1; id <= numberOfTestGroups; id++) {
        QString agentFilename=QString("/BrainiacTestAgent")%QString::number(id)%QString("Scene2XXXXXX.xml");
        QString agentFileNameTemplate=QDir::toNativeSeparators(QDir::tempPath()%agentFilename);
        QTemporaryFile *tmpFile=new QTemporaryFile(agentFileNameTemplate);
        if(!tmpFile->open()) {
            QFAIL("Failed creation of tempfile");
        }
        scene2GroupFiles.insert(id,tmpFile);
        Group *grp=scene1Groups.value(id);
        grp->setAgentFileName(tmpFile->fileName());
        grp->getAgentManager()->setFileName(tmpFile->fileName());
        QVERIFY2(grp->getAgentManager()->saveConfig(),"Error saving agent");
    }


    for( int id=1; id <= numberOfTestGroups; id++) {
        QStringList args;
        args << scene1GroupFiles.value(id)->fileName();
        args << scene2GroupFiles.value(id)->fileName();

        QProcess process;
        process.start("diff",args);

        process.waitForFinished(-1);
        if(process.exitCode()) {
            QByteArray stdOut=process.readAllStandardOutput();
            QByteArray stdErr=process.readAllStandardError();
            QString errorMessage=stdOut % stdErr % QString(" agent differs:") % QString::number(id);
            QFAIL(errorMessage.toStdString().c_str());
        } else {
            QVERIFY(true);
        }
    }

    //Compare scene 1 and scene 2

    QHashIterator<quint32, Group *> i(scene1Groups);
    while (i.hasNext()) {
        i.next();
        QHash<quint32, SegmentShape *> agent1Segs=i.value()->getAgentManager()->getBodyManager()->getSegments();
        Group *scene2grp=testScene2->getGroup(i.key());
        scene2Groups.insert(scene2grp->getId(),scene2grp);
        QHash<quint32, SegmentShape *> agent2Segs=scene2grp->getAgentManager()->getBodyManager()->getSegments();
        QVERIFY2(agent1Segs.count()==agent2Segs.count(),"Number of segments differs");
        QVERIFY2(agent1Segs.count()==m_numOfTestSegments,"Agent has wrong number of segments");
        foreach(SegmentShape *seg,i.value()->getAgentManager()->getBodyManager()->getSegments()) {
            SegmentShape *seg2=agent2Segs.value(seg->getId());
            QVERIFY2(seg->getRestRotation()==seg2->getRestRotation(),"RestRotation differs");
            QVERIFY2(seg->getRotation()==seg2->getRotation(),"Rotation differs");
            QVERIFY2(seg->getRestTranslation()==seg2->getRestTranslation(),"RestTranslation differs");
            QVERIFY2(seg->getTranslation()==seg2->getTranslation(),"Translation differs");
        }
    }
    foreach(QTemporaryFile *file,scene1GroupFiles) {
        QVERIFY2(file->remove(),"Scene 1 files could not be removed");
        file->deleteLater();
    }
    scene1GroupFiles.clear();

    foreach(QTemporaryFile *file,scene2GroupFiles) {
        QVERIFY2(file->remove(),"Scene 2 files could not be removed");
        file->deleteLater();
    }

}

void BrainiacSceneTest::createBody(AgentManager *am, int var)
{
    QVector3D testVec=QVector3D(1,2,3);
    QVector3D vec;
    QString segName="Seg1";
    BodyManager *bm=am->getBodyManager();

    am->setBodyEditorTranslation(1,10*var,5*var);
    bm->setNewSegmentId(1);
    bm->setNewSegmentName(segName);
    bm->setNewSegmentParentId(0);
    bm->setNewSegmentType(BrainiacGlobals::SPHERESEGMENT);
    bm->setNewSegmentColor(0.1f);
    vec=testVec*(float) var;
    bm->setNewSegmentRestRotation(vec);
    vec=testVec*(float) var;
    bm->setNewSegmentRestTranslation(vec);
    bm->createNewSegment();

    segName="Seg2";
    am->setBodyEditorTranslation(2,11*var,6*var);
    bm->setNewSegmentId(2);
    bm->setNewSegmentName(segName);
    bm->setNewSegmentParentId(1);
    bm->setNewSegmentType(BrainiacGlobals::SPHERESEGMENT);
    bm->setNewSegmentColor(0.2f);
    vec=testVec*(float) var;
    bm->setNewSegmentRestRotation(vec);
    vec=testVec*(float) var;
    bm->setNewSegmentRestTranslation(vec);
    bm->createNewSegment();

    segName="Seg3";
    am->setBodyEditorTranslation(3,12*var,7*var);
    bm->setNewSegmentId(3);
    bm->setNewSegmentName(segName);
    bm->setNewSegmentParentId(1);
    bm->setNewSegmentType(BrainiacGlobals::SPHERESEGMENT);
    bm->setNewSegmentColor(0.3f);
    vec=testVec*(float) var;
    bm->setNewSegmentRestRotation(vec);
    vec=testVec*(float) var;
    bm->setNewSegmentRestTranslation(vec);
    bm->createNewSegment();

    segName="Seg4";
    am->setBodyEditorTranslation(4,14*var,8*var);
    bm->setNewSegmentId(4);
    bm->setNewSegmentName(segName);
    bm->setNewSegmentParentId(2);
    bm->setNewSegmentType(BrainiacGlobals::SPHERESEGMENT);
    bm->setNewSegmentColor(0.4f);
    vec=testVec*(float) var;
    bm->setNewSegmentRestRotation(vec);
    vec=testVec*(float) var;
    bm->setNewSegmentRestTranslation(vec);
    bm->createNewSegment();
    m_numOfTestSegments=4;
}

void BrainiacSceneTest::createBrain(AgentManager *am)
{
    quint32 testAndId,testOrId,testInput1Id,testInput2Id,testInput3Id;
    testAndId=am->addAndFuzz(50,50);
    am->setFuzzyName(testAndId,testAndName);

    testOrId=am->addOrFuzz(55,55);
    am->setFuzzyName(testOrId,testOrName);

    testInput1Id=am->addInputFuzz(100,100);
    am->setFuzzyName(testInput1Id,testInput1Name);
    testInput2Id=am->addInputFuzz(200,300);
    am->setFuzzyName(testInput2Id,testInput2Name);
    testInput3Id=am->addInputFuzz(300,300);
    am->setFuzzyName(testInput3Id,testInput3Name);

    am->addConnector(testAndId,testInput1Id,false);
    am->addConnector(testAndId,testInput2Id,false);
    am->addConnector(testAndId,testInput3Id,false);

    am->addConnector(testOrId,testInput1Id,false);
    am->addConnector(testOrId,testInput2Id,false);
    am->addConnector(testOrId,testInput3Id,false);
}

QTEST_MAIN(BrainiacSceneTest)

#include "tst_brainiacscenetest.moc"
