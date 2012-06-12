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
    Q_OBJECT
public:
    enum SegmentType {SPHERE=BrainiacGlobals::SPHERE, BOX=BrainiacGlobals::CUBE, NOPRIMITIVE};
    SkeletonNode( SegmentType type, quint32 id, const QString &name, Body *body, QObject *parent = 0 );
    void draw(QGLPainter *painter);
    /**
     * @brief returns the color of this SkeletonNode
     * This is the default color of this node.
     * @return Channel
     */
    qreal getInitColor() const { return m_initColor; }
    /**
     * @brief returns color Channel of this SkeletonNode
     * the value is the actual color
     * @return Channel
     */
    Channel* getColor() { return m_color; }
    bool getColorInherited() const;
    quint32 getId() const {return m_id;}
    QString getName() const {return this->objectName();}
    quint32 getParentId() const;
    QVector3D getRestRotation() const;
    QVector3D getRestTranslation() const;
    QVector3D getRotation() const {return m_rotation;}
    QVector3D getTranslation() const {return m_translation;}
    QVector3D getScale() const;
    /**
     * @brief sets the color of this SkeletonNode
     * this is the default color
     * @param color the color of this SkeletonNode
     */
    void setColor(qreal color);
    void setColorInherited(bool inherited);
    /**
     * @brief sets the rest translation
     * The rest translation is the pivot point of this Skeleton node
     * To change translation via brain use the channels of this SkeletonNode
     * @param translation the translation of this node
     */
    void setRestTranslation(const QVector3D &translation);
    /**
     * @brief sets the rest rotation
     * The rest rotation is the rotation around the pivot point of this Skeleton node
     * To change rotation via brain use the channels of this SkeletonNode
     * @param rotation the rotaion of this node
     */
    void setRestRotation(const QVector3D &rotation);

    void setScale(const QVector3D &scale);

    /**
     * @brief sets translation of the geometry relative to pivot point
     *
     * @param translation the translation
     * @sa setRotation();
     */
    void setTranslation(const QVector3D &translation);
    /**
     * @brief sets rotation of the geometry
     *
     * @param translation the translation
     * @sa setTranslation();
     */
    void setRotation(const QVector3D &rotation);


protected:
    virtual void createChannels();
    QGLSceneNode *m_segmentNode; /**< this SkeletonNode´s geometry */
    SegmentType m_type;
    QVector3D m_rotation; //!< Rotation of SkeletonNode
    QVector3D m_translation; //!< Translation of SkeletonNode
    QVector3D m_restRotation; //!< Rest rotation of SkeletonNode
    QVector3D m_restTranslation; //!< Rest translation of SkeletonNode
    QVector3D m_scale; //!< Scale of this SkeletonNode

    QGraphicsRotation3D *m_segRestRotX;
    QGraphicsRotation3D *m_segRestRotY;
    QGraphicsRotation3D *m_segRestRotZ;

    QGraphicsRotation3D *m_segRotX;
    QGraphicsRotation3D *m_segRotY;
    QGraphicsRotation3D *m_segRotZ;

    QGraphicsTranslation3D *m_segRestTrans;
    QGraphicsScale3D *m_segScale;

    Channel *m_color; //!< segement´s color Channel
    Channel *m_channelTx; //!< x translation (input and output)
    Channel *m_channelTy; //!< y translation (input and output)
    Channel *m_channelTz; //!< z translation (input and output)
    Channel *m_channelRx; //!< x rotation (input and output)
    Channel *m_channelRy; //!< y rotation (input and output)
    Channel *m_channelRz; //!< z rotation (input and output)

    Body *m_body;
    qreal m_initColor;

    quint32 m_id;

protected slots:
    void channelRxChanged(qreal value);
    void channelRyChanged(qreal value);
    void channelRzChanged(qreal value);
};

#endif // SKELETONNODE_H
