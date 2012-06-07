#ifndef SKELETONNODE_H
#define SKELETONNODE_H

#include "core/brainiacglobals.h"
#include "qglscenenode.h"

class Body;
class Channel;

class QGraphicsRotation3D;
class QGraphicsTranslation3D;
class QGraphicsScale3D;

class SkeletonNode : public QGLSceneNode
{
public:
    enum SegmentType {SPHERE=BrainiacGlobals::SPHERE, BOX=BrainiacGlobals::CUBE, NOPRIMITIVE};
    SkeletonNode( SegmentType type, quint32 id, const QString &name, Body *body, QObject *parent = 0 );
    void draw(QGLPainter *painter);
    quint32 getId() const {return m_id;}
    QString getName() const {return m_name;}
    quint32 getParentId() const;
    QVector3D getRestTranslation() const;
    QVector3D getScale() const;
    /**
     * @brief sets the rest translation
     * The rest translation is the pivot point of this Skeleton node
     * To change translation via brain use the channels of this SkeletonNode
     * @param translation the translation of this node
     */
    void setRestTranslation(const QVector3D &translation);
    void setScale(const QVector3D &scale);

protected:
    QGLSceneNode *m_segmentNode; /**< this SkeletonNode´s geometry */
    SegmentType m_type;
    QVector3D m_rotation; //!< Rotation of SkeletonNode
    QVector3D m_translation; //!< Translation of SkeletonNode
    QVector3D m_restRotation; //!< Rest rotation of SkeletonNode
    QVector3D m_restTranslation; //!< Rest translation of SkeletonNode
    QVector3D m_scale; //!< Scale of this SkeletonNode

    QGraphicsRotation3D *m_segRestRot;
    QGraphicsTranslation3D *m_segRestTrans;
    QGraphicsScale3D *m_segScale;

    Channel *m_color; //!< segement´s color Channel
    Channel *m_tx; //!< x translation (input and output)
    Channel *m_ty; //!< y translation (input and output)
    Channel *m_tz; //!< z translation (input and output)
    Channel *m_rx; //!< x rotation (input and output)
    Channel *m_ry; //!< y rotation (input and output)
    Channel *m_rz; //!< z rotation (input and output)

    Body *m_body;
    QString m_name; /**< name of this SkeletonNode */

    quint32 m_id;
};

#endif // SKELETONNODE_H
