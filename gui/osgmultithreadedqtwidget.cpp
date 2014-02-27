#include "gui/osgmultithreadedqtwidget.h"
#include "osgDB/WriteFile"

bool OsgMultithreadedViewerWidget::saveSceneGraph()
{
    bool success=osgDB::writeNodeFile(*m_rootNode,m_osgFileName.toStdString());
    if(!success) {
        qDebug() << __PRETTY_FUNCTION__ << "Error writing scene graph to " << m_osgFileName;
    }
    return success;
}
