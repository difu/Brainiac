#include <QtCore/QString>
#include <QtTest/QtTest>

#include <QTemporaryFile>

#include "core/scene.h"
#include "core/agent/agentmanager.h"
#include "core/agent/body/bodymanager.h"
#include "core/group/group.h"
#include "core/agent/agent.h"
#include "core/agent/brain/brain.h"

#include "core/agent/brain/fuzzyand.h"
#include "core/agent/brain/input.h"
#include "core/agent/brain/fuzzyor.h"

    Q_DECLARE_METATYPE(FuzzyAnd::Mode)

class BrainiacSceneTest : public QObject
{
    Q_OBJECT

public:
    BrainiacSceneTest();

private Q_SLOTS:
    void initTestCase();

    /**
     * @brief Tests, if scene creation is valid
     *
     * creates a scene (Scene1), adds two groups.
     */
    void createScene1();
    void saveScene1Agents();
    void saveScene1();
    void loadScene2fromScene1();
    void saveScene2Agents();
    void checkAgentProperties();
    void compareScene1Scene2Agents();
    void testAnd_data();
    void testAnd();
    void testOr();
    void cleanupTestCase();
    void sceneTestCase1();

private:
    void createBody(AgentManager *am, int var);
    void createBrain(AgentManager *am);
    Scene *m_testScene1;
    QTemporaryFile m_scene1File;

    QHash <quint32, Group *> m_scene1Groups;
    QHash <quint32,QTemporaryFile *> m_scene1GroupFiles;

    QHash <quint32, Group *> m_scene2Groups;
    QHash <quint32,QTemporaryFile *> m_scene2GroupFiles;

    Scene *m_testScene2;
    QTemporaryFile m_scene2File;

    static const int m_numberOfTestGroups=5;
    QVector3D m_testVec;

    quint32 m_testAndId;
    quint32 m_testOrId;
    quint32 m_testInput1Id;
    quint32 m_testInput2Id;
    quint32 m_testInput3Id;
};

BrainiacSceneTest::BrainiacSceneTest()
{
    //m_numberOfTestGroups=2;
}

void BrainiacSceneTest::initTestCase()
{
    m_testVec=QVector3D(1.0f,2.0f,3.0f);
}

void BrainiacSceneTest::createScene1()
{
    m_testScene1=new Scene();
    for(int id=1;id<=m_numberOfTestGroups;id++) {
        Group *grp=new Group(m_testScene1);
        QString grpName=QString("Group")%QString::number(id);
        grp->setId(id);
        grp->setName(grpName);
        m_scene1Groups.insert(id,grp);
        createBody(grp->getAgentManager(),id);
        createBrain(grp->getAgentManager());
    }

    QVERIFY(m_testScene1->getAgents().count()==0);
    QVERIFY(m_testScene1->getGroups().count()==m_numberOfTestGroups);
}

void BrainiacSceneTest::saveScene1Agents()
{

    QString sceneFileNameTemplate=QDir::toNativeSeparators(QDir::tempPath()%"/BrainiacTestScene1XXXXXX.xml");
    QVERIFY2(m_scene1File.open(),"Temp file for scene 1 could not be opened");

    m_scene1File.setFileTemplate(sceneFileNameTemplate);

    for( int id=1; id <= m_numberOfTestGroups; id++) {
        QString agentFilename=QString("/BrainiacTestAgent")%QString::number(id)%QString("Scene1XXXXXX.xml");
        QString agentFileNameTemplate=QDir::toNativeSeparators(QDir::tempPath()%agentFilename);
        QTemporaryFile *tmpFile=new QTemporaryFile(agentFileNameTemplate);
        if(!tmpFile->open()) {
            QFAIL("Failed creation of tempfile");
        }
        m_scene1GroupFiles.insert(id,tmpFile);
        Group *grp=m_scene1Groups.value(id);
        grp->setAgentFileName(tmpFile->fileName());
        grp->getAgentManager()->setFileName(tmpFile->fileName());
        QVERIFY2(grp->getAgentManager()->saveConfig(),"Error saving agent");
    }
}

void BrainiacSceneTest::saveScene1()
{
    QVERIFY2(m_testScene1->saveConfig(m_scene1File.fileName()),"Error saving Scene 1");
}

void BrainiacSceneTest::loadScene2fromScene1()
{
    m_testScene2=new Scene();
    QVERIFY2(m_testScene2->openConfig(m_testScene1->getFileName()),"Error loading scene 2 from scene 1 file");
    QVERIFY2(m_testScene2->getGroups().count()==m_numberOfTestGroups,"Number of groups is not equal");
}

void BrainiacSceneTest::saveScene2Agents()
{

    for( int id=1; id <= m_numberOfTestGroups; id++) {
        QString agentFilename=QString("/BrainiacTestAgent")%QString::number(id)%QString("Scene2XXXXXX.xml");
        QString agentFileNameTemplate=QDir::toNativeSeparators(QDir::tempPath()%agentFilename);
        QTemporaryFile *tmpFile=new QTemporaryFile(agentFileNameTemplate);
        if(!tmpFile->open()) {
            QFAIL("Failed creation of tempfile");
        }
        m_scene2GroupFiles.insert(id,tmpFile);
        Group *grp=m_scene1Groups.value(id);
        grp->setAgentFileName(tmpFile->fileName());
        grp->getAgentManager()->setFileName(tmpFile->fileName());
        QVERIFY2(grp->getAgentManager()->saveConfig(),"Error saving agent");
    }


    for( int id=1; id <= m_numberOfTestGroups; id++) {
        QStringList args;
        args << m_scene1GroupFiles.value(id)->fileName();
        args << m_scene2GroupFiles.value(id)->fileName();

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
}

void BrainiacSceneTest::checkAgentProperties()
{
    QHashIterator<quint32, Group *> i(m_scene1Groups);
    while (i.hasNext()) {
        i.next();
        Group *grp=i.value();
        AgentManager *mgr=grp->getAgentManager();
    }
}

void BrainiacSceneTest::compareScene1Scene2Agents()
{
    QHashIterator<quint32, Group *> i(m_scene1Groups);
    while (i.hasNext()) {
        i.next();
        QHash<quint32, SegmentShape *> agent1Segs=i.value()->getAgentManager()->getBodyManager()->getSegments();
        Group *scene2grp=m_testScene2->getGroup(i.key());
        m_scene2Groups.insert(scene2grp->getId(),scene2grp);
        QHash<quint32, SegmentShape *> agent2Segs=scene2grp->getAgentManager()->getBodyManager()->getSegments();
        QVERIFY2(agent1Segs.count()==agent2Segs.count(),"Number of segments differs");
        foreach(SegmentShape *seg,i.value()->getAgentManager()->getBodyManager()->getSegments()) {
            SegmentShape *seg2=agent2Segs.value(seg->getId());
            QVERIFY2(seg->getRestRotation()==seg2->getRestRotation(),"RestRotation differs");
            QVERIFY2(seg->getRotation()==seg2->getRotation(),"Rotation differs");
            QVERIFY2(seg->getRestTranslation()==seg2->getRestTranslation(),"RestTranslation differs");
            QVERIFY2(seg->getTranslation()==seg2->getTranslation(),"Translation differs");
        }
    }

}

void BrainiacSceneTest::testAnd_data()
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

void BrainiacSceneTest::testAnd()
{
    AgentManager *am=0;
    QVERIFY2(m_scene2Groups.count()>0,"No Scene 2 Groups!");
    Group *grp=m_scene2Groups.begin().value();
    am=grp->getAgentManager();
    FuzzyBase *fuzz=am->getMasterAgent()->getBrain()->getFuzzy(m_testAndId);
    FuzzyAnd *testAnd=dynamic_cast<FuzzyAnd *>(fuzz);
    if(!testAnd) {
        QFAIL("No test and found!");
    }
    FuzzyBase *fuzz1=am->getMasterAgent()->getBrain()->getFuzzy(m_testInput1Id);
    FuzzyBase *fuzz2=am->getMasterAgent()->getBrain()->getFuzzy(m_testInput2Id);
    FuzzyBase *fuzz3=am->getMasterAgent()->getBrain()->getFuzzy(m_testInput3Id);

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

void BrainiacSceneTest::testOr()
{
    AgentManager *am=0;
    QVERIFY2(m_scene2Groups.count()>0,"No Scene 2 Groups!");
    Group *grp=m_scene2Groups.begin().value();
    am=grp->getAgentManager();
    FuzzyBase *fuzz=am->getMasterAgent()->getBrain()->getFuzzy(m_testOrId);
    FuzzyOr *testOr=dynamic_cast<FuzzyOr *>(fuzz);
    if(!testOr) {
        QFAIL("No test or found!");
    }
    FuzzyBase *fuzz1=am->getMasterAgent()->getBrain()->getFuzzy(m_testInput1Id);
    FuzzyBase *fuzz2=am->getMasterAgent()->getBrain()->getFuzzy(m_testInput2Id);
    FuzzyBase *fuzz3=am->getMasterAgent()->getBrain()->getFuzzy(m_testInput3Id);

    Input *input1=dynamic_cast<Input *>(fuzz1);
    Input *input2=dynamic_cast<Input *>(fuzz2);
    Input *input3=dynamic_cast<Input *>(fuzz3);

    if(!(input1&&input2&&input3)) {
        QFAIL("Parents of OR are not Inputs!");
    }

    QCOMPARE(testOr->getParents().count(),3);


}

void BrainiacSceneTest::cleanupTestCase()
{
    foreach(QTemporaryFile *file,m_scene1GroupFiles) {
        file->remove();
        file->deleteLater();
    }
    m_scene1GroupFiles.clear();

    foreach(QTemporaryFile *file,m_scene2GroupFiles) {
        file->remove();
        file->deleteLater();
    }
}

void BrainiacSceneTest::sceneTestCase1()
{
    QVERIFY2(true, "Failure");
}

void BrainiacSceneTest::createBody(AgentManager *am, int var)
{
    QVector3D vec;
    QString segName="Seg1";
    BodyManager *bm=am->getBodyManager();

    am->setBodyEditorTranslation(1,10*var,5*var);
    bm->setNewSegmentId(1);
    bm->setNewSegmentName(segName);
    bm->setNewSegmentParentId(0);
    bm->setNewSegmentType(BrainiacGlobals::SPHERESEGMENT);
    bm->setNewSegmentColor(0.1f);
    vec=m_testVec*(float) var;
    bm->setNewSegmentRestRotation(vec);
    vec=m_testVec*(float) var;
    bm->setNewSegmentRestTranslation(vec);
    bm->createNewSegment();

    segName="Seg2";
    am->setBodyEditorTranslation(2,11*var,6*var);
    bm->setNewSegmentId(2);
    bm->setNewSegmentName(segName);
    bm->setNewSegmentParentId(1);
    bm->setNewSegmentType(BrainiacGlobals::SPHERESEGMENT);
    bm->setNewSegmentColor(0.2f);
    vec=m_testVec*(float) var;
    bm->setNewSegmentRestRotation(vec);
    vec=m_testVec*(float) var;
    bm->setNewSegmentRestTranslation(vec);
    bm->createNewSegment();

    segName="Seg3";
    am->setBodyEditorTranslation(3,12*var,7*var);
    bm->setNewSegmentId(3);
    bm->setNewSegmentName(segName);
    bm->setNewSegmentParentId(1);
    bm->setNewSegmentType(BrainiacGlobals::SPHERESEGMENT);
    bm->setNewSegmentColor(0.3f);
    vec=m_testVec*(float) var;
    bm->setNewSegmentRestRotation(vec);
    vec=m_testVec*(float) var;
    bm->setNewSegmentRestTranslation(vec);
    bm->createNewSegment();

    segName="Seg4";
    am->setBodyEditorTranslation(4,14*var,8*var);
    bm->setNewSegmentId(4);
    bm->setNewSegmentName(segName);
    bm->setNewSegmentParentId(2);
    bm->setNewSegmentType(BrainiacGlobals::SPHERESEGMENT);
    bm->setNewSegmentColor(0.4f);
    vec=m_testVec*(float) var;
    bm->setNewSegmentRestRotation(vec);
    vec=m_testVec*(float) var;
    bm->setNewSegmentRestTranslation(vec);
    bm->createNewSegment();
}

void BrainiacSceneTest::createBrain(AgentManager *am)
{
    m_testAndId=am->addAndFuzz(50,50);
    am->setFuzzyName(m_testAndId,"AND");

    m_testOrId=am->addOrFuzz(55,55);
    am->setFuzzyName(m_testOrId,"OR");

    m_testInput1Id=am->addInputFuzz(100,100);
    am->setFuzzyName(m_testInput1Id,"TESTINPUT1");
    m_testInput2Id=am->addInputFuzz(200,300);
    am->setFuzzyName(m_testInput2Id,"TESTINPUT2");
    m_testInput3Id=am->addInputFuzz(300,300);
    am->setFuzzyName(m_testInput3Id,"TESTINPUT3");

    am->addConnector(m_testAndId,m_testInput1Id,false);
    am->addConnector(m_testAndId,m_testInput2Id,false);
    am->addConnector(m_testAndId,m_testInput3Id,false);

    am->addConnector(m_testOrId,m_testInput1Id,false);
    am->addConnector(m_testOrId,m_testInput2Id,false);
    am->addConnector(m_testOrId,m_testInput3Id,false);
}

QTEST_MAIN(BrainiacSceneTest)

#include "tst_brainiacscenetest.moc"
