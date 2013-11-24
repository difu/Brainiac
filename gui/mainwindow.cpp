// Brainiac is a free and open source tool for the creation of crowd simulation

// Copyright (C) 2012  Dirk Fuchs dirkfux@googlemail.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainwindowlayout.h"
#include "gui/bodyeditor/bodyeditor.h"
#include "gui/bodyeditor/segmenteditor.h"
#include "gui/braineditor/braineditor.h"
#include "gui/braineditor/fuzzyeditor.h"
#include "gui/braineditor/defuzzeditor.h"
#include "gui/braineditor/inputeditor.h"
#include "gui/braineditor/outputeditor.h"
#include "gui/braineditor/noiseeditor.h"
#include "gui/editorgraphicsview.h"
#include "gui/sceneeditor/sceneeditor.h"
#include "gui/sceneeditor/groupeditor.h"
#include "gui/editoritem.h"
#include "gui/Animation/actioneditor.h"
#include "gui/bodyeditor/bodydisplay_.h"
#include "gui/scenedisplay_.h"
#include "editorlabel.h"
#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"
#include "core/scene.h"
#include "core/simulation.h"
#include "core/group/group.h"
#include "core/agent/body/animation/animation.h"
#include <QDebug>
#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(Scene *scene, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    osg::setNotifyLevel( osg::FATAL );
    m_editMode=MainWindow::BODY;
    m_scene=scene;

    ui->setupUi(this);

    readSettings();

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

    //m_bodyDisplay=new BodyDisplay(this->m_scene);

    m_bodyDisplayOSG=new BodyDisplay_();
    m_sceneDisplayOSG=new SceneDisplay_(this->m_scene);

    //m_sceneDisplay=new SceneDisplay(this->m_scene,m_scene->getCameras().first());
    //connect(m_outputEditor,SIGNAL(updateGLContent()),m_sceneDisplay,SLOT(update()));
    //connect(m_segmentEditor,SIGNAL(updateGLContent()),m_bodyDisplay,SLOT(update()));

    // When a frame has been calculated update display
//    connect(m_scene->getSimulation(),SIGNAL(frameDone()),m_sceneDisplay,SLOT(update()),Qt::DirectConnection);

    connect(m_sceneEditor,SIGNAL(selectionChanged()),this,SLOT(sceneEditorItemClicked()));

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
    // This signal activates editor in South region
    connect(editor, SIGNAL(itemClicked(ItemEditorWidgetsBase::editMessage)),this,SLOT(editorNodeClick(ItemEditorWidgetsBase::editMessage)));
    // When a frame has been calculated update the braineditors to display the new values
    connect(m_scene->getSimulation(),SIGNAL(frameDone()),editor,SLOT(update()),Qt::DirectConnection);
    // Display statusbar messages
    connect(editor, SIGNAL(statusBarMessageChanged(QString)),this,SLOT(statusBarMessageChange(QString)));
    BodyEditor *bodyEditor=new BodyEditor(m_scene,agentManager);
    m_bodyEditors.insert(agentManager,bodyEditor);
    connect(bodyEditor,SIGNAL(itemClicked(ItemEditorWidgetsBase::editMessage)),this,SLOT(editorNodeClick(ItemEditorWidgetsBase::editMessage)));

}

void MainWindow::closeEvent(QCloseEvent *ev)
{
    //! \bug Crashes since QT5
//    if(m_bodyDisplayOSG) {
//        m_bodyDisplayOSG->hide();
//        m_bodyDisplayOSG->deleteLater();
//        m_bodyDisplayOSG=0;
//    }
//    if(m_sceneDisplayOSG) {
//        m_sceneDisplayOSG->hide();
//        m_sceneDisplayOSG->deleteLater();
//        m_sceneDisplayOSG=0;
//    }
//    if(m_actionEditor) {
//        m_actionEditor->hide();
//        m_actionEditor->deleteLater();
//        m_actionEditor=0;
//    }
    writeSettings();
    ev->accept();
}

void MainWindow::createActions()
{
    // File Menu Actions
    m_saveAgentAction=new QAction(tr("Save Agent"),this);
    connect(m_saveAgentAction,SIGNAL(triggered()),this,SLOT(saveAgent()));
    m_saveSceneAction = new QAction(tr("Save Scene"),this);
    connect(m_saveSceneAction,SIGNAL(triggered()),this,SLOT(saveScene()));
    m_loadAnimationsAction =new QAction(tr("Load Animations"),this);
    connect(m_loadAnimationsAction,SIGNAL(triggered()),this,SLOT(loadAnimation()));
    m_saveAnimationAction=new QAction(tr("Save Animation"),this);
    connect(m_saveAnimationAction,SIGNAL(triggered()),this,SLOT(saveAnimation()));
    m_loadSkeletonAction=new QAction(tr("Load Skeleton"),this);
    connect(m_loadSkeletonAction,SIGNAL(triggered()),this,SLOT(loadSkeleton()));

    // Edit Menu Actions
    m_showActionEditorAction=new QAction(tr("Action Editor"),this);
    connect(m_showActionEditorAction,SIGNAL(triggered()),this,SLOT(showActionEditor()));
    m_editSymetricAction=new QAction(tr("Symetric"),this);
    m_editSymetricAction->setCheckable(true);
    m_editSymetricAction->setChecked(m_segmentEditor->editSymetric());
    connect(m_editSymetricAction,SIGNAL(toggled(bool)),m_segmentEditor,SLOT(editSymetric(bool)));

    // Simulation Menu Actions
    m_runSimulationAction=new QAction(tr("Run"), this);
    connect(m_runSimulationAction,SIGNAL(triggered()),m_scene->getSimulation(),SLOT(startSimulation()));
    m_stopSimulationAction=new QAction(tr("Stop"),this);
    connect(m_stopSimulationAction,SIGNAL(triggered()),m_scene->getSimulation(),SLOT(stopSimulation()));
    m_resetSimulationAction=new QAction(tr("Reset"), this);
    connect(m_resetSimulationAction,SIGNAL(triggered()),m_scene->getSimulation(),SLOT(resetSimulation()));

    //View Menu Actions
    m_viewSoundEmmisions=new QAction(tr("Sound Emmissions"),this);
    m_viewSoundEmmisions->setCheckable(true);
    m_viewAgentInfos=new QAction(tr("View Agent Infos"),this);
    m_viewAgentInfos->setCheckable(true);
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
    m_actionEditor=new ActionEditor(m_scene,this);
}

void MainWindow::createEditModeWidgets()
{
    m_editModeComboBox=new QComboBox();
    m_editModeComboBox->setMinimumWidth(100);
    m_editModeComboBox->addItem("Brain", QVariant(MainWindow::BRAIN));
    m_editModeComboBox->addItem("Body", QVariant(MainWindow::BODY));
    m_editModeComboBox->addItem("Scene", QVariant(MainWindow::SCENE));
    m_editModeComboBox->addItem("Motion", QVariant(MainWindow::MOTION));
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
    myVBoxlayout->addWidget(new QLabel("SPHERE"));
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

    m_defuzzEditor=new DefuzzEditor(m_scene,m_logicElementEditWidget);
    m_defuzzEditor->setVisible(false);

    m_fuzzyEditor=new FuzzyEditor(m_scene,m_logicElementEditWidget);
    m_fuzzyEditor->setVisible(false);

    m_noiseEditor=new NoiseEditor(m_scene,m_logicElementEditWidget);
    m_noiseEditor->setVisible(false);

    m_segmentEditor=new SegmentEditor(m_scene,m_logicElementEditWidget);
    m_segmentEditor->setVisible(false);

//    foreach(BrainEditor *brainEditor,m_brainEditors) {
//        // This signal activates editor in South region
//        connect(brainEditor, SIGNAL(itemClicked(ItemEditorWidgetsBase::editMessage)),this,SLOT(editorNodeClick(ItemEditorWidgetsBase::editMessage)));
//        // When a frame has been calculated update the braineditors to display the new values
//        connect(m_scene->getSimulation(),SIGNAL(frameDone()),brainEditor,SLOT(update()),Qt::DirectConnection);
//        // Display statusbar messages
//        connect(brainEditor, SIGNAL(statusBarMessageChanged(QString)),this,SLOT(statusBarMessageChange(QString)));
//    }
    //
    connect(m_inputEditor, SIGNAL(updateBrainEditor()),this,SLOT(refreshBrainEditor()));
    connect(m_outputEditor,SIGNAL(updateBrainEditor()),this,SLOT(refreshBrainEditor()));
    connect(m_defuzzEditor,SIGNAL(updateBrainEditor()),this,SLOT(refreshBrainEditor()));
    connect(m_fuzzyEditor,SIGNAL(updateBrainEditor()),this,SLOT(refreshBrainEditor()));
    connect(m_noiseEditor,SIGNAL(updateBrainEditor()),this,SLOT(refreshBrainEditor()));
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
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_loadAnimationsAction);
    m_fileMenu->addAction(m_saveAnimationAction);
    m_fileMenu->addAction(m_loadSkeletonAction);

    m_editMenu=menuBar()->addMenu(tr("&Edit"));
    m_editMenu->addAction(m_showActionEditorAction);
    m_editMenu->addAction(m_editSymetricAction);

    m_simulationMenu=menuBar()->addMenu(tr("&Simulation"));
    m_simulationMenu->addAction(m_runSimulationAction);
    m_simulationMenu->addAction(m_stopSimulationAction);
    m_simulationMenu->addAction(m_resetSimulationAction);

    m_viewMenu=menuBar()->addMenu(tr("&View"));
    m_viewMenu->addAction(m_viewAgentInfos);
    m_viewMenu->addAction(m_viewSoundEmmisions);
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
    Group *grp;
    AgentManager *mgr;
    switch(msg.type) {
    case BrainiacGlobals::GROUP:
        grp=(Group *)msg.object;
        m_groupEditor->setGroup(grp);
        m_activeAgentManager=grp->getAgentManager();
        //m_bodyDisplay->setAgentManager(m_activeAgentManager);
        m_bodyDisplayOSG->setAgentManager(m_activeAgentManager);
        break;
    case BrainiacGlobals::OUTPUT:
        mgr=(AgentManager *)msg.object;
        m_outputEditor->setOutputConfig(mgr,msg.id);
        break;
    case BrainiacGlobals::INPUT:
        mgr=(AgentManager *)msg.object;
        m_inputEditor->setInputConfig(mgr,msg.id);
        break;
    case BrainiacGlobals::DEFUZZ:
        mgr=(AgentManager *)msg.object;
        m_defuzzEditor->setDefuzzConfig(mgr,msg.id);
        break;
    case BrainiacGlobals::FUZZ:
        mgr=(AgentManager *)msg.object;
        m_fuzzyEditor->setFuzzConfig(mgr,msg.id);
        break;
    case BrainiacGlobals::NOISE:
        mgr=(AgentManager *)msg.object;
        m_noiseEditor->setNoiseConfig(mgr,msg.id);
        break;
    case BrainiacGlobals::CUBE:
        case BrainiacGlobals::SPHERE:
        mgr=(AgentManager *)msg.object;
        m_segmentEditor->setSegmentConfig(mgr,msg.id);
        break;
    default:
    ;
    }
    m_groupEditor->setVisible(msg.type==BrainiacGlobals::GROUP);
    m_inputEditor->setVisible(msg.type==BrainiacGlobals::INPUT);
    m_outputEditor->setVisible(msg.type==BrainiacGlobals::OUTPUT);
    m_defuzzEditor->setVisible(msg.type==BrainiacGlobals::DEFUZZ);
    m_fuzzyEditor->setVisible(msg.type==BrainiacGlobals::FUZZ);
    m_noiseEditor->setVisible(msg.type==BrainiacGlobals::NOISE);
    m_segmentEditor->setVisible(msg.type==BrainiacGlobals::CUBE || msg.type==BrainiacGlobals::SPHERE);
}

void MainWindow::readSettings()
{
    QSettings settings("Brainiac SW", "Brainiac");

    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();
}

void MainWindow::refreshBrainEditor()
{
    m_editorView->scene()->update();
}

void MainWindow::loadAnimation()
{
    QFileDialog::Options options;
    options |= QFileDialog::DontUseNativeDialog;
    QString selectedFilter;
    QFileInfo fInfo(m_scene->getFileName());
    QStringList fileNames = QFileDialog::getOpenFileNames(this,tr("Select one or more animations to import"),fInfo.absolutePath(),tr("BVH (*.bvh);;Brainiac Animation Format (*.baf)"),&selectedFilter,options);
    foreach(QString file, fileNames) {
        if(m_activeAgentManager) {
            m_activeAgentManager->loadAnimation(file);
        }
    }
    // Refresh the Editor
    m_actionEditor->setAgentManager(m_activeAgentManager);
}

void MainWindow::saveAnimation()
{
    if(m_actionEditor->isVisible()) {
        qDebug() << __PRETTY_FUNCTION__ << "Saving " << m_actionEditor->getActiveAnimationId();
        if(m_activeAgentManager) {
            Animation *anim=m_activeAgentManager->getAnimations()->value(m_actionEditor->getActiveAnimationId());
            if(anim) {
                if(!anim->fileName().length()) {
                    QFileDialog::Options options;
                    options |= QFileDialog::DontUseNativeDialog;
                    QString selectedFilter;
                    QFileInfo fInfo(m_scene->getFileName());
                    QString fileName=QFileDialog::getSaveFileName(this,tr("Select a file to save the animation"),fInfo.absolutePath(),tr("Brainiac Animation Format(*.baf)"),&selectedFilter,options);
                    anim->saveAnimation(fileName);
                } else {
                    anim->saveAnimation();
                }
            } else {
                QMessageBox::warning(this,"No animation selected","Select an animation first!");
            }
        }
    } else {
        QMessageBox::warning(this,"No animation selected","Select an animation first!");
    }

}

void MainWindow::loadSkeleton()
{
    QFileDialog::Options options;
    options |= QFileDialog::DontUseNativeDialog;
    QString selectedFilter;
    QFileInfo fInfo(m_scene->getFileName());
    QString fileName = QFileDialog::getOpenFileName(this,tr("Select a skeleton to import"),fInfo.absolutePath(),tr("BVH (*.bvh)"),&selectedFilter,options);
    Group *newGroup=new Group(m_scene);
    newGroup->setId(10);
    newGroup->setName("BVHImportTest");
    newGroup->getAgentManager()->loadSkeleton(fileName);
    newGroup->setEditorTranslation(2200,2000);
    m_sceneEditor->refresh();
    addAgentManager(newGroup->getAgentManager());

}

/** \brief saves the selected agent

**/
void MainWindow::saveAgent()
{
    if(m_activeAgentManager) {
        m_brainEditors.value(m_activeAgentManager)->updateItemLocations();
        m_bodyEditors.value(m_activeAgentManager)->updateItemLocations();
        if(!m_activeAgentManager->isFileNameSet()) {
            QString fileName = QFileDialog::getSaveFileName(this, tr("Save Agent as"),
                                        "",
                                        tr("XML (*.xml)"));
            m_activeAgentManager->setFileName(fileName);
        }
        m_activeAgentManager->saveConfig();
    }
}

/** \brief saves the scene

**/
void MainWindow::saveScene()
{
    // Update EditorItem position
    foreach(QGraphicsItem *item, m_sceneEditor->items()) {
        EditorItem *sItem=dynamic_cast<EditorItem *> (item);
        if(sItem) {
            if(sItem->getType()==BrainiacGlobals::GROUP) {
                Group *grp=(Group *) sItem->getObject();
                grp->setEditorTranslation(item->pos().x(),item->pos().y());
                qDebug() << item->pos();
            } else if(sItem->getType()==BrainiacGlobals::AGENT) {
                AgentManager *mgr=(AgentManager*) sItem->getObject();
                mgr->setEditorTranslation(item->pos().x(),item->pos().y());
            }
        }
    }

    m_scene->saveConfig();
}

void MainWindow::sceneEditorItemClicked()
{
    int numItemsSelected=m_sceneEditor->selectedItems().count();
    if(numItemsSelected<=1)
        m_loadSkeletonAction->setEnabled(true);
    else
        m_loadSkeletonAction->setEnabled(false);
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
        //m_bodyDisplay->setAgentManager(m_activeAgentManager);
        m_bodyDisplayOSG->setAgentManager(m_activeAgentManager);
        if(m_bodyEditors.contains(m_activeAgentManager)) {
            m_editorView->setScene(m_bodyEditors.value(m_activeAgentManager));
        }
        else {
            m_editorView->setScene(0);
        }
        break;
    case MainWindow::BRAIN:
        if(m_brainEditors.contains(m_activeAgentManager)) {
            m_editorView->setScene(m_brainEditors.value(m_activeAgentManager));
        }
        else {
            m_editorView->setScene(0);
        }
        break;
    case MainWindow::MOTION:
        break;
    }

}

void MainWindow::showActionEditor()
{
        m_actionEditor->setAgentManager(m_activeAgentManager);
        m_actionEditor->show();
}

void MainWindow::statusBarMessageChange(const QString &message)
{
    this->statusBar()->showMessage(message);
}

void MainWindow::writeSettings()
{
    QSettings settings("Brainiac SW", "Brainiac");

    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
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
