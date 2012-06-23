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
class DefuzzEditor;
class FuzzyEditor;
class NoiseEditor;
class SceneDisplay;
class BodyDisplay;
class SegmentEditor;

class AgentManager;
class BrainEditor;
class BodyEditor;

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

    /**
     * @brief called before colsing the mainwindow
     *
     * @param ev the close event
     */
    void closeEvent(QCloseEvent *ev);
    void createActions();
    void createEditors();
    void createEditModeWidgets();
    void createEditorItemBars();
    void createEditorWidgets();
    void createMenues();

    /**
     * @brief reads the window´s geometry
     *
     */
    void readSettings();

    /**
     * @brief saves the window´s geometry
     *
     */
    void writeSettings();

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
    DefuzzEditor *m_defuzzEditor;
    FuzzyEditor *m_fuzzyEditor;
    NoiseEditor *m_noiseEditor;
    SegmentEditor *m_segmentEditor;


    QHash<AgentManager*, BrainEditor*> m_brainEditors;  //!< all BrainEditors.
    QHash<AgentManager*, BodyEditor*> m_bodyEditors;  //!< all BrainEditors
    AgentManager *m_activeAgentManager; //!< AgentManager that is edited in brain/body editor

    QAction *m_saveAgentAction;
    QAction *m_saveSceneAction;

    QAction *m_runSimulationAction;
    QAction *m_resetSimulationAction;
    QAction *m_stopSimulationAction;

    QAction *m_viewSoundEmmisions;
    QAction *m_viewAgentInfos;

    QMenu *m_fileMenu;
    QMenu *m_simulationMenu;
    QMenu *m_viewMenu;

    SceneDisplay *m_sceneDisplay; //!< Window that shows the rendered gl scene
    BodyDisplay *m_bodyDisplay; //!< Window that shows the body of selected agent


public slots:
    void statusBarMessageChange( const QString & message );

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
