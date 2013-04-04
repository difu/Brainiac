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
    void checkAgentProperties();
    void compareScene1Scene2Agents();
    void cleanupTestCase();
    void sceneTestCase1();

private:
    void createBody(AgentManager *am, int var);
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

QTEST_MAIN(BrainiacSceneTest)

#include "tst_brainiacscenetest.moc"
