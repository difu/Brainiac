#include "editorlabel.h"
#include <QtGui>
#include <QDebug>


EditorLabel::EditorLabel(BrainiacGlobals::ItemType labelType) :
    QLabel()
{
    m_type=labelType;
    switch(m_type) {
    case BrainiacGlobals::AND:
        setPixmap(QPixmap(":/gui/pics/editor_logo_and.png"));
        break;
    case BrainiacGlobals::OR:
        setPixmap(QPixmap(":/gui/pics/editor_logo_or.png"));
        break;
    case BrainiacGlobals::FUZZ:
        setPixmap(QPixmap(":/gui/pics/editor_logo_fuzz.png"));
        break;
    case BrainiacGlobals::DEFUZZ:
        setPixmap(QPixmap(":/gui/pics/editor_logo_defuzz.png"));
        break;
    case BrainiacGlobals::TIMER:
        setPixmap(QPixmap(":/gui/pics/editor_logo_timer.png"));
        break;
    case BrainiacGlobals::NOISE:
        setPixmap(QPixmap(":/gui/pics/editor_logo_noise.png"));
        break;
    case BrainiacGlobals::OUTPUT:
        setPixmap(QPixmap(":/gui/pics/editor_logo_output.png"));
        break;
    case BrainiacGlobals::INPUT:
        setPixmap(QPixmap(":/gui/pics/editor_logo_input.png"));
        break;
    case BrainiacGlobals::AGENT:
        setPixmap(QPixmap(":/gui/pics/editor_logo_agent.png"));
        break;
    case BrainiacGlobals::GROUP:
        setPixmap(QPixmap(":/gui/pics/editor_logo_group.png"));
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
    case BrainiacGlobals::AND:
        pixmap.load(":/gui/pics/editor_logo_and.png");
        break;
    case BrainiacGlobals::OR:
        pixmap.load(":/gui/pics/editor_logo_or.png");
        break;
    case BrainiacGlobals::FUZZ:
        pixmap.load(":/gui/pics/editor_logo_fuzz.png");
        break;
    case BrainiacGlobals::DEFUZZ:
        pixmap.load(":/gui/pics/editor_logo_defuzz.png");
        break;
    case BrainiacGlobals::TIMER:
        pixmap.load(":/gui/pics/editor_logo_timer.png");
        break;
    case BrainiacGlobals::NOISE:
        pixmap.load(":/gui/pics/editor_logo_noise.png");
        break;
    case BrainiacGlobals::OUTPUT:
        pixmap.load(":/gui/pics/editor_logo_output.png");
        break;
    case BrainiacGlobals::INPUT:
        pixmap.load(":/gui/pics/editor_logo_input.png");
        break;
    case BrainiacGlobals::AGENT:
        pixmap.load(":/gui/pics/editor_logo_agent.png");
        break;
    case BrainiacGlobals::GROUP:
        pixmap.load(":/gui/pics/editor_logo_group.png");
        break;
    default:
        qDebug() << __PRETTY_FUNCTION__ << "Wrong label parameter or label not yet implemented!";
    }

    drag->setPixmap(pixmap);
    QMimeData *mimeData = new QMimeData;
    drag->setHotSpot(QPoint(drag->pixmap().width()/2,
                            drag->pixmap().height()));
    mimeData->setText(QString("AddLogicType "+QString::number(m_type) ));
    drag->setMimeData(mimeData);
    drag->start();
}
