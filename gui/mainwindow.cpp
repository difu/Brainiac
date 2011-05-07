#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainwindowlayout.h"
#include "gui/editorgraphicsview.h"
#include "gui/sceneeditor/sceneeditor.h"
#include "gui/sceneeditor/groupeditor.h"
#include "editorlabel.h"
#include "core/scene.h"
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


//    m_layout->addWidget(new QLabel("EAST"),MainWindowLayout::East);
//    m_layout->addWidget(new QLabel("SOUTH"),MainWindowLayout::South);

    //qDebug() << __PRETTY_FUNCTION__;

    widget->setLayout(m_layout);
    setCentralWidget(widget);

    //connect(m_sceneEditor,SIGNAL(editorItemClicked(EditorLabel::EditorItemType)),this,SLOT(editorNodeClick(EditorLabel::EditorItemType)));
}

void MainWindow::createActions()
{
    // File Menu Actions
    m_saveSceneAction = new QAction(tr("Save Scene"),this);
    connect(m_saveSceneAction,SIGNAL(triggered()),this,SLOT(saveScene()));
}

void MainWindow::createEditors()
{
    m_editorView=new EditorGraphicsView();
    m_layout->addWidget(m_editorView,MainWindowLayout::Center);

    m_sceneEditor=new SceneEditor(m_scene);
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
    m_layout->addWidget(m_logicElementEditWidget,MainWindowLayout::South);
    connect(m_sceneEditor, SIGNAL(itemClicked(ItemEditorWidgetsBase::editMessage)),this,SLOT(editorNodeClick(ItemEditorWidgetsBase::editMessage)));

}


/** \brief creates all menues of the main window
**/
void MainWindow::createMenues()
{
    m_fileMenu=menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_saveSceneAction);
}

void MainWindow::editModeComboChange(int index)
{
    QVariant em=m_editModeComboBox->itemData(index);
    setEditMode((EditMode)em.toInt());
}

/** \brief called when a node in an editor is clicked

                This slot is called when any node in any editor is clicked.
                It shows the depending node editor and updates the editor with information of the
                clicked item

**/
void MainWindow::editorNodeClick(ItemEditorWidgetsBase::editMessage msg)
{
    m_groupEditor->setVisible(msg.type==BrainiacGlobals::GROUP);
    switch(msg.type) {
    case BrainiacGlobals::GROUP:
        m_groupEditor->setGroup((Group *)msg.object);
        break;
    default:
    ;
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
    m_sceneEditorItems->setVisible(em=MainWindow::SCENE);

    int index=m_editModeComboBox->findData(QVariant(em));
    m_editModeComboBox->setCurrentIndex(index);
    switch( em ) {
    case MainWindow::SCENE:
        m_editorView->setScene(m_sceneEditor);
        break;
    case MainWindow::BODY:
    case MainWindow::BRAIN:
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
