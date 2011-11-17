#ifndef LOCATOR_H
#define LOCATOR_H

#include <QtGlobal>
#include <QVector4D>

class Group;

/**
 * @brief Describes an agent position in the Scene
 *
 */
class Locator
{
public:

/**
 * @brief Constructs a new Locator
 *
 * @param group the group
 * @param x x ordinate
 * @param y y ordinate
 * @param z z ordinate
 * @param w rotation angle around y axis
 */
    Locator(Group *group, qreal x, qreal y, qreal z, qreal w);

    Group* getGroup() const {return m_group;}
    /**
     * @brief returns the xyz position and rotation angle around y axis
     *
     * @return QVector4D * position and rotation
     */
    QVector4D& getLocation();

    /**
     * @brief Destructor
     *
     */

    virtual ~Locator();
protected:
    QVector4D m_position; /**< Position and rotation of agent */
    Group *m_group; /**< Group of this locator */
};

#endif // LOCATOR_H
