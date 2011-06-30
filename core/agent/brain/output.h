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
    enum DefuzzMode {AVERAGE, MAX};
    Output(quint32 id, Brain *brain, QString name, QString channel, qreal min, qreal max);
    QString getChannelName();
    void calculate();
    Output::DefuzzMode getDefuzzMode();
    void setChannelName(QString channel);
    void setDefuzzMode( DefuzzMode mode);
    bool setResult(qreal result,bool emitChange=true);

protected:
    QString m_channelName;
    Channel *m_channel;
    DefuzzMode m_defuzzMode;
};

#endif // OUTPUT_H
