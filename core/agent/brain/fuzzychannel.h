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

#ifndef FUZZYCHANNEL_H
#define FUZZYCHANNEL_H

#include "core/agent/brain/fuzzybase.h"
#include "core/agent/brain/fuzzychannel.h"

class Channel;

class FuzzyChannel : public FuzzyBase
{
    Q_OBJECT
public:
    FuzzyChannel(LogicType logicType, quint32 id, Brain *brain, const QString& channelName, qreal min, qreal max);

    /** \brief  set the channel of this fuzzy
      @param channel the name of the channel
    **/
    virtual void setChannelName(const QString &channelName);

protected:
    Channel *m_channel; //!< a pointer to the channel, null if no channel is assigned
protected slots:
    void channelDelete();
};

#endif // FUZZYCHANNEL_H
