#include "brainiacglobals.h"

QColor BrainiacGlobals::getColorFromBrainiacColorValue(qreal colVal)
{
    qreal val=qBound(0.0,colVal,1.0);
    if( val<0.1) {
        return QColor::fromHsvF(val,10*val,10*val);
    } else if( val<0.9 ) {
        return QColor::fromHsvF(val,1,1);
    } else {
        return QColor::fromHsvF(val,1-(val-0.9)*10,1);
    }
}
