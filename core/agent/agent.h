#ifndef AGENT_H
#define AGENT_H

#include <QObject>
#include <QHash>
#include <QString>

class Body;
class Channel;

class Agent : public QObject
{
    Q_OBJECT
public:
    explicit Agent(QObject *parent = 0);
    bool addInputChannel(Channel* channel, QString name);
    bool addOutputChannel(Channel* channel, QString name);
    void deleteChannel(Channel* channel);
    Body *getBody();
    virtual ~Agent();

protected:
    Body *m_body; //!< the agent´s body
    QHash<QString, Channel *> m_inputs; //!< List of all input channels of this agent
    QHash<QString, Channel *> m_outputs; //!< List of all output channels of this agent
    void createChannels();
    Channel *m_tx; //!< x translation rate of agent (input and output)
    Channel *m_ty; //!< y translation rate of agent (input and output)
    Channel *m_tz; //!< z translation rate of agent (input and output)

signals:

public slots:

};

#endif // AGENT_H
