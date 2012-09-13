#ifndef ANDEDITOR_H
#define ANDEDITOR_H

#include <QWidget>
#include "gui/itemeditorwidgetsbase.h"

class AgentManager;
class BrainiacSlider;
class FuzzyAnd;

namespace Ui {
class AndEditor;
}

class AndEditor : public QWidget,ItemEditorWidgetsBase
{
    Q_OBJECT
    
public:
    explicit AndEditor(Scene *scene, QWidget *parent = 0);
    void setInputConfig(AgentManager *manager, quint32 id);
    void updateEditor();
    ~AndEditor();

    AgentManager *m_agentManager;
    quint32 m_id;
    BrainiacSlider *m_slider;
    FuzzyAnd *m_fuzzyAnd;

protected slots:
    void manualResultChange(qreal value);
    void manualNameChange();

signals:
    void updateBrainEditor(); //!< emitted when editors should update
    void updateGLContent(); //!< emitted when openGL content should update
private:
    Ui::AndEditor *ui;
};

#endif // ANDEDITOR_H
