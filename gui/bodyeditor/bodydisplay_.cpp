#include "bodydisplay_.h"

BodyDisplay_::BodyDisplay_() : OsgMultithreadedViewerWidget()
{
//    osg::Camera* camera = createCamera( 50, 50, 640, 480 );
//    osg::Node *ground=osgDB::readNodeFile("/Users/dirkfuchs/Desktop/ground.obj");
    setWindowFlags(Qt::Tool);
    show();
}
