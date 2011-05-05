#ifndef GENERATOR_H
#define GENERATOR_H

#include <QList>

class QVector4D;

/** \brief  Generator base class for agent instances

        Generates agent instances based on differnt types of modes. Also groups are assigned to instances.

**/
class Generator
{
public:
    enum GeneratorType{ POINT, //!< Generates agents at specific points
                        CIRCLE //!< Generates agents within a circle
                      };
    struct locator {
        QVector4D *locator;
        unsigned int groupId;
    };

    Generator( GeneratorType genType);
    QList <locator> getLocations();
    GeneratorType getType() { return m_genType; }

protected:
    GeneratorType m_genType;
    QList <locator> m_locations; //!< x,y,z Position in world space, w rotation angle
};

#endif // GENERATOR_H
