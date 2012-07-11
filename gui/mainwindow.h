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
class ActionEditor;

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

    // File menu actions
    QAction *m_saveAgentAction;
    QAction *m_saveSceneAction;
    QAction *m_loadAnimationsAction;
    QAction *m_saveAnimationAction;
    QAction *m_loadSkeletonAction;

    // Edit menu actions
    QAction *m_showActionEditorAction;
    QAction *m_editSymetricAction;

    // Simulation menu actions
    QAction *m_runSimulationAction;
    QAction *m_resetSimulationAction;
    QAction *m_stopSimulationAction;

    // View menu actions
    QAction *m_viewSoundEmmisions;
    QAction *m_viewAgentInfos;

    QMenu *m_fileMenu;
    QMenu *m_editMenu;
    QMenu *m_simulationMenu;
    QMenu *m_viewMenu;

    SceneDisplay *m_sceneDisplay; //!< Window that shows the rendered gl scene
    BodyDisplay *m_bodyDisplay; //!< Window that shows the body of selected agent
    ActionEditor *m_actionEditor;  /**< Window of ActionEditor */

public slots:
    void statusBarMessageChange( const QString & message );

protected slots:
    void addAgentManager(AgentManager *agentManager);
    void editModeComboChange(int index);
    void editorNodeClick(ItemEditorWidgetsBase::editMessage msg);
    /**
     * @brief called, when any item is clicked (selected or deselected)
     *
     * this is used to determine the import skeleton menu state
     * @sa loadSkeleton()
     * @sa m_loadSkeletonAction
     * @fn sceneEditorItemClicked
     */
    void sceneEditorItemClicked();
    void refreshBrainEditor();
    void loadAnimation();
    void saveAnimation();
    void loadSkeleton();
    void saveAgent();
    void saveScene();
    void showActionEditor();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
