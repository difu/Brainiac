#ifndef OUTPUT_H
#define OUTPUT_H

#include "core/agent/brain/fuzzybase.h"

class Channel;

/** \brief  Output fuzz node

        this class describes and manages an output node in the agent´s brain.
        if this node is associated with an output channel, it writes the calculated results out

**/
class Output : public FuzzyBase
{
public:
    Output(quint32 id, Brain *brain, QString name, QString channel, qreal min, qreal max);
    QString getChannelName();
    void calculate();
    void setChannelName(QString channel);
    bool setResult(qreal result);

protected:
    QString m_channelName;
    Channel *m_channel;
};

#endif // OUTPUT_H
