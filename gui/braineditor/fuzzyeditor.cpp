#include "fuzzyeditor.h"
#include "ui_fuzzyeditor.h"

FuzzyEditor::FuzzyEditor(Scene *scene, QWidget *parent) :
    QWidget(parent),
    ItemEditorWidgetsBase(scene),
    ui(new Ui::FuzzyEditor)
{
    ui->setupUi(this);
}

FuzzyEditor::~FuzzyEditor()
{
    delete ui;
}

void FuzzyEditor::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
