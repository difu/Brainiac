#include "coordcrossdrawable.h"
#include "core/brainiacglobals.h"
#include <GLUT/glut.h>

CoordCrossDrawable::CoordCrossDrawable()
{
}

osg::BoundingBox CoordCrossDrawable::computeBound() const
{
    osg::Vec3 min(0,0,0), max(1,1,1);
    return osg::BoundingBox(min, max);
}

void CoordCrossDrawable::drawImplementation(osg::RenderInfo &renderInfo) const
{
    osg::ref_ptr<osg::StateSet> standartSet(new osg::StateSet);
    renderInfo.getState()->pushStateSet(standartSet);
    static const float _length=10.0f;
    glColor3f(BrainiacGlobals::defaultXColor.redF(),BrainiacGlobals::defaultXColor.greenF(),BrainiacGlobals::defaultXColor.blueF());
    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(_length,0,0);
    glEnd();
    glColor3f(BrainiacGlobals::defaultYColor.redF(),BrainiacGlobals::defaultYColor.greenF(),BrainiacGlobals::defaultYColor.blueF());
    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(0,_length,0);
    glEnd();
    glColor3f(BrainiacGlobals::defaultZColor.redF(),BrainiacGlobals::defaultZColor.greenF(),BrainiacGlobals::defaultZColor.blueF());
    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(0,0,_length);
    glEnd();
    renderInfo.getState()->popStateSet();
}
