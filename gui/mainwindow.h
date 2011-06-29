#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHash>
#include "editorlabel.h"
#include "core/brainiacglobals.h"
#include "gui/itemeditorwidgetsbase.h"

namespace Ui {
    class MainWindow;
}


class EditorGraphicsView;
class MainWindowLayout;
class Scene;
class SceneEditor;
class QComboBox;
class GroupEditor;
class InputEditor;
class OutputEditor;
class SceneDisplay;

class AgentManager;
class BrainEditor;

/** \brief  Main User Interface

        This class is the main GUI of the application.
        It provides places for e.g. brain editor, animations etc.

**/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Scene *scene, QWidget *parent = 0);
    enum EditMode{ BRAIN, BODY, SCENE };
    EditMode getEditMode() { return m_editMode; }
    void setScene(Scene *scene) { m_scene=scene; } //!< Setter method for the scene to be handled
    void setEditMode(EditMode em);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);
    void createActions();
    void createEditors();
    void createEditModeWidgets();
    void createEditorItemBars();
    void createEditorWidgets();
    void createMenues();
    Scene *m_scene; //!< the scene to be handled
    SceneEditor *m_sceneEditor;
    EditMode m_editMode;
    QComboBox *m_editModeComboBox;
    EditorGraphicsView *m_editorView;
    MainWindowLayout *m_layout;

    QWidget *m_editorItems;
    QWidget *m_brainEditorItems;
    QWidget *m_bodyEditorItems;
    QWidget *m_sceneEditorItems;

    QWidget *m_logicElementEditWidget;

    GroupEditor *m_groupEditor;
    InputEditor *m_inputEditor;
    OutputEditor *m_outputEditor;


    QHash<AgentManager*, BrainEditor*> m_brainEditors;
    AgentManager *m_activeAgentManager; //!< AgentManager that is edited in brain/body editor

    QAction *m_saveAgentAction;
    QAction *m_saveSceneAction;
    QAction *m_runSimulationAction;
    QAction *m_stopSimulationAction;

    QMenu *m_fileMenu;
    QMenu *m_simulationMenu;

    SceneDisplay *m_sceneDisplay; //!< Window that shows the rendered gl scene

protected slots:
    void addAgentManager(AgentManager *agentManager);
    void editModeComboChange(int index);
    void editorNodeClick(ItemEditorWidgetsBase::editMessage msg);
    void refreshBrainEditor();
    void saveAgent();
    void saveScene();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
