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
#include <QtGui/QtGui>
#include <osgDB/ReadFile>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osgGA/TrackballManipulator>
#include <osgGA/SphericalManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/Viewer>
#include <osgQt/GraphicsWindowQt>


class KeyPressReleaseEater : public QObject
 {
     Q_OBJECT

 protected:
     bool eventFilter(QObject *obj, QEvent *event)
     {
         if (event->type() == QEvent::KeyPress) {
             QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
             qDebug("Ate key press %d", keyEvent->key());
             emit keyPressed((Qt::Key)keyEvent->key());
             return false; // continue event processing
             //QObject::eventFilter(obj, event);
         } else {
             // standard event processing
             return QObject::eventFilter(obj, event);
         }
     }
signals:
     void keyPressed(Qt::Key key);
 };



class BrainiacGlWindow : public osgQt::GraphicsWindowQt
{
public:
    BrainiacGlWindow(osg::GraphicsContext::Traits* traits, QWidget* parent = NULL, const QGLWidget* shareWidget = NULL, Qt::WindowFlags f = 0 )
        :GraphicsWindowQt(traits, parent , shareWidget,f ){
        m_kpEater=new KeyPressReleaseEater();
        getGLWidget()->installEventFilter(m_kpEater);
    }
    virtual ~BrainiacGlWindow(){
        m_kpEater->deleteLater();
    }
    KeyPressReleaseEater *getKeyPressedReleasedEater() { return m_kpEater; }
    KeyPressReleaseEater *m_kpEater;
};

class RenderThread : public QThread
{
public:
    RenderThread() : QThread(), viewerPtr(0),m_targetFPS(60) {
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
    OsgMultithreadedViewerWidget( osg::Camera* camera=0, osg::Node* scene=0 ) :   QWidget()
    {
        m_camera=camera;
        m_rootNode=new osg::Group;
        m_rootNode->setName("Viewer rootGroupNode");
        if(!m_camera)
            m_camera=createCamera( 50, 50, 640, 480 );
        m_viewer.setCamera( m_camera );
        if(scene)
            m_rootNode->addChild(scene);
        else {
            //m_rootNode->addChild(osgDB::readNodeFile("/Users/dirkfuchs/Desktop/ground.obj"));
        }
        m_viewer.setSceneData(m_rootNode);
        m_viewer.addEventHandler( new osgViewer::StatsHandler );
        osgGA::TrackballManipulator *tbm=new osgGA::TrackballManipulator;
        tbm->setHomePosition(osg::Vec3f(-300,0,0),osg::Vec3f(),osg::Vec3f(0,1,0));
        m_viewer.setCameraManipulator(tbm  );
        //m_viewer.setThreadingModel( osgViewer::Viewer::CullThreadPerCameraDrawThreadPerContext );
        //m_viewer.setRunFrameScheme(osgViewer::ViewerBase::ON_DEMAND);

        osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>( m_camera->getGraphicsContext() );
        if ( gw )
        {
            QVBoxLayout* layout = new QVBoxLayout;
            layout->addWidget( gw->getGLWidget() );
            setLayout( layout );
        }

        m_thread.viewerPtr = &m_viewer;
        m_thread.start();
        setGeometry( 100, 100, 800, 600 );
        setWindowFlags(Qt::Tool);
        show();
    }
    osg::Camera* createCamera( int x, int y, int w, int h )
    {
        osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
        osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
        traits->windowDecoration = false;
        traits->x = x;
        traits->y = y;
        traits->width = w;
        traits->height = h;
        traits->doubleBuffer = true;

        osg::ref_ptr<osg::Camera> camera = new osg::Camera;
        m_glWindow=new BrainiacGlWindow(traits.get());
        camera->setGraphicsContext( m_glWindow );
        camera->setClearColor( osg::Vec4(0.2, 0.2, 0.6, 1.0) );
        camera->setViewport( new osg::Viewport(0, 0, traits->width, traits->height) );
        camera->setProjectionMatrixAsPerspective(
            30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f );

        //camera->setViewMatrixAsLookAt(osg::Vec3f(-1000,0.0f,0.0f),osg::Vec3f(),osg::Vec3f(1.0f,0.0f,0.0f));
        return camera.release();
    }
    BrainiacGlWindow *getGlWindow()
    {
        return m_glWindow;
    }

    osgGA::CameraManipulator* getCameraManipulator() {
        return m_viewer.getCameraManipulator();
    }

protected:
    osgViewer::Viewer m_viewer;
    osg::Group *m_rootNode;
    osg::Camera* m_camera;
    RenderThread m_thread;
    BrainiacGlWindow *m_glWindow;
};

#endif // OSGMULTITHREADEDQTWIDGET_H
