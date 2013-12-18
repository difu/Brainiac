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

#ifndef SEGMENT_H
#define SEGMENT_H

#include <QObject>
#include <QVector3D>
#include "core/brainiacglobals.h"
#include <osg/MatrixTransform>

/**
 * @brief Describes a segment of the Agent ´s Body
 *
 * @class Segment segment.h "core/agent/body/segment.h"
 */
class Segment : public QObject
{
    Q_OBJECT
public:
/**
 * @brief Constructs a Segment
 *
 * @fn Segment
 */
    Segment();
/**
 * @brief Copy constructor
 *
 * @fn Segment
 * @param other the original Segment
 */
    Segment( const Segment& other );

    void copyFromOther( const Segment &other);

    /**
     * @brief Destructor
     *
     * @fn ~Segment
     */
    virtual ~Segment();

    /**
     * @brief returns the diameter this segment
     * Use this if you want to query the diameter of the tube segment
     * Internally the diameter is stored as the x component of the @sa m_segmentSize property.
     * @fn getDiameter
     * @return qreal the diameter
     */
    qreal getDiameter() const { return m_segmentSize.x(); }

    /**
     * @brief returns the id of this segment
     *
     * @fn getId
     * @return quint32
     */
    quint32 getId() const { return m_segmentId; }

    /**
     * @brief returns the length this segment
     * Use this if you want to query the length of the tube segment
     * Internally the length is stored as the y component of the @sa m_segmentSize property.
     * @fn getLength
     * @return qreal the length
     */
    qreal getLength() const { return m_segmentSize.y(); }

    /**
     * @brief
     *
     * @fn getParentId
     * @return quint32
     */
    quint32 getParentId() const { return m_segmentParentId; }

    /**
     * @brief
     *
     * @fn getRotation
     * @return QVector3D
     */
    QVector3D getRotation() const { return m_segmentRotation; }

    /**
     * @brief returns this segments rotation
     *
     * @fn getRotation
     * @return QVector3D
     */
    QVector3D& getRotation() { return m_segmentRotation; }

    /**
     * @brief returns this segments rest rotation
     *
     * @return QVector3D
     */
    QVector3D getRestRotation() const { return m_segmentRestRotation; }

    /**
     * @brief returns a modifiable reference of this segments rest rotation
     *
     * @return QVector3D&
     */
    QVector3D& getRestRotation() { return m_segmentRestRotation; }

    osg::ref_ptr<osg::MatrixTransform> getTransform() const { return m_transformNode; }

    /**
     * @brief
     *
     * @return QVector3D
     */
    QVector3D getTranslation () const { return m_segmentTranslation; }

    /**
     * @brief
     *
     * @return QVector3D
     */
    QVector3D& getTranslation () { return m_segmentTranslation; }

    /**
     * @brief
     *
     * @return QVector3D
     */
    QVector3D getRestTranslation () const { return m_segmentRestTranslation; }

    /**
     * @brief
     *
     * @return QVector3D
     */
    QVector3D& getRestTranslation () { return m_segmentRestTranslation; }

    /**
     * @brief
     *
     * @return QVector3D
     */
    QVector3D getSize() const { return m_segmentSize; }

    /**
     * @brief
     *
     * @fn getName
     * @return QString
     */
    QString getName() const { return this->objectName(); }

    /**
     * @brief
     *
     * @fn getColor
     * @return qreal
     */

    qreal getColor() const { return m_segmentColor; }

    /**
     * @brief
     *
     * @fn getColorInherited
     * @return bool
     */
    bool getColorInherited() const { return m_segmentColorInherited; }

    /**
     * @brief
     *
     * @fn getType
     * @return BrainiacGlobals::SegmentType
     */
    BrainiacGlobals::SegmentType getType() const { return m_segmentType; }

    /**
     * @brief
     *
     * @return QList<BrainiacGlobals::RotTrans>
     */
    QList<BrainiacGlobals::RotTrans> getRotationTranslationOrder() const { return m_segmentRotTransOrder; }

    /**
     * @brief
     *
     * @return QList<BrainiacGlobals::RotTrans>
     */
    QList<BrainiacGlobals::RotTrans>& getRotationTranslationOrder() { return m_segmentRotTransOrder; }

    /**
     * @brief returns, if this is a root segment
     *
     * A segment is a root segment, if its parent id == 0
     * @fn isRootSegment
     * @return bool true, if this segment is a root segment
     */
    bool isRootSegment() const {
        return m_segmentParentId==0;
    }

    /**
     * @brief seth the diameter of this segment
     * Internally the x property of @sa m_segmentSize is set. Use this if you want to change the diameter of the tube
     *
     * @fn setDiameter
     * @param qreal diameter
     */
    void setDiameter(qreal diameter) {m_segmentSize.setX(diameter);
                                    updateAndNotify();}

    /**
     * @brief
     *
     * @fn setId
     * @param id
     */
    void setId(quint32 id) {m_segmentId=id;}

    /**
     * @brief seth the length of this segment
     * Internally the y property of @sa m_segmentSize is set. Use this if you want to change the length of the tube
     *
     * @fn setLength
     * @param qreal length
     */
    void setLength(qreal length) {m_segmentSize.setY(length);
                                    updateAndNotify();}

    /**
     * @brief
     *
     * @fn setParentId
     * @param parent
     */
    void setParentId(quint32 parent) {m_segmentParentId=parent;
                                      updateAndNotify();}
    /**
     * @brief
     *
     * @fn setTranslation
     * @param translation
     */
    void setTranslation(const QVector3D &translation) {m_segmentTranslation=translation;
                                                updateAndNotify();}
    /**
     * @brief
     *
     * @fn setRestTranslation
     * @param restTranslation
     */
    void setRestTranslation(const QVector3D &restTranslation) {m_segmentRestTranslation=restTranslation;
                                                        updateAndNotify();}
    /**
     * @brief
     *
     * @fn setRotation
     * @param rotation
     */
    void setRotation(const QVector3D &rotation) {m_segmentRotation=rotation;
                                          updateAndNotify();}
    /**
     * @brief
     *
     * @fn setRestRotation
     * @param restRotation
     */
    void setRestRotation(const QVector3D &restRotation) {m_segmentRestRotation=restRotation;
                                                  updateAndNotify();}
    /**
     * @brief
     *
     * @fn setRotationTranslationOrder
     * @param list
     */
    void setRotationTranslationOrder( QList<BrainiacGlobals::RotTrans> list);

    /**
     * @brief
     *
     * @fn setSize
     * @param size
     */
    void setSize(const QVector3D &size) {m_segmentSize=size;
                                    updateAndNotify();}
    /**
     * @brief
     *
     * @fn setType
     * @param type
     */
    void setType(BrainiacGlobals::SegmentType type) {m_segmentType=type;}
    /**
     * @brief
     *
     * @fn setName
     * @param name
     */

    void setName(const QString &name) {setObjectName(name);}
    /**
     * @brief
     *
     * @fn setColorInherited
     * @param inherited
     */
    void setColorInherited(bool inherited) {m_segmentColorInherited=inherited;
                                           updateAndNotify();}
    /**
     * @brief
     *
     * @fn setColor
     * @param color
     */
    void setColor(qreal color) {m_segmentColor=color;
                               updateAndNotify();}

protected:
    /**
     * @brief updateAndNotify Updates internal data and emit signals
     *
     * Whenever this is called, the transformation matrix is re-calculated
     */
    virtual void updateAndNotify();
    QVector3D m_segmentRotation; /**< TODO */
    QVector3D m_segmentTranslation; /**< TODO */
    QVector3D m_segmentRestRotation; /**< TODO */
    QVector3D m_segmentRestTranslation; /**< TODO */
    QVector3D m_segmentSize; /**< TODO */
    quint32 m_segmentId; /**< the id of this segment */
    quint32 m_segmentParentId; /**< The Id of the parent Segment */
    BrainiacGlobals::SegmentType m_segmentType; /**< TODO */
    bool m_segmentColorInherited; /**< TODO */
    qreal m_segmentColor; /**< TODO */
    QList<BrainiacGlobals::RotTrans> m_segmentRotTransOrder; /**< TODO */
    osg::ref_ptr<osg::MatrixTransform> m_transformNode; /**< the transformation node of the segment. This is the node that is shared between all agent instances @sa AgentManager */


signals:
    /**
     * @brief this signal is emmited whenever anything of this segments geometry, rotation, translation, scale or color changes
     *
     * @fn updated
     */
    void updated();
    void updated(quint32 id);
};

#endif // SEGMENT_H
