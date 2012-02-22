#ifndef SIMULATION_H
#define SIMULATION_H

#include <QObject>
#include <QMutex>

class Scene;


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
     * @brief returns the current frame of the Simulation
     *
     * @return quint32 the current frame of the Simulation
     */
    quint32 getCurrentFrame() const;

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

    /** \brief @returns true if simulation is running
    **/
    bool isRunning() const { return m_running; }

protected:
    int m_frameCalculationTime; //!< time in ms a frame calculation took
    quint32 m_fps; //!< frametrate of the sim
    bool m_running; //!< true, if simulation is currently running
    Scene *m_scene; //!< The scene
    quint32 m_currentFrame;  //!< current frame of the simualtion;
    bool m_late; //!< shows, if sim step is calculated in realtime
    QMutex m_simMutex; //!< prevent new simulation step when current has not finished yet
    void advance();
    void timerEvent(QTimerEvent *);

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
