#ifndef SEGMENT_H
#define SEGMENT_H

#include "core/brainiacglobals.h"
#include <QList>
#include <QString>
#include <QObject>

class Body;
class Channel;
class QVector3D;

class BrainiacColor;

/**
 * @brief
 *
 */
class Segment : public QObject
{
    Q_OBJECT
public:
    enum SegmentType {SPHERE=BrainiacGlobals::SPHERE, CUBE=BrainiacGlobals::CUBE};
/**
 * @brief
 *
 * @param type
 * @param id
 * @param body
 * @param name
 * @param rot
 * @param trans
 * @param parent
 */
    Segment(SegmentType type, quint32 id, Body *body, QString name, QVector3D *rot, QVector3D *trans, Segment *parent=0);
    virtual ~Segment();
    /**
     * @brief
     *
     * @param segment
     */
    void addChild(Segment *segment);
    /**
     * @brief
     *
     * @param id
     */
    void addChildId(quint32 id);
    Body* getBody() { return m_body; }
    Channel* getColor() { return m_color; }
    QList<Segment *> getChildren() { return m_children; }
    SegmentType getType() { return m_type; }
    quint32 getId() { return m_id; }
    QString getName() {return m_name; }
    Segment* getParent();
    quint32 getParentId();
    qreal getSegmentColor() { return m_segmentColor; }
    QVector3D* getRestRotation() { return m_rotation; }
    QVector3D*  getRestTranslation() { return m_translation; }
    Channel* getTransX() { return m_tx; }
    Channel* getTransY() { return m_ty; }
    Channel* getTransZ() { return m_tz; }
    /**
     * @brief
     *
     * @return bool
     */
    bool isColorInherited();
    /**
     * @brief
     *
     * @return bool
     */
    bool isRootSegment();
    /**
     * @brief
     *
     */
    void renderGL();

    /** \brief reset this segment

                    resets this segement´s rotation and translation etc. to its rest pose

    **/
    virtual void reset();
    void setColorInherited(bool inherited);
    void setName(const QString & name);
    void setParent(Segment *segment);
    void setParentId(quint32 id);
    void setSegmentColor(qreal color);

protected:
    virtual void createSegmentChannels();
    virtual void renderGLSegment()=0;
    void init(SegmentType type, quint32 id, Body *body, QString name, QVector3D *restRot, QVector3D *restTrans, Segment *parent=0);
    quint32 m_id;
    Segment *m_parent;
    Body *m_body;
    qreal m_segmentColor; //!< color of Segment
    SegmentType m_type;
    QList<Segment *> m_children;
    QString m_name; /**< name of this Segment */
    QVector3D *m_rotation; //!< Rotation of Segment
    QVector3D *m_translation; //!< Translation of Segment
    Channel *m_color; //!< segement´s color Channel
    Channel *m_tx; //!< x translation (input and output)
    Channel *m_ty; //!< y translation (input and output)
    Channel *m_tz; //!< z translation (input and output)
    Channel *m_rx; //!< x rotation (input and output)
    Channel *m_ry; //!< y rotation (input and output)
    Channel *m_rz; //!< z rotation (input and output)
    Channel *m_scale; //!<  scale factor of this segment (input and output)
};

#endif // SEGMENT_H
