#include "locator.h"

Locator::Locator(Group *group, qreal x, qreal y, qreal z, qreal w)
{
    m_position.setW(w);
    m_position.setX(x);
    m_position.setY(y);
    m_position.setZ(z);
    m_group=group;
}

QVector4D& Locator::getLocation()
{
    return m_position;
}

Locator::~Locator()
{
    //delete m_position;
}
