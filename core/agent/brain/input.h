#ifndef INPUT_H
#define INPUT_H

#include "core/agent/brain/fuzzybase.h"

class Channel;

/** \brief  Input fuzz node

        this class describes and manages an input node in the agent´s brain.
        if this node is associated with an input channel, it reads the calculated results out

**/

class Input : public FuzzyBase
{
public:
    Input(quint32 id, Brain *brain, QString name, QString channel, qreal min, qreal max);
    /** \brief  @return the channel name
    **/
    QString getChannelName();

    void calculate();

    /** @brief returns if this is a sound input
      @return true if this is a sound input
    **/
    bool isSoundInput() const { return m_isSoundInput; }

    /** \brief  set the this input as a sound input
      @param isSound true, if this input is a sound Input
    **/
    void setIsSoundInput(bool isSound) { m_isSoundInput=isSound; }

    /** \brief  set the channel of this input
      @param channel the name of the channel assigned to this Input
    **/
    void setChannelName(QString channel);

protected:
    QString m_channelName; //!< name of this input node
    Channel *m_channel; //!< a pointer to the channel, null if no channel is assigned
    bool m_isSoundInput; //!< true, if a sound channel is assigned
};


#endif // INPUT_H
