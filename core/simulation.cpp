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
    if(m_running) {
        if( !qFuzzyCompare(m_frameCalculationTime+1.0f,1.0f)) {
            qreal fps=1000.0f/m_frameCalculationTime;
            if(fps>(qreal)m_fps)
                return (qreal)m_fps;
            else
                return fps;
        }
        return (qreal)m_fps;
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
