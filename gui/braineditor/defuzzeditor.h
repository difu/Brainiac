#ifndef DEFUZZEDITOR_H
#define DEFUZZEDITOR_H

#include "gui/itemeditorwidgetsbase.h"
#include <QWidget>


class AgentManager;
class BrainiacSlider;
class FuzzyDefuzz;

namespace Ui {
    class DefuzzEditor;
}

class DefuzzEditor : public QWidget, public ItemEditorWidgetsBase
{
    Q_OBJECT

public:
    explicit DefuzzEditor(Scene *scene, QWidget *parent = 0);
    void setDefuzzConfig(AgentManager *manager, quint32 id);
    void updateEditor();
    ~DefuzzEditor();

protected:
    void changeEvent(QEvent *e);
    AgentManager *m_agentManager;
    quint32 m_id;
    FuzzyDefuzz *m_defuzz;
    BrainiacSlider *m_fuzzValSlider;
    BrainiacSlider *m_fuzzResultSlider;

protected slots:
    void manualDefuzzValueChange(qreal value);
    void manualNameChange();
    void manualIsElseChange(bool isChecked);
signals:
    void updateBrainEditor(); //!< emitted when editors should update
    void updateGLContent(); //!< emitted when openGL content should update

private:
    Ui::DefuzzEditor *ui;
};

#endif // DEFUZZEDITOR_H
