#include <QtCore/QString>
#include <QtTest/QtTest>

#include <QTemporaryFile>

#include "core/scene.h"
#include "core/agent/agentmanager.h"
#include "core/agent/body/bodymanager.h"
#include "core/group/group.h"

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
    void compareScene1Scene2Agents();
    void cleanupTestCase();
    void sceneTestCase1();

private:
    void createBody(AgentManager *am);
    Scene *m_testScene1;
    QTemporaryFile m_scene1File;
    Group *m_group1Scene1;
    Group *m_group2Scene1;
    QTemporaryFile m_scene1Agent1File;
    QTemporaryFile m_scene1Agent2File;

    QTemporaryFile m_scene2Agent1File;
    QTemporaryFile m_scene2Agent2File;

    Scene *m_testScene2;
    QTemporaryFile m_scene2File;
    Group *m_group1Scene2;
    Group *m_group2Scene2;
    static const qint32 m_numberOfTestGroups=2;
};

BrainiacSceneTest::BrainiacSceneTest()
{
    //m_numberOfTestGroups=2;
}

void BrainiacSceneTest::initTestCase()
{

}

void BrainiacSceneTest::createScene1()
{
    m_testScene1=new Scene();
    m_group1Scene1=new Group(m_testScene1);
    m_group1Scene1->setId(1);
    m_group2Scene1=new Group(m_testScene1);
    m_group2Scene1->setId(2);
    QVERIFY(m_testScene1->getAgents().count()==0);
    QVERIFY(m_testScene1->getGroups().count()==m_numberOfTestGroups);
    createBody(m_group1Scene1->getAgentManager());
}

//void BrainiacSceneTest::blaTest()
//{
//    QVERIFY(false);
//}

void BrainiacSceneTest::saveScene1Agents()
{

    QString sceneFileNameTemplate=QDir::toNativeSeparators(QDir::tempPath()%"/BrainiacTestScene1XXXXXX.xml");
    QVERIFY2(m_scene1File.open(),"Temp file for scene 1 could not be opened");

    m_scene1File.setFileTemplate(sceneFileNameTemplate);

    QString agent1FileNameTemplate=QDir::toNativeSeparators(QDir::tempPath()%"/BrainiacTestAgent1Scene1XXXXXX.xml");
    QString agent2FileNameTemplate=QDir::toNativeSeparators(QDir::tempPath()%"/BrainiacTestAgent2Scene1XXXXXX.xml");

    m_scene1Agent1File.setFileTemplate(agent1FileNameTemplate);
    m_scene1Agent2File.setFileTemplate(agent2FileNameTemplate);




    QVERIFY2(m_scene1Agent1File.open(),"TempFile for agent1 could not be opened");
    QVERIFY2(m_scene1Agent2File.open(),"TempFile for agent2 could not be opened");
    m_group1Scene1->setAgentFileName(m_scene1Agent1File.fileName());
    m_group2Scene1->setAgentFileName(m_scene1Agent2File.fileName());

    m_group1Scene1->getAgentManager()->setFileName(m_scene1Agent1File.fileName());
    m_group2Scene1->getAgentManager()->setFileName(m_scene1Agent2File.fileName());

    //m_scene1Agent1File.close();

    QVERIFY2(m_group1Scene1->getAgentManager()->saveConfig(),"Error saving agent 1");
    QVERIFY2(m_group2Scene1->getAgentManager()->saveConfig(),"Error saving agent 2");
}

void BrainiacSceneTest::saveScene1()
{
//    QString sceneFileNameTemplate=QDir::toNativeSeparators(QDir::tempPath()%"/BrainiacTestScene1XXXXXX.xml");
//    QVERIFY2(m_scene1File.open(),"Temp file for scene 1 could not be opened");

//    m_scene1File.setFileTemplate(sceneFileNameTemplate);


    QVERIFY2(m_testScene1->saveConfig(m_scene1File.fileName()),"Error saving Scene 1");
}

void BrainiacSceneTest::loadScene2fromScene1()
{
    m_testScene2=new Scene();
//    qDebug() << "Scene 1 Filename" << m_testScene1->getFileName() << m_scene1File.fileName();
    QVERIFY2(m_testScene2->openConfig(m_testScene1->getFileName()),"Error loading scene 2 from scene 1 file");
    QVERIFY2(m_testScene2->getGroups().count()==m_numberOfTestGroups,"Number of groups is not equal");
}

void BrainiacSceneTest::saveScene2Agents()
{
    m_group1Scene2=m_testScene2->getGroup(1);
    m_group2Scene2=m_testScene2->getGroup(2);
//    QString sceneFileNameTemplate=QDir::toNativeSeparators(QDir::tempPath()%"/BrainiacTestScene1XXXXXX.xml");
//    QVERIFY2(m_scene1File.open(),"Temp file for scene 1 could not be opened");

//    m_scene1File.setFileTemplate(sceneFileNameTemplate);

    QString agent1FileNameTemplate=QDir::toNativeSeparators(QDir::tempPath()%"/BrainiacTestAgent1Scene2XXXXXX.xml");
    QString agent2FileNameTemplate=QDir::toNativeSeparators(QDir::tempPath()%"/BrainiacTestAgent2Scene2XXXXXX.xml");

    m_scene2Agent1File.setFileTemplate(agent1FileNameTemplate);
    m_scene2Agent2File.setFileTemplate(agent2FileNameTemplate);


    QVERIFY2(m_scene2Agent1File.open(),"TempFile for agent1 could not be opened");
    QVERIFY2(m_scene2Agent2File.open(),"TempFile for agent2 could not be opened");
    m_group1Scene2->setAgentFileName(m_scene1Agent1File.fileName());
    m_group2Scene2->setAgentFileName(m_scene1Agent2File.fileName());

    m_group1Scene2->getAgentManager()->setFileName(m_scene2Agent1File.fileName());
    m_group2Scene2->getAgentManager()->setFileName(m_scene2Agent2File.fileName());

    //m_scene1Agent1File.close();

    QVERIFY2(m_group1Scene2->getAgentManager()->saveConfig(),"Error saving agent 1");
    QVERIFY2(m_group2Scene2->getAgentManager()->saveConfig(),"Error saving agent 2");

    QStringList args;
    args << m_scene2Agent1File.fileName();
    args << m_scene1Agent1File.fileName();

    QProcess process;
    process.start("diff",args);

    process.waitForFinished(-1);
    if(process.exitCode()) {
        QByteArray stdOut=process.readAllStandardOutput();
        qDebug() << stdOut;
        QVERIFY2(false,"agent files differ!");
    } else {
        QVERIFY(true);
    }
}

void BrainiacSceneTest::compareScene1Scene2Agents()
{
    QHash<quint32, SegmentShape *> agent1Segs=m_group1Scene1->getAgentManager()->getBodyManager()->getSegments();
    QHash<quint32, SegmentShape *> agent2Segs=m_group1Scene2->getAgentManager()->getBodyManager()->getSegments();
    QVERIFY2(agent1Segs.count()==agent2Segs.count(),"Number of segments differs");
    foreach(SegmentShape *seg,m_group1Scene1->getAgentManager()->getBodyManager()->getSegments()) {
        SegmentShape *seg2=agent2Segs.value(seg->getId());
        QVERIFY2(seg->getRestRotation()==seg2->getRestRotation(),"RestRotation differs");
        QVERIFY2(seg->getRotation()==seg2->getRotation(),"Rotation differs");
        QVERIFY2(seg->getRestTranslation()==seg2->getRestTranslation(),"RestTranslation differs");
        QVERIFY2(seg->getTranslation()==seg2->getTranslation(),"Translation differs");
    }

}

void BrainiacSceneTest::cleanupTestCase()
{
    m_testScene1->getAgents().count();
}

void BrainiacSceneTest::sceneTestCase1()
{
    QVERIFY2(true, "Failure");
}

void BrainiacSceneTest::createBody(AgentManager *am)
{
    QVector3D vec;
    QString segName="Seg1";
    BodyManager *bm=am->getBodyManager();

    am->setBodyEditorTranslation(1,123,456);
    bm->setNewSegmentId(1);
    bm->setNewSegmentName(segName);
    bm->setNewSegmentParentId(0);
    bm->setNewSegmentType(BrainiacGlobals::SPHERESEGMENT);
    bm->createNewSegment();

    segName="Seg2";
    am->setBodyEditorTranslation(2,789,1012);
    bm->setNewSegmentId(2);
    bm->setNewSegmentName(segName);
    bm->setNewSegmentParentId(1);
    bm->setNewSegmentType(BrainiacGlobals::SPHERESEGMENT);
    bm->setNewSegmentColor(0.5f);
    vec=QVector3D(0.1f,0.2f,0.3f);
    bm->setNewSegmentRestRotation(vec);
    vec=QVector3D(0.4f,0.5f,0.6f);
    bm->setNewSegmentRestTranslation(vec);
    bm->createNewSegment();

    segName="Seg3";
    am->setBodyEditorTranslation(2,789,1012);
    bm->setNewSegmentId(3);
    bm->setNewSegmentName(segName);
    bm->setNewSegmentParentId(1);
    bm->setNewSegmentType(BrainiacGlobals::SPHERESEGMENT);
    bm->setNewSegmentColor(0.5f);
    vec=QVector3D(0.1f,0.2f,0.3f);
    bm->setNewSegmentRestRotation(vec);
    vec=QVector3D(0.4f,0.5f,0.6f);
    bm->setNewSegmentRestTranslation(vec);
    bm->createNewSegment();

    segName="Seg4";
    am->setBodyEditorTranslation(2,789,1012);
    bm->setNewSegmentId(4);
    bm->setNewSegmentName(segName);
    bm->setNewSegmentParentId(2);
    bm->setNewSegmentType(BrainiacGlobals::SPHERESEGMENT);
    bm->setNewSegmentColor(0.5f);
    vec=QVector3D(0.1f,0.2f,0.3f);
    bm->setNewSegmentRestRotation(vec);
    vec=QVector3D(0.4f,0.5f,0.6f);
    bm->setNewSegmentRestTranslation(vec);
    bm->createNewSegment();
}

QTEST_MAIN(BrainiacSceneTest)

#include "tst_brainiacscenetest.moc"
