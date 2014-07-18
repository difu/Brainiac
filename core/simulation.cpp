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
#include <QtConcurrent/QtConcurrent>
#include "core/agent/agent.h"
#include "core/agent/agentmanager.h"
#include "core/brainiacerror.h"
#include "core/scene.h"
#include "core/simulationsettings.h"
#include "core/brainiaclogger.h"

void advanceAgent(Agent* param_agent)
{
    param_agent->advance();
}

void advanceAgentCommit(Agent* param_agent)
{
    param_agent->advanceCommit();
    if(param_agent->getScene()->getSimulation()->writeMotionData()) {
        param_agent->writeBVHMotionData();
    }
}

void agentReset(Agent* param_agent)
{
    param_agent->reset();
}

Simulation::Simulation(Scene *scene) :
    QObject(scene),  m_currentFrameIsCalculated(false), m_scene(scene)
{
    m_settings=new SimulationSettings(this);
    m_frameCalculationTime=0;
    m_currentFrame=0;
    m_late=true;
    m_running=false;
    //m_fps=24;
    startTimer(1000/getFps());
    connect(&m_futureWatcherAdvance,SIGNAL(finished()),this,SLOT(advanceDone()));
    connect(&m_futureWatcherAdvanceCommit,SIGNAL(finished()),this,SLOT(advanceCommitDone()));
    m_agents=m_scene->getAgents();
}


void Simulation::advance()
{
    if(m_simMutex.tryLock() ) {
        if(!m_currentFrameIsCalculated){
            m_currentFrameIsCalculated=true;
            m_t.start();
            // Kepp the List up2date
            m_agents=m_scene->getAgents();
            m_futureWatcherAdvance.setFuture(QtConcurrent::map(m_agents,&::advanceAgent));
        }
        m_simMutex.unlock();
    } else {
    }
}

void Simulation::advanceDone()
{
    m_futureWatcherAdvanceCommit.setFuture(QtConcurrent::map(m_agents,&::advanceAgentCommit));
}

void Simulation::advanceCommitDone()
{
    m_currentFrame++;
    emit frameDone();
    m_frameCalculationTime=m_t.elapsed();
    QMutexLocker locker(&m_simMutex);
    Q_UNUSED(locker);
    m_currentFrameIsCalculated=false;

    qCDebug(bSimulation) << __PRETTY_FUNCTION__ << "frame calc time " << m_frameCalculationTime << "for frame " << m_currentFrame;
    if(m_frameCalculationTime>1000/(int)getFps()) {
        m_late=true;
        qCDebug(bSimulation) << __PRETTY_FUNCTION__ << "sim is late ";
    } else {
        m_late=false;
    }
    if(getSimulationMode()==SIMULATE) {
        if(m_currentFrame>=m_settings->getEndFrame()) {
            m_running=false;
        }
    }
    if(!m_running) {
        emit stopped();
    }
}


void Simulation::advanceOneFrame()
{
    if(m_simMutex.tryLock() ) {
        m_t.start();
        // Kepp the List up2date
        m_agents=m_scene->getAgents();
        QtConcurrent::blockingMap(m_agents,&::advanceAgent);
        QtConcurrent::blockingMap(m_agents,&::advanceAgentCommit);
        m_currentFrame++;
        emit frameDone();
        m_simMutex.unlock();
        m_frameCalculationTime=m_t.elapsed();
    } else {
        qDebug() << __PRETTY_FUNCTION__ << "simualtion is locked!";

    }

}

void Simulation::cancelSimulation()
{
    qCDebug(bSimulation) << __PRETTY_FUNCTION__ << "Cancelling started";
    stopSimulation();
    m_futureWatcherAdvance.cancel();
    m_futureWatcherAdvanceCommit.cancel();
    resetSimulation();
    qCDebug(bSimulation) << __PRETTY_FUNCTION__ << "Simulation canceled completly";
}

quint32 Simulation::getCurrentFrame() const
{
    return m_currentFrame;
}

qreal Simulation::getCurrentTimeMs() const
{
    return m_currentFrame*1000.0/getFps();
}

quint32 Simulation::getFps() const
{
    return m_settings->getFps();
}

qreal Simulation::getFpsCalc() const {
    if(m_running) {
        if( !qFuzzyCompare(m_frameCalculationTime+1.0f,1.0f)) {
            qreal fps=1000.0f/m_frameCalculationTime;
            if(fps>(qreal)getFps())
                return (qreal)getFps();
            else
                return fps;
        }
        return (qreal)getFps();
    } else
        return 0.0;
}

void Simulation::resetSimulation() {
    QMutexLocker locker(&m_simMutex);
    Q_UNUSED(locker);
    stopSimulation();
    m_futureWatcherAdvance.waitForFinished();
    m_futureWatcherAdvanceCommit.waitForFinished();
    QList<Agent *> agents=m_scene->getAgents();
    QtConcurrent::blockingMap(agents,&::agentReset);
    m_currentFrame=0;
}

bool Simulation::startSimulation()
{
    if(!m_running) {
        // if we run in simulate mode, check if we want to write something to any sim out dirs
        if(getSimulationMode()==SIMULATE) {
            if( writeMotionData() ) {
                QString dir=m_scene->getAbsoluteFileDir()%QDir::separator()%m_settings->getMotionOutputDir()%QDir::separator();
                qCDebug(bSimulation) << __PRETTY_FUNCTION__ << "writing sim output to " << dir;
                m_agents=m_scene->getAgents();
                foreach(Agent *agent, m_agents) {
                    agent->createBVHChannelList();
                    QFile agentMotionFile(dir%agent->objectName()%".bvh");
                    if (!agentMotionFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
                        qCDebug(bSimulation) << __PRETTY_FUNCTION__ << "unable to open file " << agentMotionFile.fileName();
                        BrainiacError::setLastError(BrainiacError::COULD_NOT_WRITE_FILE,agentMotionFile.fileName(),0,"unable to write to file for motion data");
                        cancelSimulation();
                        return false;
                    }
                    qCDebug(bSimulation) << __PRETTY_FUNCTION__ << "writing to file " << agentMotionFile.fileName();
                    agent->setBVHFileName(agentMotionFile.fileName());
                    QTextStream out(&agentMotionFile);
                    out << agent->getAgentManager()->getBVHSkeleton();
                    out << "MOTION\n" <<
                           "Frames: " << QString::number(m_settings->getEndFrame()-m_settings->getStartFrame()) << "\n" <<
                           "Frame Time: " << QString::number(1.0/(qreal)m_settings->getFps(),'f') << "\n";
                }
            }
        }
        emit started();
    }
    m_running=true;
    return true;
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

bool Simulation::writeMotionData() const
{
    return (!m_settings->getMotionOutputDir().isEmpty() && m_settings->getWriteMotion() );
}

Simulation::~Simulation()
{
    cancelSimulation();
    qDebug() << __PRETTY_FUNCTION__;
}

