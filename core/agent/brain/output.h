#ifndef OUTPUT_H
#define OUTPUT_H

#include "core/agent/brain/fuzzybase.h"

class Output : public FuzzyBase
{
public:
    Output(quint32 id, QString name, QString channel, quint32 editorX, quint32 editorY);

protected:
    QString m_channel;
};

#endif // OUTPUT_H
