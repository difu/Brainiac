#include "brainiactest.h"


BrainiacTest::BrainiacTest()
{
    QLoggingCategory::setFilterRules("*.debug=true");
}

const QString BrainiacTest::testInput1Name=QString("TESTINPUT1");
const QString BrainiacTest::testInput2Name=QString("TESTINPUT2");
const QString BrainiacTest::testInput3Name=QString("TESTINPUT3");
const QString BrainiacTest::testAndName=QString("TESTAND");
const QString BrainiacTest::testOrName=QString("TESTOR");
int BrainiacTest::m_numOfTestSegments=0;


void BrainiacTest::initTestCase()
{
    QVERIFY2(BrainiacGlobals::ChannelNames_Latches.count()==(int)MotionTreeManager::NUM_OF_TREE_TRACKS,"Latch channel name count mismatch!");
    QVERIFY2(BrainiacGlobals::ChannelNames_Phases.count()==(int)MotionTreeManager::NUM_OF_TREE_TRACKS,"Phase channel name count mismatch!");
}

void BrainiacTest::parseBVH()
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
    testScene1->deleteLater();
}

void BrainiacTest::agentManager_data()
{
    QTest::addColumn<QString>("agentName");

    QTest::addColumn<QString>("segment1Name");

    QTest::addColumn<QString>("segment2Name");

    QTest::addColumn<QString>("segment3Name");

    QTest::addColumn<QString>("segment4Name");

    QTest::newRow("Case1") << "agent1" \
                           << "root" \
                           << "rootChild1" \
                           << "rootChild2" \
                           << "child1Child";

}

void BrainiacTest::agentManager()
{
    Scene testScene1;
    Group *grp=new Group(&testScene1);
    grp->setId(1);
    AgentManager *am1=grp->getAgentManager();
    BodyManager *bm1=am1->getBodyManager();
//    osgViewer::Viewer m_viewer;
//    m_viewer.setSceneData(am1->getActionAgent()->getBody()->getBodyRoot());
//    m_viewer.updateTraversal();

}

void BrainiacTest::motionTree()
{
    QString action1Name="action 1";
    QString transition1Name="trans 1";
    quint32 transColor=2;
    QTemporaryFile tmpFile;

    Scene testScene1;
    Group *grp=new Group(&testScene1);
    grp->setId(1);
    AgentManager *am1=grp->getAgentManager();
    MotionTreeManager *mtm=am1->getMotionTreeManager();
    MotionTree *tree1=mtm->getMotionTrees().value(1);
    tree1->addAction();
    tree1->addAction();
    tree1->addTransition();
    tree1->addTransition();
    QVERIFY2(tree1->getActions().count()==2,  "Wrong number of unnamed actions");
    QVERIFY2(tree1->getTransitions().count()==2,  "Wrong number of unnamed transitions");

    tree1->addAction(action1Name);
    tree1->addTransition(transition1Name);

    //Connect Action with Transition
    bool connSuccess=tree1->connectActionWithTransition(action1Name,transition1Name);
    QVERIFY2(connSuccess,"Could not connect action with transition");

    connSuccess=tree1->connectActionWithTransition(action1Name,transition1Name);
    QVERIFY2(connSuccess==false,"Could connect action with transition, although connection already exists");

    connSuccess=tree1->connectActionWithTransition("Does not exist","Does not exists too");
    QVERIFY2(connSuccess==false,"Could connect action where action and transition do not exist");

    connSuccess=tree1->connectActionWithTransition(action1Name,"Dummy");
    QVERIFY2(connSuccess==false,"Could connect action with transition, although transition does not exist");

    connSuccess=tree1->connectActionWithTransition("Dummy",transition1Name);
    QVERIFY2(connSuccess==false,"Could connect action with transition, although action does not exist");

    // Connect Transition with Action
    connSuccess=tree1->connectTransitionWithAction(transition1Name,action1Name);
    QVERIFY2(connSuccess,"Could not connect transition with action");

    connSuccess=tree1->connectTransitionWithAction(transition1Name,action1Name);
    QVERIFY2(connSuccess==false,"Could connect transition with action, although connection already exists");

    connSuccess=tree1->connectTransitionWithAction("Does not exist","Does not exist");
    QVERIFY2(connSuccess==false,"Could connect transition with action, although transition and action do not exist");

    connSuccess=tree1->connectTransitionWithAction(transition1Name,"Dummy");
    QVERIFY2(connSuccess==false,"Could connect transition with action, although action not exists");

    connSuccess=tree1->connectTransitionWithAction("Dummy",action1Name);
    QVERIFY2(connSuccess==false,"Could connect transition with action, although action not exists");

    QVERIFY2(tree1->getActions().count()==3,  "Wrong number of unnamed actions");
    QVERIFY2(tree1->getTransitions().count()==3,  "Wrong number of unnamed transitions");

    // saving/loading tests

    QString standActionName="stand";
    QPointF standActionEditorPos=QPointF(10,20);
    QString standTransName="standTrans";
    QPointF standTransEditorPos=QPointF(-10,20);
    QString defaultActionName=standActionName;
    QString triggerStandName="stand";
    QString triggerStandName2="stand2";
    QString triggerWalkName="walk";
    QList<QString> triggers;
    triggers << triggerStandName << triggerStandName2 << triggerWalkName;

    Scene saveScene2;
    Group *saveGroup=new Group(&saveScene2);
    saveGroup->setId(1);
    AgentManager *saveAgentManager=saveGroup->getAgentManager();
    MotionTreeManager *saveMotionTreeManager=saveAgentManager->getMotionTreeManager();
    MotionTree *saveTree1=saveMotionTreeManager->getMotionTrees().value(1);

    saveTree1->addAction(standActionName);
    saveTree1->getActions().value(standActionName)->getEditorItem()->setPos(standActionEditorPos);
    saveTree1->addTransition(standTransName);
    MotionTreeTransition *saveTrans=saveTree1->getTransitions().value(standTransName);
    saveTrans->setColorId(transColor);
    saveTrans->getEditorItem()->setPos(standTransEditorPos);
    saveTree1->setTreeDefaultAction(defaultActionName);

    foreach(QString tr, triggers) {
        saveMotionTreeManager->addTrigger(tr);
    }

    quint32 standTriggerId=saveMotionTreeManager->getTriggers().key(triggerStandName);
    quint32 stand2TriggerId=saveMotionTreeManager->getTriggers().key(triggerStandName2);

    MotionTreeAction *standAction=saveTree1->getActions().value(standActionName);
    standAction->addTriggerId(standTriggerId);
    standAction->addTriggerId(stand2TriggerId);

    saveTree1->connectActionWithTransition(standActionName,standTransName);
    saveTree1->connectTransitionWithAction(standTransName,standActionName);

    saveTree1->setTreeDefaultAction(standActionName);
    QVERIFY2(tmpFile.open(),"Temp file could not be opened");
    saveAgentManager->setFileName(tmpFile.fileName());
    saveAgentManager->saveConfig();

    Scene LoadScene;
    Group *loadGroup=new Group(&LoadScene);
    AgentManager *loadAgentManager=loadGroup->getAgentManager();
    loadAgentManager->setFileName(tmpFile.fileName());
    loadAgentManager->loadConfig();

    //Check, if tree 1 is as defined
    MotionTreeManager *loadTreeManager=loadAgentManager->getMotionTreeManager();
    MotionTree *loadTree1=loadTreeManager->getMotionTrees().value(1);
    QVERIFY2(loadTree1->getDefaultActionName()==defaultActionName,"Default action name of loaded tree differs!");
    MotionTreeAction *action=loadTree1->getActions().value(standActionName,0);
    QVERIFY2(action,"No stand action loaded");
    MotionTreeTransition *trans=loadTree1->getTransitions().value(standTransName,0);
    QVERIFY2(trans,"No stand transition loaded");

    // Check Triggers
    QVERIFY2(loadTreeManager->getTriggers().count()==saveMotionTreeManager->getTriggers().count(),"loaded num of triggers differs");
    QHashIterator<quint32, QString> itTriggers(saveMotionTreeManager->getTriggers());
    while(itTriggers.hasNext()) {
        itTriggers.next();
        if(loadTreeManager->getTriggers().contains(itTriggers.key())) {
            QVERIFY2(itTriggers.value()==loadTreeManager->getTriggers().value(itTriggers.key()),"loaded trigger name differ");
        } else {
            QFAIL("loaded trigger does not contain saved key(s)");
        }
    }
    foreach(QString tr, triggers) {
        QVERIFY2(loadTreeManager->getTriggers().key(tr)>0,"loaded triggers does not contain trigger");
    }

    QVERIFY2(trans->getEditorItem()->pos()==standTransEditorPos,"Transition EditorPos differs");
    QVERIFY2(action->getEditorItem()->pos()==standActionEditorPos,"Action EditorPos differs");
    QVERIFY2(trans->getColorId()==transColor,"Transition loaded color differs");

    QVERIFY2(loadTree1->getActionTransitionConnections().count()==1,"Wrong number of loaded Action Trans connections!");
    QVERIFY2(loadTree1->getTransitionActionConnections().count()==1,"Wrong number of loaded Trans Action connections!");

    // Check all trees have the same contents as the originals
    for(unsigned int i=0; i<MotionTreeManager::NUM_OF_TREE_TRACKS;i++) {
        MotionTree *loadTree=loadAgentManager->getMotionTreeManager()->getMotionTrees().value(i);
        MotionTree *saveTree=saveAgentManager->getMotionTreeManager()->getMotionTrees().value(i);
        QVERIFY2(loadTree->getActions().count()==saveTree->getActions().count(),"number of loaded tree actions differs");
        QVERIFY2(loadTree->getTransitions().count()==saveTree->getTransitions().count(),"number of loaded tree transitions differs");
        QVERIFY2(loadTree->getTransitionActionConnections().count()==saveTree->getTransitionActionConnections().count(),"number of loaded trans action connections differs");
        QVERIFY2(loadTree->getActionTransitionConnections().count()==saveTree->getActionTransitionConnections().count(),"number of loaded action trans connections differs");
    }
    MotionTreeAction *loadedStandAction=loadTree1->getActions().value(standActionName);
    QVERIFY2(loadedStandAction->getTriggerIds().count()==2,"loaded stand action trigger id count !=2");

}

void BrainiacTest::curves()
{
    AnimationCurve curve=ModifiableAnimation::createTransitionCurve();
    QVERIFY2(curve.keyFrames().count()==(int) ModifiableAnimation::TransitionCurveKeyFrames+1,"Wrong number of keyframes");
    qreal max=curve.getMaxValue();
    qreal min=curve.getMinValue();
    qreal length=curve.length();
    QVERIFY2(qFuzzyCompare(length,ModifiableAnimation::TransitionCurveLengthMs),"wrong ength of Transitioncurve");
    QVERIFY2(qFuzzyCompare(max,1.0),"max value too high");
    QVERIFY2(qFuzzyCompare(min+1.0,1.0),"min value too low");
}


void BrainiacTest::animation_data()
{
    QTest::addColumn<QString>("animationName");

    QTest::addColumn<QString>("curve1Name");
    QTest::addColumn<QlistVec2d>("keyframesCurve1");

    QTest::addColumn<QString>("curve2Name");
    QTest::addColumn<QlistVec2d>("keyframesCurve2");

    QTest::addColumn<qreal>("maxTime"); // Expected max time

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

    QList<QVector2D> kfcurve1case1;
    kfcurve1case1 << QVector2D(0.0,0.3) << QVector2D(0.1,0.3) << QVector2D(0.2,-50.3) << QVector2D(0.4,0.3) << QVector2D(1.0,0.3);
    QList<QVector2D> kfcurve2case1;
    kfcurve2case1 << QVector2D(0.0,0.1) << QVector2D(0.12,0.2) << QVector2D(0.21,-50.3) << QVector2D(10.0,0.4);

    QTest::newRow("Case1") << "anim1" \
                           << "curve1" << kfcurve1case1 \
                           << "curve2" << kfcurve2case1 \
                           << 10.0\
                           << BrainiacGlobals::STATIC  << false << true\
                           << "latch1" << QVector2D(0.0,0.1) << QVector2D(0.2,0.1) << QVector2D(0.4,0.1)\
                           << "latch2" << QVector2D(0.0,0.11) << QVector2D(0.2,0.12) << QVector2D(0.4,0.13);

    QList<QVector2D> kfcurve1case2;
    kfcurve1case2 << QVector2D(0.0,0.3) << QVector2D(0.1,0.3) << QVector2D(0.2,-50.3) << QVector2D(0.4,0.3) << QVector2D(1.0,0.3);
    QList<QVector2D> kfcurve2case2;
    kfcurve2case2 << QVector2D(0.0,0.1) << QVector2D(0.12,0.2) << QVector2D(0.21,-50.3) << QVector2D(10.0,0.4) << QVector2D(11.0,0.4);

    QTest::newRow("Case2") << "aNimAtion"\
                           << "curve1" << kfcurve1case2 \
                           << "curve2" << kfcurve2case2 \
                           << 11.0\
                           << BrainiacGlobals::LOCOMOTION << true << false\
                           << "latch1" << QVector2D(0.0,0.15) << QVector2D(0.2,0.15) << QVector2D(0.4,0.15)\
                           << "latch2" << QVector2D(0.0,0.111) << QVector2D(0.2,0.123) << QVector2D(0.4,0.134);


}

void BrainiacTest::animation()
{
    QTemporaryFile animFile;

    QFETCH(QString, curve1Name);
    QFETCH(QString, curve2Name);
    AnimationCurve testCurve1;
    AnimationCurve testCurve2;
    QFETCH(qreal, maxTime);

    QFETCH(QlistVec2d, keyframesCurve1);
    QFETCH(QlistVec2d, keyframesCurve2);

    foreach( QVector2D kf, keyframesCurve1) {
        testCurve1.addKeyFrame(kf);
    }
    foreach( QVector2D kf, keyframesCurve2) {
        testCurve2.addKeyFrame(kf);
    }

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

    AnimationCurve transCurve=ModifiableAnimation::createTransitionCurve();
    testAnim.setTransitionCurve(transCurve);

    if(!animFile.open()) {
        QFAIL("Failed creation of tempfile");
    }
    testAnim.setFileName(animFile.fileName());
    QVERIFY2(testAnim.saveAnimation(),"Error while saving animation");

    Animation* loadedAnimation;
    loadedAnimation=Animation::loadAnimation(animFile.fileName());
qDebug() << "got " << testAnim.getLength()<< "Exp: " << maxTime;
    QVERIFY2(qFuzzyCompare(testAnim.getLength(),maxTime),"Wrong length calculation!");

    QVERIFY2(loadedAnimation->name().compare(animationName)==0,"wrong animation name in loaded animation");

    QVERIFY2(loadedAnimation->animationType()==animType,"wrong animation type!");
    QVERIFY2(testAnim.animationType()==loadedAnimation->animationType(),"Loaded animation has different animytion type!");

    QVERIFY2(testAnim.isLoopedAnimation()==isLooped,"looped property differs!");
    QVERIFY2(testAnim.isLoopedAnimation()==loadedAnimation->isLoopedAnimation(),"looped property of loaded animation differs!");

    QVERIFY2(testAnim.isRetriggerable()==isRetriggerable,"is retriggerable differs");
    QVERIFY2(testAnim.isRetriggerable()==loadedAnimation->isRetriggerable(),"is retriggerable of loaded animation differs");

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
    QVERIFY2(loadedAnimation->getTransitionCurve()->keyFrames()==testAnim.getTransitionCurve()->keyFrames(),"Transititoncurve number of keyframes differ from original");
    delete(loadedAnimation);
}

void BrainiacTest::fuzzyAnd_data()
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

void BrainiacTest::fuzzyAnd()
{
    Scene testScene1;
    Group *grp=new Group(&testScene1);
    grp->setId(1);
    createBody(grp->getAgentManager(),1);
    createBrain(grp->getAgentManager());

    QVERIFY(testScene1.getAgents().count()==0);
    QVERIFY2(testScene1.getGroups().count()>0,"No group in scene!");
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

void BrainiacTest::fuzzyOr_data()
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


void BrainiacTest::fuzzyOr()
{
    Scene testScene1;
    Group *grp=new Group(&testScene1);
    grp->setId(1);
    createBody(grp->getAgentManager(),1);
    createBrain(grp->getAgentManager());

    QVERIFY(testScene1.getAgents().count()==0);
    QVERIFY2(testScene1.getGroups().count()>0,"No group in scene!");
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
    //testScene1->deleteLater();
}

void BrainiacTest::cleanupTestCase()
{

}

void BrainiacTest::sceneCreateLoadSave()
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
    testScene1->deleteLater();
    testScene2->deleteLater();
}

void BrainiacTest::simulation1()
{
    int numOfAgents=10;
    int numOfFrames=10;
    Scene testScene1;
    Group *testGroup1=new Group(&testScene1);
    PointGenerator *gen=new PointGenerator(&testScene1);
    QHash<quint32,qreal> groupRatio; groupRatio.insert(testGroup1->getId(),1);
    gen->setNumber(numOfAgents);
    gen->setGroupRatios(groupRatio);
    testScene1.addGenerator(gen);
    testScene1.createAgents();
    QVERIFY2(testScene1.getAgents().count()==numOfAgents,"Wrong number of agents created!");
    testGroup1->getAgentManager()->addOutputFuzz("tz","tz",0,10,10,10);
    QHash<Agent *,QVector3D > agentPos;

    // Walk "numOfFrames" frames straight ahead
    foreach(Agent *agent, testScene1.getAgents()) {
        agent->getOutputChannel("tz")->setValue(1);
        agentPos.insert(agent, agent->getPosition());
    }
    for(int i=0; i<numOfFrames; i++) {
        testScene1.getSimulation()->advanceOneFrame();
        foreach(Agent *agent, testScene1.getAgents()) {
            QVector3D newPos=agentPos.value(agent)+QVector3D(0,0,1)*(i+1);
            //qDebug()  << newPos << agent->getPosition();
            QVERIFY2(agent->getPosition()!=agentPos.value(agent),"Agent hasnt moved");
            QVERIFY2(qFuzzyCompare(agent->getPosition(),newPos),"Wrong agent position when walking straight");
        }
    }
    QVERIFY2((int)testScene1.getSimulation()->getCurrentFrame()==numOfFrames,"Wrong frame count");
    testScene1.getSimulation()->resetSimulation();
    QVERIFY2((int)testScene1.getSimulation()->getCurrentFrame()==0,"Wrong frame count after reset");

    foreach(Agent *agent, testScene1.getAgents()) {
        QVERIFY2(agent->getPosition()==agentPos.value(agent),"Agent pos was not reset");
    }

    // Turn 90 degrees, stop walking
    foreach(Agent *agent, testScene1.getAgents()) {
        agent->getOutputChannel("tz")->setValue(0);
        agent->getOutputChannel("ry")->setValue(90);
    }
    // Stop turning, start walking
    testScene1.getSimulation()->advanceOneFrame();
    foreach(Agent *agent, testScene1.getAgents()) {
        agent->getOutputChannel("tz")->setValue(1);
        agent->getOutputChannel("ry")->setValue(0);
    }
    for(int i=0; i<numOfFrames; i++) {
        testScene1.getSimulation()->advanceOneFrame();
        foreach(Agent *agent, testScene1.getAgents()) {
            QVector3D newPos=agentPos.value(agent)+QVector3D(1,0,0)*(i+1);
            // qDebug()  << newPos << agent->getPosition();
            QVERIFY2(agent->getPosition()!=agentPos.value(agent),"Agent hasnt moved");
            QVERIFY2(qFuzzyCompare(agent->getPosition(),newPos),"Wrong agent position when walking straight");
        }
    }
//    QBENCHMARK {
//        testScene1.getSimulation()->advanceOneFrame();
//    }
}

void BrainiacTest::createBody(AgentManager *am, int var)
{
        //!< @bug @todo rewrite with statical data
    QVector3D testVec=QVector3D(1,2,3);
    QVector3D vec;
    QString segName="Seg1";
    BodyManager *bm=am->getBodyManager();

    am->setBodyEditorTranslation(1,10*var,5*var);
    bm->setNewSegmentId(1);
    bm->setNewSegmentName(segName);
    bm->setNewSegmentParentId(0);
    bm->setNewSegmentType(BrainiacGlobals::TUBESEGMENT);
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
    bm->setNewSegmentType(BrainiacGlobals::BOXSEGMENT);
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

void BrainiacTest::createBrain(AgentManager *am)
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

QTEST_MAIN(BrainiacTest)
