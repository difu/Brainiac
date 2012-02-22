#include "simulation.h"
#include "QtConcurrentMap"
#include "core/agent/agent.h"
#include "core/scene.h"

void advanceAgent(Agent* param_agent)
{
    param_agent->advance();
}

void advanceAgentCommit(Agent* param_agent)
{
    param_agent->advanceCommit();
}

void agentReset(Agent* param_agent)
{
    param_agent->reset();
}

Simulation::Simulation(Scene *scene) :
    QObject(), m_scene(scene)
{
    m_frameCalculationTime=0;
    m_currentFrame=0;
    m_late=true;
    m_running=false;
    m_fps=24;
    startTimer(1000/m_fps);
}


/** \brief go one step further in simulation

**/
void Simulation::advance()
{
    if(m_simMutex.tryLock()) {
        QTime t;
        t.start();
        QList<Agent *> agents=m_scene->getAgents();
        QtConcurrent::blockingMap(agents,&::advanceAgent);
        QtConcurrent::blockingMap(agents,&::advanceAgentCommit);
        m_frameCalculationTime=t.elapsed();
        m_currentFrame++;
        m_simMutex.unlock();
        emit frameDone();
    } else {
        m_late=false;
        qDebug() << __PRETTY_FUNCTION__ << "sim is late";
    }
}

quint32 Simulation::getCurrentFrame() const
{
    return m_currentFrame;
}

quint32 Simulation::getFps() const
{
    return m_fps;
}

qreal Simulation::getFpsCalc() const {
    if(m_running && m_frameCalculationTime!=0 ) {
        return 1.0f/m_frameCalculationTime;
    } else
        return 0.0f;
}

void Simulation::resetSimulation() {
    m_simMutex.lock();
    QList<Agent *> agents=m_scene->getAgents();
    stopSimulation();
    QtConcurrent::blockingMap(agents,&::agentReset);
    m_simMutex.unlock();
}

void Simulation::startSimulation()
{
    m_running=true;
}

void Simulation::stopSimulation()
{
    m_running=false;
}

void Simulation::timerEvent(QTimerEvent *)
{
    if(m_running) {
        this->advance();
    }
}
