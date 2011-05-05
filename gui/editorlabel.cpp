#include "editorlabel.h"
#include <QtGui>
#include <QDebug>


EditorLabel::EditorLabel(EditorLabelType labelType) :
    QLabel()
{
    m_type=labelType;
    switch(m_type) {
    case EditorLabel::AND:
        setPixmap(QPixmap(":/gui/pics/editor_logo_and.png"));
        break;
    case EditorLabel::OR:
        setPixmap(QPixmap(":/gui/pics/editor_logo_or.png"));
        break;
    case EditorLabel::FUZZ:
        setPixmap(QPixmap(":/gui/pics/editor_logo_fuzz.png"));
        break;
    case EditorLabel::DEFUZZ:
        setPixmap(QPixmap(":/gui/pics/editor_logo_defuzz.png"));
        break;
    case EditorLabel::TIMER:
        setPixmap(QPixmap(":/gui/pics/editor_logo_timer.png"));
        break;
    case EditorLabel::NOISE:
        setPixmap(QPixmap(":/gui/pics/editor_logo_noise.png"));
        break;
    case EditorLabel::OUTPUT:
        setPixmap(QPixmap(":/gui/pics/editor_logo_output.png"));
        break;
    case EditorLabel::INPUT:
        setPixmap(QPixmap(":/gui/pics/editor_logo_input.png"));
        break;
    default:
        qDebug() << __PRETTY_FUNCTION__ << "Wrong label parameter or label not yet implemented!";
    }
    setMouseTracking(true);
}

void EditorLabel::mousePressEvent(QMouseEvent *ev)
{
    QLabel::mousePressEvent(ev);
    QDrag *drag=new QDrag(this);
    QPixmap pixmap;
    switch(m_type) {
    case EditorLabel::AND:
        pixmap.load(":/gui/pics/editor_logo_and.png");
        break;
    case EditorLabel::OR:
        pixmap.load(":/gui/pics/editor_logo_or.png");
        break;
    case EditorLabel::FUZZ:
        pixmap.load(":/gui/pics/editor_logo_fuzz.png");
        break;
    case EditorLabel::DEFUZZ:
        pixmap.load(":/gui/pics/editor_logo_defuzz.png");
        break;
    case EditorLabel::TIMER:
        pixmap.load(":/gui/pics/editor_logo_timer.png");
        break;
    case EditorLabel::NOISE:
        pixmap.load(":/gui/pics/editor_logo_noise.png");
        break;
    case EditorLabel::OUTPUT:
        pixmap.load(":/gui/pics/editor_logo_output.png");
        break;
    case EditorLabel::INPUT:
        pixmap.load(":/gui/pics/editor_logo_input.png");
        break;
    default:
        qDebug() << __PRETTY_FUNCTION__ << "Wrong label parameter or label not yet implemented!";
    }

    drag->setPixmap(pixmap);
    QMimeData *mimeData = new QMimeData;
    drag->setHotSpot(QPoint(drag->pixmap().width()/2,
                            drag->pixmap().height()));
    mimeData->setText(QString("AddLogicType"+QString::number(m_type) ));
    drag->setMimeData(mimeData);
    drag->start();
}
