#ifndef FUZZYEDITOR_H
#define FUZZYEDITOR_H

#include "gui/itemeditorwidgetsbase.h"

#include <QWidget>

class AgentManager;
class FuzzyFuzz;
class FuzzyEditorCurveEditor;


namespace Ui {
    class FuzzyEditor;
}

/** \brief  Editor to modify the properties of a fuzz rule
  @sa FuzzyFuzz
**/
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
    FuzzyEditorCurveEditor *m_editor; //!< QGraphicsScene which is the editor of the membership curve

protected slots:
    void modeLinearPressed();
    void modeSinePressed();
    void modeQuadPressed();

    void modeDiracPressed();
    void modeActivatePressed();
    void modeDeactivatePressed();
    void modeTrianglePressed();
    void modeTrapezoidPressed();
    void manualNameChanged();
    void pointChanged();  //!< called when a membership point is moved, NOT finally released

signals:
    void updateBrainEditor(); //!< emitted when editors should update
    void updateGLContent(); //!< emitted when openGL content should update

private:
    Ui::FuzzyEditor *ui;
};

#endif // FUZZYEDITOR_H
