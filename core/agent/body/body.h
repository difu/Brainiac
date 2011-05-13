#ifndef BODY_H
#define BODY_H

class Agent;

class Body
{
public:
    Body(Agent *agent);
    Agent* getAgent();
protected:
    Agent *m_agent;
};

#endif // BODY_H
