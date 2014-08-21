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


#ifndef OSGMULTITHREADEDQTWIDGET_H
#define OSGMULTITHREADEDQTWIDGET_H

#include <QWidget>
#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#ifdef BRAINIAC_SUPPRESS_THIRD_PARTY_WARNINGS
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Woverloaded-virtual"
#endif
#include <osgDB/ReadFile>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/Viewer>
#ifdef BRAINIAC_SUPPRESS_THIRD_PARTY_WARNINGS
    #pragma clang diagnostic pop
#endif
#include <osgQt/GraphicsWindowQt>
#include "gui/brainiacdisplaykeymouseeater.h"
#include "gui/brainiaccameramanipulator.h"


class BrainiacGlWindow : public osgQt::GraphicsWindowQt
{
public:
    BrainiacGlWindow(osg::GraphicsContext::Traits* traits, QWidget* parent = NULL, const QGLWidget* shareWidget = NULL, Qt::WindowFlags f = 0 )
        :GraphicsWindowQt(traits, parent , shareWidget,f ){
        m_kmEater=new BrainiacDisplayKeyMouseEater();
        getGLWidget()->installEventFilter(m_kmEater);
    }
    virtual ~BrainiacGlWindow(){
        getGLWidget()->removeEventFilter(m_kmEater);
        m_kmEater->deleteLater();
    }
    BrainiacDisplayKeyMouseEater *getKeyMouseEater() { return m_kmEater; }
    BrainiacDisplayKeyMouseEater *m_kmEater;
};

class RenderThread : public QThread
{
public:
    RenderThread() : QThread(), m_targetFPS(60), viewerPtr(0) {
        calcTargetFPSParams();
    }

    virtual ~RenderThread()
    {
        if (viewerPtr) {
            viewerPtr->setDone(true);
            wait();
            qDebug() << __PRETTY_FUNCTION__;
        }

    }
    int m_targetFPS;
    int m_waitMS,m_waitUS;

    osgViewer::Viewer* viewerPtr;

protected:
    void calcTargetFPSParams() {
        m_waitMS=1000/m_targetFPS;
        m_waitUS=1000000/m_targetFPS;
    }

    virtual void run() {
        if (viewerPtr) {
            //viewerPtr->run();
            while (!viewerPtr->done()) {
                QTime t;
                t.start();
                viewerPtr->frame();
                int elapsed=t.elapsed();
                if(elapsed<m_waitMS) {
                    usleep(m_waitUS-elapsed*1000);
                }
            }
        }
    }
};


class OsgMultithreadedViewerWidget : public QWidget
{
public:
    enum Manipulator {TRACKBALL,SPHERICAL};
    OsgMultithreadedViewerWidget( osg::Camera* camera=0, osg::Node* scene=0, bool fixVerticalAxis=true );
    bool saveSceneGraph();
    virtual ~OsgMultithreadedViewerWidget() {
    }

    osg::Camera* createCamera( int x, int y, int w, int h );
    BrainiacGlWindow *getGlWindow();

    /**
     * @brief switches on/off a coordinate cross at the origin of the scene
     *
     * @fn showOriginCoordCross
     * @param show true, if the coord cross should appear
     */
    void showOriginCoordCross(bool show);

public slots:
    /**
     * @brief toggles the coordinate cross at origin
     *
     * @sa showOriginCoordCross()
     * @fn toggleOriginCoordCross
     */
    void toggleOriginCoordCross();

    osgGA::CameraManipulator* getCameraManipulator();

private:
    osg::Group *m_rootNode;
    osgQt::GraphicsWindowQt *m_gw;
    osg::Matrix m_scaleOriginCoordCross;
    osg::ref_ptr<osg::Switch> m_originCoordCrossWitch;
    bool m_showOriginCoordCross;
    osgViewer::Viewer m_viewer;
    osg::Camera* m_camera;
    RenderThread m_thread;
    BrainiacGlWindow *m_glWindow;
    QTimer m_timer;
    BrainiacCameraManipulator *m_cameraManipulator;


protected:
    virtual void paintEvent( QPaintEvent* event );
    void addOriginCoordCross(osg::Group * node );
    QString m_osgFileName;
    osg::ref_ptr<osg::Group> m_sceneNode;
};

#endif // OSGMULTITHREADEDQTWIDGET_H
