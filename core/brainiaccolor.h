#ifndef BRAINIACCOLOR_H
#define BRAINIACCOLOR_H

#include<QtCore>
#include<QColor>

class BrainiacColor
{
public:
    BrainiacColor();
    BrainiacColor(qreal color);
    void setColor(qreal color);
    qreal getColor();
    QColor getQColor();

protected:
    qreal m_color;
};

#endif // BRAINIACCOLOR_H
