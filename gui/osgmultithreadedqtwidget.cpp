#include <osgGA/TrackballManipulator>
#include <osgGA/SphericalManipulator>
#include <osgGA/OrbitManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/TerrainManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/MultiTouchTrackballManipulator>

#include "gui/osgmultithreadedqtwidget.h"
#include "osgDB/WriteFile"

OsgMultithreadedViewerWidget::OsgMultithreadedViewerWidget( osg::Camera* camera, osg::Node* scene )
{
    m_osgFileName=QDir::tempPath()%"/osgOut.osg";
    m_camera=camera;
    m_rootNode=new osg::Group;
    m_rootNode->setName("Viewer rootGroupNode");
    if(!m_camera)
        m_camera=createCamera( 50, 50, 640, 480 );
    m_viewer.setCamera( m_camera );
    if(scene)
        m_rootNode->addChild(scene);
    else {
        m_rootNode->addChild(osgDB::readNodeFile("/ground.obj"));
    }
    m_viewer.setSceneData(m_rootNode);
    m_viewer.addEventHandler( new osgViewer::StatsHandler );
    m_trackBallManipulator=new osgGA::TrackballManipulator;
    m_sphericalManipulatior=new osgGA::SphericalManipulator;
//        m_currentManipulator=m_trackBallManipulator;
//        m_currentManipulator->setHomePosition(osg::Vec3f(-300,0,0),osg::Vec3f(),osg::Vec3f(0,1,0));
    //m_viewer.setCameraManipulator(m_currentManipulator  );
//        changeManipulator(TRACKBALL);
    osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;

    keyswitchManipulator->addMatrixManipulator( '1', "Trackball", new osgGA::TrackballManipulator() );
    keyswitchManipulator->addMatrixManipulator( '2', "Flight", new osgGA::FlightManipulator() );
    keyswitchManipulator->addMatrixManipulator( '3', "Drive", new osgGA::DriveManipulator() );
//        unsigned int num = keyswitchManipulator->getNumMatrixManipulators();
    keyswitchManipulator->addMatrixManipulator( '4', "Terrain", new osgGA::TerrainManipulator() );
    keyswitchManipulator->addMatrixManipulator( '5', "SPHERIC", new osgGA::SphericalManipulator() );
    keyswitchManipulator->addMatrixManipulator( '6', "Orbit", new osgGA::OrbitManipulator() );
    keyswitchManipulator->addMatrixManipulator( '7', "Multitouch", new osgGA::MultiTouchTrackballManipulator() );
    keyswitchManipulator->setHomePosition(osg::Vec3f(-300,0,0),osg::Vec3f(),osg::Vec3f(0,1,0));
    m_viewer.setCameraManipulator(keyswitchManipulator);

    m_viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);
    //m_viewer.setThreadingModel( osgViewer::Viewer::CullThreadPerCameraDrawThreadPerContext );
    //m_viewer.setRunFrameScheme(osgViewer::ViewerBase::ON_DEMAND);

    m_gw = dynamic_cast<osgQt::GraphicsWindowQt*>( m_camera->getGraphicsContext() );
    if ( m_gw )
    {
        QVBoxLayout* layout = new QVBoxLayout;
        layout->addWidget( m_gw->getGLWidget() );
        setLayout( layout );
    }
    if(false) {
        m_thread.viewerPtr = &m_viewer;
        m_thread.start();
    } else {
        connect( &m_timer, SIGNAL(timeout()), this, SLOT(update()) );
        m_timer.start( 40 );
    }
    setGeometry( 100, 100, 800, 600 );
    setWindowFlags(Qt::Tool);
    show();
}


bool OsgMultithreadedViewerWidget::saveSceneGraph()
{
    bool success=osgDB::writeNodeFile(*m_rootNode,m_osgFileName.toStdString());
    if(!success) {
        qWarning() << __PRETTY_FUNCTION__ << "Error writing scene graph to " << m_osgFileName;
    }
    return success;
}

osg::Camera *OsgMultithreadedViewerWidget::createCamera(int x, int y, int w, int h)
{
    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
    Q_UNUSED(ds);
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
    camera->getOrCreateStateSet()->setGlobalDefaults(); //! \bug "wrong gfx" http://forum.openscenegraph.org/viewtopic.php?t=13111
    return camera.release();
}

BrainiacGlWindow *OsgMultithreadedViewerWidget::getGlWindow()
{
    return m_glWindow;
}

osgGA::CameraManipulator *OsgMultithreadedViewerWidget::getCameraManipulator() {
    return m_viewer.getCameraManipulator();
}
