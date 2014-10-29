#ifndef SCENEPROXYGENERATOR_H
#define SCENEPROXYGENERATOR_H

#include <QObject>

class SceneDisplay;

class SceneProxyGenerator : public QObject
{
    Q_OBJECT
public:
    explicit SceneProxyGenerator(QObject *parent = 0);
    SceneDisplay *getSceneDisplay() const;
signals:

public slots:
    void scheduleUpdate();
protected:
    virtual void timerEvent(QTimerEvent *event);
private:
    bool m_dirty;
};

#endif // SCENEPROXYGENERATOR_H
