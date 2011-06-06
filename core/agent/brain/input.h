#ifndef INPUT_H
#define INPUT_H

#include "core/agent/brain/fuzzybase.h"

class Channel;

/** \brief  Output fuzz node

        this class describes and manages an input node in the agent´s brain.
        if this node is associated with an output channel, it reads the calculated results out

**/

class Input : public FuzzyBase
{
public:
    Input(quint32 id, Brain *brain, QString name, QString channel);
    QString getChannelName();
    void calculate();
    void setChannelName(QString channel);
    void setResult(qreal result);

protected:
    QString m_channelName;
    Channel *m_channel;
};


#endif // INPUT_H
