#ifndef FUZZYEDITOR_H
#define FUZZYEDITOR_H

#include "gui/itemeditorwidgetsbase.h"

#include <QWidget>

class AgentManager;
class BrainiacSlider;
class FuzzyFuzz;

namespace Ui {
    class FuzzyEditor;
}

class FuzzyEditor : public QWidget, public ItemEditorWidgetsBase
{
    Q_OBJECT

public:
    explicit FuzzyEditor(Scene *scene, QWidget *parent = 0);
    ~FuzzyEditor();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::FuzzyEditor *ui;
};

#endif // FUZZYEDITOR_H
