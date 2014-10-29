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

#include "scenedisplay.h"
#include "core/scene.h"
#include "gui/sceneproxygenerator.h"

SceneDisplay::SceneDisplay(Scene *scene): OsgMultithreadedViewerWidget(0,scene->getRootSceneNode()),
    m_scene(scene)
{
    setWindowTitle("Scene");
    m_osgFileName=QDir::tempPath()%"/Scene.osg";
    connect(getGlWindow()->getKeyMouseEater(),SIGNAL(keyPressed(Qt::Key)),this,SLOT(keyPressed(Qt::Key)));
    m_sceneProxy=new SceneProxyGenerator(this);
}

void SceneDisplay::keyPressed(Qt::Key key)
{
    if(key==Qt::Key_Comma) {
        if( saveSceneGraph() ) {
            qDebug() << "Saved scene to " << m_osgFileName;
        }
    } else if(key==Qt::Key_C) {
        toggleOriginCoordCross();
    }
    //Q_UNUSED(key);
}

SceneDisplay::~SceneDisplay()
{
    //osgDB::writeNodeFile(*m_rootNode,"/tmp/scene.osgt");
}
Scene *SceneDisplay::scene() const
{
    return m_scene;
}
