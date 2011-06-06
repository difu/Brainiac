#ifndef INPUTEDITOR_H
#define INPUTEDITOR_H

#include "gui/itemeditorwidgetsbase.h"
#include <QWidget>

class AgentManager;
class BrainiacSlider;
class Input;

namespace Ui {
    class InputEditor;
}

class InputEditor : public QWidget, public ItemEditorWidgetsBase
{
    Q_OBJECT

public:
    explicit InputEditor(Scene *scene, QWidget *parent = 0);
    void setInputConfig(AgentManager *manager, quint32 id);
    void updateEditor();
    ~InputEditor();

protected:
    void changeEvent(QEvent *e);
    AgentManager *m_agentManager;
    quint32 m_id;
    BrainiacSlider *m_slider;
    Input *m_input;

protected slots:
    void manualResultChange(qreal value);
//    void nameChange(QString param_name);
//    void channelChange(QString param_name);
//    void minValueChange(QString param_value);
//    void maxValueChange(QString param_value);

signals:
    void updateBrainEditor(); //!< emitted when editors should update
    void updateGLContent(); //!< emitted when openGL content should update

private:
    Ui::InputEditor *ui;
};

#endif // INPUTEDITOR_H
