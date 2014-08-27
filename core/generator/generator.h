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


#ifndef GENERATOR_H
#define GENERATOR_H

#include <QList>
#include <QXmlStreamAttributes>
#include <QHash>
#include <QVector3D>
#include "locator.h"

class Scene;
class QVector4D;
class QXmlStreamReader;
class QXmlStreamWriter;

/** \brief  Generator base class for agent instances

        Generates agent instances based on differnt types of modes. Also groups are assigned to instances.

**/
class Generator
{
public:
    enum GeneratorType{ POINT, //!< Generates agents around a point
                        CIRCLE //!< Generates agents within a circle
                      };


    Generator( GeneratorType genType, Scene *scene);

    QVector4D getCenter() const;

    /**
     * @brief returns a list of all Locators
     *
     * @return QList<Locator> locators of this Generator
     */
    const QList <Locator *>* getLocations() const;

    QString getName() const { return m_name; }

    /**
     * @brief returns the Scene
     *
     * @return Scene * the scene
     */
    Scene* getScene() const { return m_scene; }

    /**
     * @brief returns the type of this Generator
     *
     * @return GeneratorType the type
     */
    GeneratorType getType() const { return m_genType; }

    void setCenter(QVector3D &center ) { m_centerPos=center; }
    void setGroupRatios( QHash<quint32,qreal> ratios) { m_groupRatio=ratios; }
    void setNumber(quint32 number) { m_number=number; }
    void setAngleNoise(qreal noise) { m_angleNoise=noise; }
    void setDistance(qreal distance) { m_distance=distance; }
    virtual void loadConfig(QXmlStreamReader *xml)=0;
    virtual void saveConfig(QXmlStreamWriter *xml)=0;
    virtual void generateLocators()=0;
    virtual ~Generator();

protected:
    void readCommonXmlAttribs( const QXmlStreamAttributes &attribs);
    QString m_name;
    GeneratorType m_genType; /**< the type of the generator */
    QList <Locator *> m_locations; //!< x,y,z Position in world space, w rotation angle
    Scene *m_scene; /**< the Scene */
    quint32 m_id;
    QVector4D m_centerPos;
    quint32 m_number;
    qreal m_distance;
    qreal m_angle;
    qreal m_angleNoise;
    QHash<quint32,qreal> m_groupRatio;
};

#endif // GENERATOR_H
