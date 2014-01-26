#include "motiontreeeditorgui.h"
#include "ui_motiontreeeditorgui.h"

MotionTreeEditorGui::MotionTreeEditorGui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MotionTreeEditorGui)
{
    ui->setupUi(this);
}

MotionTreeEditorGui::~MotionTreeEditorGui()
{
    delete ui;
}
