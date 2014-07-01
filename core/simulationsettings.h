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

#ifndef SIMULATIONSETTINGS_H
#define SIMULATIONSETTINGS_H

#include <QObject>

class QXmlStreamWriter;
class QXmlStreamReader;

/**
 * @brief Manages the settings of a simulations
 *
 * @class SimulationSettings simulationsettings.h "core/simulationsettings.h"
 */
class SimulationSettings : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructor
     *
     * @fn SimulationSettings
     * @param parent
     */
    explicit SimulationSettings(QObject *parent = 0);


    /**
     * @brief returns the framerate of the Simulation
     *
     * @return quint32 the framerate
     */
    quint32 getFps() const { return m_fps;}

    /**
     * @brief get directory where motion data is to be saved
     *
     * @fn getMotionOutputDir
     * @return QString string containing the dir relative to scene directory
     */
    QString getMotionOutputDir() const { return m_motionOutDir; }

    /**
     * @brief returns if motion data should be written
     *
     * @fn getWriteMotion
     * @return bool true, if motion data should be written
     */
    bool getWriteMotion() const { return m_writeMotion; }

    /**
     * @brief loads the confog
     *
     * @fn loadConfig
     * @param xml pointer to the xml stream to read from
     */
    void loadConfig(QXmlStreamReader *xml);


    /**
     * @brief
     *
     * @fn saveConfig
     * @param xml pointer to the xml stream to write to
     */
    void saveConfig(QXmlStreamWriter *xml);

    /**
     * @brief sets the dir name to write motion data in
     * the directory is relative to scene directory
     *
     * @fn setMotionOutputDir
     * @param dirName the name of the dir
     */
    void setMotionOutputDir( const QString &dirName ) { m_motionOutDir=dirName;}

    /**
     * @brief switch on/off of writing motion data
     * switches on/off writing motion data in motion data dir
     * @sa setMotionOutputDir()
     * @sa getMotionOutputDir()
     * @fn setWriteMotion
     * @param write true, if motion data should be written
     */
    void setWriteMotion( bool write ) { m_writeMotion=write; }

private:
    QString m_motionOutDir; //!< directory to write ot motion data relative to scene directory
    bool m_writeMotion; //!< toggles writing of motion data in @sa m_motionOutDir
    quint32 m_fps; //!< frametrate of the sim

signals:

public slots:

};

#endif // SIMULATIONSETTINGS_H
