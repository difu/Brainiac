#include "output.h"

Output::Output( quint32 id, QString name, QString channel, quint32 editorX, quint32 editorY) : FuzzyBase(FuzzyBase::OUTPUT, id, name, editorX, editorY)
{
    m_channel=channel;
}
