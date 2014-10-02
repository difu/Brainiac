// Brainiac is a free and open source tool for the creation of crowd simulation

// Copyright (C) 2014  Dirk Fuchs dirkfux@googlemail.com

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

#include "fuzzyproperties.h"
#include "core/brainiacglobals.h"

FuzzyProperties::FuzzyProperties():
    m_min(0.0),
    m_max(1.0),
    m_soundType(NOSOUNDCHANNEL)
{
}
qreal FuzzyProperties::getMin() const
{
    return m_min;
}

void FuzzyProperties::setMin(const qreal &min)
{
    m_min = min;
}
qreal FuzzyProperties::getMax() const
{
    return m_max;
}

void FuzzyProperties::setMax(const qreal &max)
{
    m_max = max;
}
const QPoint& FuzzyProperties::getEditorLocation() const
{
    return m_editorLocation;
}

void FuzzyProperties::setEditorLocation(const QPoint &editorLocation)
{
    m_editorLocation = editorLocation;
}
const QString &FuzzyProperties::getChannelName() const
{
    return m_channelName;
}

void FuzzyProperties::setChannelName(const QString &channelName)
{
    m_soundType=NOSOUNDCHANNEL;
    m_channelName = channelName;
    if(QString::compare(m_channelName,BrainiacGlobals::ChannelName_Sound_x,Qt::CaseInsensitive)==0 ) {
        m_soundType=SOUND_X;
    } else if(QString::compare(m_channelName,BrainiacGlobals::ChannelName_Sound_f,Qt::CaseInsensitive)==0 ) {
        m_soundType=SOUND_F;
    } else if(QString::compare(m_channelName,BrainiacGlobals::ChannelName_Sound_d,Qt::CaseInsensitive)==0 ) {
        m_soundType=SOUND_D;
    } else if(QString::compare(m_channelName,BrainiacGlobals::ChannelName_Sound_ox,Qt::CaseInsensitive)==0 ) {
        m_soundType=SOUND_OX;
    }
}

FuzzyProperties::SoundChannelType FuzzyProperties::getSoundChannelType() const
{
    return m_soundType;
}

bool FuzzyProperties::isChannelSoundD() const
{
    return m_soundType==SOUND_D;
}

bool FuzzyProperties::isChannelSoundF() const
{
    return m_soundType==SOUND_F;
}

bool FuzzyProperties::isChannelSoundX() const
{
    return m_soundType==SOUND_X;
}

bool FuzzyProperties::isChannelSoundOx() const
{
    return m_soundType==SOUND_OX;
}

bool FuzzyProperties::isChannelSound() const
{
    bool isSound=isChannelSoundD() ||
            isChannelSoundF() ||
            isChannelSoundOx() ||
            isChannelSoundX();
    return isSound;
}
