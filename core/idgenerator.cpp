#include "idgenerator.h"

IdGenerator::IdGenerator():m_maxId(0)
{
}

IdGenerator::IdGenerator(quint32 id):m_maxId(id)
{
}

quint32 IdGenerator::getNewId() {
    QMutexLocker locker(&m_mutex);
    m_maxId++;
    return m_maxId;
}

void IdGenerator::registerId(quint32 id)
{
    QMutexLocker locker(&m_mutex);
    if(id>m_maxId) {
        m_maxId=id;
    }
}
