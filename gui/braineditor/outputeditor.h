#ifndef OUTPUTEDITOR_H
#define OUTPUTEDITOR_H

#include "gui/itemeditorwidgetsbase.h"

#include <QWidget>

class AgentManager;
class BrainiacSlider;
class Output;

namespace Ui {
    class OutputEditor;
}

class OutputEditor : public QWidget, public ItemEditorWidgetsBase
{
    Q_OBJECT

public:
    explicit OutputEditor(Scene *scene, QWidget *parent = 0);
    void setOutputConfig(AgentManager *manager, quint32 id);
    void updateEditor();
    ~OutputEditor();

protected:
    void changeEvent(QEvent *e);
    AgentManager *m_agentManager;
    quint32 m_id;
    BrainiacSlider *m_slider;
    Output *m_output;

protected slots:
    void manualResultChange(qreal param_value);
//    void nameChange(QString param_name);
//    void channelChange(QString param_name);
//    void minValueChange(QString param_value);
//    void maxValueChange(QString param_value);

signals:
    void updateBrainEditor(); //!< emitted when editors should update

private:
    Ui::OutputEditor *ui;
};

#endif // OUTPUTEDITOR_H
