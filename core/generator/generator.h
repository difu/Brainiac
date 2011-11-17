#ifndef GENERATOR_H
#define GENERATOR_H

#include <QList>
#include <QXmlStreamAttributes>
#include <QHash>
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


    /**
     * @brief returns a list of all Locators
     *
     * @return QList<Locator> locators of this Generator
     */
    const QList <Locator *>* getLocations() const;
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

    virtual void loadConfig(QXmlStreamReader *xml)=0;
    virtual void saveConfig(QXmlStreamWriter *xml)=0;
    virtual void generateLocators()=0;
    virtual ~Generator();

protected:
    void readCommonXmlAttribs( const QXmlStreamAttributes &attribs);
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
