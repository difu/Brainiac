#ifndef MOTIONTREEEDITORGUI_H
#define MOTIONTREEEDITORGUI_H

#include <QWidget>
#include "gui/itemeditorwidgetsbase.h"

class AgentManager;

namespace Ui {
class MotionTreeEditorGui;
}

class MotionTreeEditorGui : public QWidget,ItemEditorWidgetsBase
{
    Q_OBJECT

public:
    explicit MotionTreeEditorGui(Scene *scene,QWidget *parent = 0);
    void setAgentManager( AgentManager *agentManager);
    ~MotionTreeEditorGui();
protected:
    void setActiveTree(quint32 id);
    AgentManager *m_agentManager;

protected slots:
    void motionTreeButtonPressed();
signals:
    void motionTreeChange( quint32 id);
private:
    Ui::MotionTreeEditorGui *ui;
};

#endif // MOTIONTREEEDITORGUI_H
