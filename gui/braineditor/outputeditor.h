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
    void manualResultChange(qreal value);
    void manualNameChange();
    void manualChannelChange();
    void manualMinMaxValueChange();
    void manualDefuzzMaxPressed();
    void manualDefuzzAvgPressed();

signals:
    void updateBrainEditor(); //!< emitted when editors should update
    void updateGLContent(); //!< emitted when openGL content should update

private:
    Ui::OutputEditor *ui;
};

#endif // OUTPUTEDITOR_H
