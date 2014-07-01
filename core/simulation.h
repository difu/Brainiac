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


#ifndef SIMULATION_H
#define SIMULATION_H

#include <QObject>
#include <QMutex>
#include <QFutureWatcher>
#include <QTime>

class Scene;
class Agent;
class SimulationSettings;


/** \brief  The Simulation manager of the scene

        This class manages the simulation of this scene. When triggered it invokes the
        agents to advance one step further.
        @sa Agent
        @sa Scene

**/
class Simulation : public QObject
{
    Q_OBJECT
public:
    explicit Simulation(Scene *scene);

    /**
     * @brief advances the simulation one frame
     * This method blocks until calculation is done
     *
     */
    void advanceOneFrame();

    /**
     * @brief cancels the Simulation
     *
     * @fn cancelSimulation
     */
    void cancelSimulation();

    /**
     * @brief returns the current frame of the Simulation
     *
     * @return quint32 the current frame of the Simulation
     */
    quint32 getCurrentFrame() const;

    /**
     * @brief returns the current time of the Simulation in s
     *
     * @return quint32 the current time of the Simulation in s
     */
    qreal getCurrentTime() const { return (qreal)m_currentFrame*1.0/(qreal)getFps(); }

    /**
     * @brief returns the current time of the Simulation in ms
     *
     * @return quint32 the current time of the Simulation in ms
     */
    qreal getCurrentTimeMs() const;

    /**
     * @brief returns the framerate of the Simulation
     *
     * @return quint32 the framerate
     */
    quint32 getFps() const;

    /**
     * @brief returns the current calculation framerate of the simulation
     *
     * The function returns 0 if the simulation is not running

     * @return qreal the actual framerate
     */
    qreal getFpsCalc() const;
    SimulationSettings *getSettings() { return m_settings; }

    /** \brief @returns true if simulation is running
    **/
    bool isRunning() const { return m_running; }

    /**
     * @brief Destructor
     *
     * @fn ~Simulation
     */
    virtual ~Simulation();

private:
    bool m_currentFrameIsCalculated; //!< true, if the current frame is actually calculated
    int m_frameCalculationTime; //!< time in ms a frame calculation took
    bool m_running; //!< true, if simulation is currently running
    Scene *m_scene; //!< The scene
    quint32 m_currentFrame;  //!< current frame of the simualtion;
    bool m_late; //!< shows, if sim step is calculated in realtime
    QMutex m_simMutex; //!< prevent new simulation step when current has not finished yet
    QFutureWatcher<void> m_futureWatcherAdvance; /**< the future watcher of the async advance calculation */
    QFutureWatcher<void> m_futureWatcherAdvanceCommit; /**< th future watcher of the async advance commit calculation */
    SimulationSettings *m_settings;

    /** \brief go one step further in simulation

    **/
    void advance();
    void timerEvent(QTimerEvent *);
    QTime m_t;  /**< Time to calculate the frame calculation time */

    /** \brief QtConcurrent needs this list of all scene agents, can´t use Scene::getAgents()
    * this has to be kept up2date everytime advance() is called!
    **/
    QList<Agent *> m_agents;
private slots:
    /**
     * @brief called when the asynchronous calculation of agent advancing is done
     * It further starts the asynchronous calculation of agent advance commit step.
     * The mutex that holds the lock to prevent multiple calculations is NOT released here!
     * @sa Agent::advanceCommit()
     * @sa advanceCommitDone()
     */
    void advanceDone();


    /**
     * @brief called when the asynchronous calculation of agent advance commit is done
     * This function releases the lock to prevent multiple calculations
     * Also the signal frameDone() is emited
     * @sa frameDone()
     */
    void advanceCommitDone();


signals:
    void frameDone(); //!< emitted when a frame has been done
    void started(); //!< emitted when simulation starts
    void reset(); //!<  emmited when simulation is reset
    void stopped(); //!< emitted when simulation is stopped
    void simLate(); //!< emitted when simulation is late

public slots:
    /**
     * @brief starts the simulation
     *
     */
    void startSimulation();

    /**
     * @brief stops/pauses the simulation
     *
     */
    void stopSimulation();

    /**
     * @brief resets the simulation
     * the simulation is resetted and stopped
     * @sa stopSimulation()
     */
    void resetSimulation();
};

#endif // SIMULATION_H
