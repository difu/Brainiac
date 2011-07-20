#ifndef IDGENERATOR_H
#define IDGENERATOR_H

#include<QMutex>

class IdGenerator
{
public:
    IdGenerator();
    IdGenerator( quint32 id );
    quint32 getNewId();
    void registerId(quint32 id);
protected:
    QMutex m_mutex;
    quint32 m_maxId;
};

#endif // IDGENERATOR_H
