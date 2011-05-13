#include "brainiaccolor.h"

BrainiacColor::BrainiacColor()
{
    setColor(0);
}

BrainiacColor::BrainiacColor(qreal color)
{
    setColor(color);
}

void BrainiacColor::setColor(qreal color)
{
    m_color=qBound((qreal)0,color,(qreal)1);
}

qreal BrainiacColor::getColor()
{
    return m_color;
}

QColor BrainiacColor::getQColor()
{
    return QColor::fromHsvF(m_color,1,1);
}
