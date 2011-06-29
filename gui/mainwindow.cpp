#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainwindowlayout.h"
#include "gui/braineditor/braineditor.h"
#include "gui/braineditor/inputeditor.h"
#include "gui/braineditor/outputeditor.h"
#include "gui/editorgraphicsview.h"
#include "gui/sceneeditor/sceneeditor.h"
#include "gui/sceneeditor/groupeditor.h"
#include "gui/scenedisplay.h"
#include "editorlabel.h"
#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"
#include "core/scene.h"
#include "core/simulation.h"
#include "core/group/group.h"
#include <QDebug>
#include <QComboBox>
#include <QDialog>
#include <QLabel>

MainWindow::MainWindow(Scene *scene, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    m_editMode=MainWindow::BODY;
    m_scene=scene;

    ui->setupUi(this);

    m_layout=new MainWindowLayout();
    QWidget *widget = new QWidget;

    createEditorItemBars();
    createEditors();
    createEditorWidgets();
    createEditModeWidgets();
    createActions();
    createMenues();

    setEditMode(MainWindow::SCENE);

    //qDebug() << __PRETTY_FUNCTION__;

    widget->setLayout(m_layout);
    setCentralWidget(widget);

    m_sceneDisplay=new SceneDisplay(this->m_scene,m_scene->getCameras().first());
    connect(m_outputEditor,SIGNAL(updateGLContent()),m_sceneDisplay,SLOT(updateGL()));

    // When a frame has been calculated update display
    connect(m_scene->getSimulation(),SIGNAL(frameDone()),m_sceneDisplay,SLOT(updateGL()),Qt::DirectConnection);

    // Assign each BrainEditor the first of its Agentmanagers Agent as the to be edited agentbrain
    QHashIterator<AgentManager*, BrainEditor*> i(m_brainEditors);
    while (i.hasNext()) {
        i.next();
        if(i.key()->getGroup()->getAgents().count()>0) {
            i.value()->setSelectedAgent(i.key()->getGroup()->getAgents().first());
        } else {
            i.value()->setSelectedAgent(i.key()->getMasterAgent());
        }
    }
}

void MainWindow::addAgentManager(AgentManager *agentManager)
{
    BrainEditor *editor=new BrainEditor(m_scene,agentManager);
    m_brainEditors.insert(agentManager,editor);

}

void MainWindow::createActions()
{
    // File Menu Actions
    m_saveAgentAction=new QAction(tr("Save Agent"),this);
    connect(m_saveAgentAction,SIGNAL(triggered()),this,SLOT(saveAgent()));
    m_saveSceneAction = new QAction(tr("Save Scene"),this);
    connect(m_saveSceneAction,SIGNAL(triggered()),this,SLOT(saveScene()));

    // Simulation Menu Actions
    m_runSimulationAction=new QAction(tr("Run"), this);
    connect(m_runSimulationAction,SIGNAL(triggered()),m_scene->getSimulation(),SLOT(startSimulation()));
    m_stopSimulationAction=new QAction(tr("Stop"),this);
    connect(m_stopSimulationAction,SIGNAL(triggered()),m_scene->getSimulation(),SLOT(stopSimulation()));
}

void MainWindow::createEditors()
{
    m_editorView=new EditorGraphicsView();
    m_layout->addWidget(m_editorView,MainWindowLayout::Center);

    m_sceneEditor=new SceneEditor(m_scene);
    m_activeAgentManager=0;
    foreach(Group *grp,m_scene->getGroups()) {
        AgentManager *mngr=grp->getAgentManager();
        addAgentManager(mngr);
        m_activeAgentManager=mngr;
    }

}

void MainWindow::createEditModeWidgets()
{
    m_editModeComboBox=new QComboBox();
    m_editModeComboBox->setMinimumWidth(100);
    m_editModeComboBox->addItem("Brain", QVariant(MainWindow::BRAIN));
    m_editModeComboBox->addItem("Body", QVariant(MainWindow::BODY));
    m_editModeComboBox->addItem("Scene", QVariant(MainWindow::SCENE));
    QHBoxLayout *northLayout = new QHBoxLayout();
    QDialog *northDialog = new QDialog();
    northLayout->addStretch();

    northLayout->addWidget(m_editModeComboBox);
    northDialog->setLayout(northLayout);
    m_layout->addWidget(northDialog,MainWindowLayout::North);
    connect(m_editModeComboBox,SIGNAL(activated(int)),SLOT(editModeComboChange(int)));
}

void MainWindow::createEditorItemBars()
{
    m_editorItems=new QWidget();
    m_editorItems->setFixedWidth(80);

    QVBoxLayout *myVBoxlayout=new QVBoxLayout();
    m_brainEditorItems = new QWidget(m_editorItems);
    EditorLabel *editLabel=new EditorLabel(BrainiacGlobals::AND);
    myVBoxlayout->addWidget(editLabel);
    myVBoxlayout->addWidget(new QLabel("AND"));
    editLabel=new EditorLabel(BrainiacGlobals::OR);
    myVBoxlayout->addWidget(editLabel);
    myVBoxlayout->addWidget(new QLabel("OR"));
    editLabel=new EditorLabel(BrainiacGlobals::FUZZ);
    myVBoxlayout->addWidget(editLabel);
    myVBoxlayout->addWidget(new QLabel("FUZZ"));
    editLabel=new EditorLabel(BrainiacGlobals::DEFUZZ);
    myVBoxlayout->addWidget(editLabel);
    myVBoxlayout->addWidget(new QLabel("DEFUZZ"));
    editLabel=new EditorLabel(BrainiacGlobals::INPUT);
    myVBoxlayout->addWidget(editLabel);
    myVBoxlayout->addWidget(new QLabel("INPUT"));
    editLabel=new EditorLabel(BrainiacGlobals::OUTPUT);
    myVBoxlayout->addWidget(editLabel);
    myVBoxlayout->addWidget(new QLabel("OUTPUT"));
    editLabel=new EditorLabel(BrainiacGlobals::TIMER);
    myVBoxlayout->addWidget(editLabel);
    myVBoxlayout->addWidget(new QLabel("TIMER"));
    editLabel=new EditorLabel(BrainiacGlobals::NOISE);
    myVBoxlayout->addWidget(editLabel);
    myVBoxlayout->addWidget(new QLabel("NOISE"));
    myVBoxlayout->addStretch();
    m_brainEditorItems->setLayout(myVBoxlayout);

    myVBoxlayout=new QVBoxLayout();
    m_bodyEditorItems = new QWidget(m_editorItems);
    myVBoxlayout->addWidget(new QLabel("CUBE"));
    myVBoxlayout->addWidget(new QLabel("Sphere"));
    myVBoxlayout->addStretch();
    m_bodyEditorItems->setLayout(myVBoxlayout);

    myVBoxlayout=new QVBoxLayout();
    m_sceneEditorItems = new QWidget(m_editorItems);
    editLabel=new EditorLabel(BrainiacGlobals::AGENT);
    myVBoxlayout->addWidget(editLabel);
    myVBoxlayout->addWidget(new QLabel("Agent"));
    editLabel=new EditorLabel(BrainiacGlobals::GROUP);
    myVBoxlayout->addWidget(editLabel);
    myVBoxlayout->addWidget(new QLabel("Group"));
    myVBoxlayout->addStretch();
    m_sceneEditorItems->setLayout(myVBoxlayout);

    m_layout->addWidget(m_editorItems,MainWindowLayout::West);

}

void MainWindow::createEditorWidgets()
{
    m_logicElementEditWidget=new QWidget();
    m_logicElementEditWidget->setFixedHeight(130);

    m_groupEditor=new GroupEditor(m_scene, m_logicElementEditWidget);
    m_groupEditor->setVisible(false);
    connect(m_sceneEditor, SIGNAL(itemClicked(ItemEditorWidgetsBase::editMessage)),this,SLOT(editorNodeClick(ItemEditorWidgetsBase::editMessage)));

    m_inputEditor=new InputEditor(m_scene, m_logicElementEditWidget);
    m_inputEditor->setVisible(false);

    m_outputEditor=new OutputEditor(m_scene, m_logicElementEditWidget);
    m_outputEditor->setVisible(false);
    foreach(BrainEditor *brainEditor,m_brainEditors) {
        // This signal activates editor in South region
        connect(brainEditor, SIGNAL(itemClicked(ItemEditorWidgetsBase::editMessage)),this,SLOT(editorNodeClick(ItemEditorWidgetsBase::editMessage)));
        // When a frame has been calculated update the braineditors to display the new values
        connect(m_scene->getSimulation(),SIGNAL(frameDone()),brainEditor,SLOT(update()),Qt::DirectConnection);
    }
    //
    connect(m_inputEditor, SIGNAL(updateBrainEditor()),this,SLOT(refreshBrainEditor()));
    connect(m_outputEditor,SIGNAL(updateBrainEditor()),this,SLOT(refreshBrainEditor()));
    //connect(m_outputEditor,SIGNAL(updateGLContent()),m_sceneDisplay,SLOT(updateGL()));
    m_layout->addWidget(m_logicElementEditWidget,MainWindowLayout::South);
}


/** \brief creates all menues of the main window
**/
void MainWindow::createMenues()
{
    m_fileMenu=menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_saveAgentAction);
    m_fileMenu->addAction(m_saveSceneAction);

    m_simulationMenu=menuBar()->addMenu(tr("&Simulation"));
    m_simulationMenu->addAction(m_runSimulationAction);
}

void MainWindow::editModeComboChange(int index)
{
    QVariant em=m_editModeComboBox->itemData(index);
    setEditMode((EditMode)em.toInt());
}

/** \brief called when a node in an editor (Brain, Body, Scene) is clicked

                This slot is called when any node in any editor is clicked.
                It shows the depending node editor and updates the editor with information of the
                clicked item

**/
void MainWindow::editorNodeClick(ItemEditorWidgetsBase::editMessage msg)
{
    m_groupEditor->setVisible(msg.type==BrainiacGlobals::GROUP);
    m_inputEditor->setVisible(msg.type==BrainiacGlobals::INPUT);
    m_outputEditor->setVisible(msg.type==BrainiacGlobals::OUTPUT);
    Group *grp;
    AgentManager *mgr;
    switch(msg.type) {
    case BrainiacGlobals::GROUP:
        grp=(Group *)msg.object;
        m_groupEditor->setGroup(grp);
        m_activeAgentManager=grp->getAgentManager();
        break;
    case BrainiacGlobals::OUTPUT:
        mgr=(AgentManager *)msg.object;
        m_outputEditor->setOutputConfig(mgr,msg.id);
        break;
    case BrainiacGlobals::INPUT:
        mgr=(AgentManager *)msg.object;
        m_inputEditor->setInputConfig(mgr,msg.id);
        break;

    default:
    ;
    }
}

void MainWindow::refreshBrainEditor()
{
    m_editorView->scene()->update();
}

/** \brief saves the selected agent

**/
void MainWindow::saveAgent()
{
    if(m_activeAgentManager) {
        m_brainEditors.value(m_activeAgentManager)->updateItemLocations();
        m_activeAgentManager->saveConfig();
    }
}

/** \brief saves the scene

**/
void MainWindow::saveScene()
{
    m_scene->saveConfig();
}

/** \brief changes the mainwindow´s edit mode

                when called the sidebar´s items are changed and the corresponding editor shows up
                in the central area

**/
void MainWindow::setEditMode(EditMode em)
{
    m_editMode=em;
    m_brainEditorItems->setVisible(em==MainWindow::BRAIN);
    m_bodyEditorItems->setVisible(em==MainWindow::BODY);
    m_sceneEditorItems->setVisible(em==MainWindow::SCENE);

    m_groupEditor->setVisible(false);
    m_outputEditor->setVisible(false);

    int index=m_editModeComboBox->findData(QVariant(em));
    m_editModeComboBox->setCurrentIndex(index);
    switch( em ) {
    case MainWindow::SCENE:
        m_editorView->setScene(m_sceneEditor);
        break;
    case MainWindow::BODY:
    case MainWindow::BRAIN:
        if(m_brainEditors.contains(m_activeAgentManager))
            m_editorView->setScene(m_brainEditors.value(m_activeAgentManager));
        else {
            m_editorView->setScene(0);
        }
        break;
    }
    m_editorView->centerOn(m_editorView->scene()->width()/2,m_editorView->scene()->height()/2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
