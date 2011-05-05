#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainwindowlayout.h"
#include "editorlabel.h"
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
    createEditModeWidgets();

    setEditMode(MainWindow::BODY);


//    m_layout->addWidget(new QLabel("EAST"),MainWindowLayout::East);
//    m_layout->addWidget(new QLabel("SOUTH"),MainWindowLayout::South);

    //qDebug() << __PRETTY_FUNCTION__;

    widget->setLayout(m_layout);
    setCentralWidget(widget);
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
    EditorLabel *editLabel=new EditorLabel(EditorLabel::AND);
    myVBoxlayout->addWidget(editLabel);
    myVBoxlayout->addWidget(new QLabel("AND"));
    editLabel=new EditorLabel(EditorLabel::OR);
    myVBoxlayout->addWidget(editLabel);
    myVBoxlayout->addWidget(new QLabel("OR"));
    editLabel=new EditorLabel(EditorLabel::FUZZ);
    myVBoxlayout->addWidget(editLabel);
    myVBoxlayout->addWidget(new QLabel("FUZZ"));
    editLabel=new EditorLabel(EditorLabel::DEFUZZ);
    myVBoxlayout->addWidget(editLabel);
    myVBoxlayout->addWidget(new QLabel("DEFUZZ"));
    editLabel=new EditorLabel(EditorLabel::INPUT);
    myVBoxlayout->addWidget(editLabel);
    myVBoxlayout->addWidget(new QLabel("INPUT"));
    editLabel=new EditorLabel(EditorLabel::OUTPUT);
    myVBoxlayout->addWidget(editLabel);
    myVBoxlayout->addWidget(new QLabel("OUTPUT"));
    editLabel=new EditorLabel(EditorLabel::TIMER);
    myVBoxlayout->addWidget(editLabel);
    myVBoxlayout->addWidget(new QLabel("TIMER"));
    editLabel=new EditorLabel(EditorLabel::NOISE);
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

    m_layout->addWidget(m_editorItems,MainWindowLayout::West);

}

void MainWindow::editModeComboChange(int index)
{
    QVariant em=m_editModeComboBox->itemData(index);
    setEditMode((EditMode)em.toInt());
}

void MainWindow::setEditMode(EditMode em)
{
    m_editMode=em;
    m_brainEditorItems->setVisible(em==MainWindow::BRAIN);
    m_bodyEditorItems->setVisible(em==MainWindow::BODY);

    int index=m_editModeComboBox->findData(QVariant(em));
    m_editModeComboBox->setCurrentIndex(index);
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
