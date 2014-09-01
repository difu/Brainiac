#ifndef FUZZYCHANNEL_H
#define FUZZYCHANNEL_H

#include "core/agent/brain/fuzzybase.h"
#include "core/agent/brain/fuzzychannel.h"

class Channel;

class FuzzyChannel : public FuzzyBase
{
    Q_OBJECT
public:
    FuzzyChannel(LogicType logicType, quint32 id, Brain *brain, const QString &name, const QString& channelName, qreal min, qreal max);

    /** \brief  @return the channel name
     *  a look up is made, if the channel is already registered in the agents brain. If so, the name is read out of the brain of the agent.
     * If the look up fails, its internal (chached) value is returned instead.
     * @todo ugly code... rewrite. Use only one name instance, implement proper agent copying/cloning
     *
    **/
    QString getChannelName() const;

    /** \brief  set the channel of this fuzzy
      @param channel the name of the channel
    **/
    virtual void setChannelName(const QString &channelName);

protected:
    QString m_channelName; //!< name of this channel
    Channel *m_channel; //!< a pointer to the channel, null if no channel is assigned
protected slots:
    void channelDelete();
};

#endif // FUZZYCHANNEL_H
