#ifndef SKELETONNODEBOX_H
#define SKELETONNODEBOX_H
#include "skeletonnode.h"

class Channel;

class SkeletonNodeBox : public SkeletonNode
{
    Q_OBJECT
public:
    SkeletonNodeBox(quint32 id, const QString &name, Body *body);
};

#endif // SKELETONNODEBOX_H
