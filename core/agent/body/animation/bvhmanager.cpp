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



#include "bvhmanager.h"
#include "core/agent/agent.h"
#include "core/agent/agentmanager.h"
#include "core/agent/body/segment.h"
#include "core/agent/body/bodymanager.h"

BvhManager::BvhManager(AgentManager *parent) :
    QObject(parent)
{
    m_options=PositionAsBone;
    m_bvhSkeletonDirty=true;
    m_bvhSegmentChannelListDirty=true;
}

const QList<QString>& BvhManager::getBVHMotionChannelList() const {
    AgentManager *am=(AgentManager *) parent();
    if(m_bvhSegmentChannelListDirty || m_bvhChannelList.count()==0) {
        m_bvhChannelList.clear();
        foreach(quint32 segId, am->getBodyManager()->getTraversedSegmentIds()) {
            Segment seg=am->getBodyManager()->getSegment(segId);
            for(int i=5 ; i>=0 ; i--) {
                BrainiacGlobals::RotTrans rt=seg.getRotationTranslationOrder().at(i);
                switch(rt) {
                case BrainiacGlobals::TX:
                    m_bvhChannelList.append(seg.getName()%":tx");
                    break;
                case BrainiacGlobals::TY:
                    m_bvhChannelList.append(seg.getName()%":ty");
                    break;
                case BrainiacGlobals::TZ:
                    m_bvhChannelList.append(seg.getName()%":tz");
                    break;
                case BrainiacGlobals::RX:
                    m_bvhChannelList.append(seg.getName()%":rx");
                    break;
                case BrainiacGlobals::RY:
                    m_bvhChannelList.append(seg.getName()%":ry");
                    break;
                case BrainiacGlobals::RZ:
                    m_bvhChannelList.append(seg.getName()%":rz");
                    break;
                }
            }
        }
        m_bvhSegmentChannelListDirty=false;
    }
    return m_bvhChannelList;
}

quint32 BvhManager::getBvhRootId() const
{
    AgentManager *am=(AgentManager *) parent();
    return am->getBodyManager()->getRootSegment().getId();;
}

QString BvhManager::getBvhRotTransFromBrainiacRotTrans(const QList<BrainiacGlobals::RotTrans> &list)
{
    QString bvhRotTrans;
    Q_ASSERT(list.count()==6);
    for(int i=5 ; i>=0 ; i--) {
        BrainiacGlobals::RotTrans rt=list.at(i);
        switch(rt) {
        case BrainiacGlobals::TX:
            bvhRotTrans.append("Xposition ");
            break;
        case BrainiacGlobals::TY:
            bvhRotTrans.append("Yposition ");
            break;
        case BrainiacGlobals::TZ:
            bvhRotTrans.append("Zposition ");
            break;
        case BrainiacGlobals::RX:
            bvhRotTrans.append("Xrotation ");
            break;
        case BrainiacGlobals::RY:
            bvhRotTrans.append("Yrotation ");
            break;
        case BrainiacGlobals::RZ:
            bvhRotTrans.append("Zrotation ");
            break;
        }
    }
    return bvhRotTrans;

}

const QString& BvhManager::getBVHSkeleton() const
{
    if(m_bvhSkeletonDirty) {
        AgentManager *am=(AgentManager *) parent();
        m_bvhSkeleton.clear();
        m_bvhSkeleton.append("HIERARCHY\n");
        int level=0;
        int oldLevel=level-1;
        int levelOffset=0;
        if(m_options&PositionAsBone) {
            levelOffset=1;
            m_bvhSkeleton.append("ROOT Position\n");
            m_bvhSkeleton.append("{\n");
            m_bvhSkeleton.append(" OFFSET 0.0 0.0 0.0\n");
            m_bvhSkeleton.append(" CHANNELS 6 ");
            for(int i=0 ; i<=5 ; i++) {
                BrainiacGlobals::RotTrans rt=am->getBodyManager()->getRootRotTransOrder().at(i);
                switch(rt) {
                case BrainiacGlobals::TX:
                    m_bvhSkeleton.append("Xposition ");
                    break;
                case BrainiacGlobals::TY:
                    m_bvhSkeleton.append("Yposition ");
                    break;
                case BrainiacGlobals::TZ:
                    m_bvhSkeleton.append("Zposition ");
                    break;
                case BrainiacGlobals::RX:
                    m_bvhSkeleton.append("Xrotation ");
                    break;
                case BrainiacGlobals::RY:
                    m_bvhSkeleton.append("Yrotation ");
                    break;
                case BrainiacGlobals::RZ:
                    m_bvhSkeleton.append("Zrotation ");
                    break;
                }
            }
            m_bvhSkeleton.append("\n");
            //m_bvhSkeleton.append(QString::number(seg.getRestTranslation().x(),'f')).append(" ").append(QString::number(seg.getRestTranslation().y(),'f')).append(" ").append(QString::number(seg.getRestTranslation().z(),'f')).append("\n");
        }
        foreach(quint32 segId, am->getBodyManager()->getTraversedSegmentIds()) {
            Segment seg=am->getBodyManager()->getSegment(segId);
            level=am->getBodyManager()->getSegmentLevel(segId)+levelOffset;
            if(oldLevel>level) {
                for( int i=oldLevel-level; i>=0; i--) {
                    m_bvhSkeleton.append(QString(" ").repeated((level + i)*2)).append("}\n");
                }
            }
            oldLevel=level;

            QString indent=QString(" ").repeated(level*2);
            QString indent_1=QString(" ").repeated((level+1)*2);
            m_bvhSkeleton.append(indent);
            if(seg.isRootSegment() && level==0) {
                m_bvhSkeleton.append("ROOT ");
            } else {
                m_bvhSkeleton.append("JOINT ");
            }
            m_bvhSkeleton.append(seg.getName());
            m_bvhSkeleton.append("\n").append(indent).append("{").append("\n");
            m_bvhSkeleton.append(indent_1).append("OFFSET ");
            m_bvhSkeleton.append(QString::number(seg.getRestTranslation().x(),'f')).append(" ").append(QString::number(seg.getRestTranslation().y(),'f')).append(" ").append(QString::number(seg.getRestTranslation().z(),'f')).append("\n");
            m_bvhSkeleton.append(indent_1).append("CHANNELS ").append(QString::number(seg.getRotationTranslationOrder().count())).append(" ");
//            for(int i=5 ; i>=0 ; i--) {
//                BrainiacGlobals::RotTrans rt=seg.getRotationTranslationOrder().at(i);
//                switch(rt) {
//                case BrainiacGlobals::TX:
//                    m_bvhSkeleton.append("Xposition ");
//                    break;
//                case BrainiacGlobals::TY:
//                    m_bvhSkeleton.append("Yposition ");
//                    break;
//                case BrainiacGlobals::TZ:
//                    m_bvhSkeleton.append("Zposition ");
//                    break;
//                case BrainiacGlobals::RX:
//                    m_bvhSkeleton.append("Xrotation ");
//                    break;
//                case BrainiacGlobals::RY:
//                    m_bvhSkeleton.append("Yrotation ");
//                    break;
//                case BrainiacGlobals::RZ:
//                    m_bvhSkeleton.append("Zrotation ");
//                    break;
//                }
//            }
            m_bvhSkeleton.append(getBvhRotTransFromBrainiacRotTrans(seg.getRotationTranslationOrder()));

            //if(false) {
            if(am->getBodyManager()->hasChildren(seg.getId())) {
                m_bvhSkeleton.append("\n");
                m_bvhSkeleton.append(indent_1).append("End Site\n");
                m_bvhSkeleton.append(indent_1).append("{\n");
                m_bvhSkeleton.append(indent_1).append(" OFFSET ").append(QString::number(seg.getTranslation().x(),'f')).append(" ").append(QString::number(seg.getTranslation().y(),'f')).append(" ").append(QString::number(seg.getTranslation().z(),'f')).append("\n");;
                m_bvhSkeleton.append(indent_1).append("}");
            }
            m_bvhSkeleton.append("\n");
        }
        // Close all braces at the end
        for( int i=level; i>=0; i--) {
            m_bvhSkeleton.append(QString(" ").repeated(i*2)).append("}\n");
        }
        m_bvhSkeletonDirty=false;
    }

    return m_bvhSkeleton;
}

void BvhManager::setChannelListDirty()
{
    m_bvhSegmentChannelListDirty=true;
}

void BvhManager::setOptions(BvhOptions options)
{
    setChannelListDirty();
    setSkeletonDirty();
    m_options=options;
}

void BvhManager::setSkeletonDirty()
{
    m_bvhSkeletonDirty=true;
}
