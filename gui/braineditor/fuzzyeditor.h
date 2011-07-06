#ifndef FUZZYEDITOR_H
#define FUZZYEDITOR_H

#include "gui/itemeditorwidgetsbase.h"

#include <QWidget>

class AgentManager;
class FuzzyFuzz;


namespace Ui {
    class FuzzyEditor;
}

class FuzzyEditor : public QWidget, public ItemEditorWidgetsBase
{
    Q_OBJECT

public:
    explicit FuzzyEditor(Scene *scene, QWidget *parent = 0);
    void setFuzzConfig(AgentManager *manager, quint32 id);
    void updateEditor();
    ~FuzzyEditor();

protected:
    void changeEvent(QEvent *e);
    AgentManager *m_agentManager;
    quint32 m_id;
    FuzzyFuzz *m_fuzz;

signals:
    void updateBrainEditor(); //!< emitted when editors should update
    void updateGLContent(); //!< emitted when openGL content should update

private:
    Ui::FuzzyEditor *ui;
};

#endif // FUZZYEDITOR_H
