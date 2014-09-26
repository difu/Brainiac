#include <QtCore/QString>
#include <QtTest/QtTest>

#include <QTemporaryFile>
#include <QGraphicsItem>

#include "core/scene.h"
#include "core/simulation.h"
#include "core/generator/pointgenerator.h"
#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"
#include "core/agent/body/body.h"

#include "core/agent/brain/brain.h"
#include "core/agent/channel.h"

#include "core/agent/body/bodymanager.h"

#include "core/agent/body/animation/animation.h"
#include "core/agent/body/animation/animationcurve.h"
#include "core/agent/body/animation/modifiableanimation.h"
#include "core/agent/body/animation/animationcurve.h"
#include "core/agent/body/animation/latchcurve.h"
#include "core/agent/body/animation/motiontree.h"
#include "core/agent/body/animation/motiontreemanager.h"
#include "core/agent/body/animation/motiontreeaction.h"
#include "core/agent/body/animation/motiontreetransition.h"
#include "gui/Animation/motiontreeeditoritem.h"

#include "core/group/group.h"


#include "core/agent/brain/fuzzyand.h"
#include "core/agent/brain/input.h"
#include "core/agent/brain/fuzzyor.h"

typedef QList<QVector2D> QListVec2d;
typedef QList<qreal> QListQReal;

Q_DECLARE_METATYPE(FuzzyAnd::Mode)
Q_DECLARE_METATYPE(FuzzyOr::Mode)
Q_DECLARE_METATYPE(BrainiacGlobals::AnimationType)
Q_DECLARE_METATYPE(QListVec2d)

class BrainiacTest : public QObject
{
    Q_OBJECT

public:
    BrainiacTest();

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

    void agentManager_data();
    void agentManager();

    void agentInstancing_data();
    void agentInstancing();

    void motionTree();

    void curves();
    void animation_data();
    void animation();

    void fuzzyAnd_data();
    void fuzzyAnd();
    void fuzzyOr_data();
    void fuzzyOr();
    void simulation_data();

    /**
     * @brief tests basic functionality of a simulation
     * saving/loading
     *
     * @fn simulation
     */
    void simulation();
    void simulation1();

    void cleanupTestCase();


private:
    static const QString testInput1Name;
    static const QString testInput2Name;
    static const QString testInput3Name;
    static const QString testAndName;
    static const QString testOrName;
    static int m_numOfTestSegments;
    void createSimpleTestBody(AgentManager *am, int var);
    void createAndOrTestBrain(AgentManager *am);

};

//#include "brainiactest.moc"
