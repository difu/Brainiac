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


#ifndef OUTPUT_H
#define OUTPUT_H

#include "core/agent/brain/fuzzychannel.h"

class Channel;

/** \brief  Output fuzz node

        this class describes and manages an output node in the agent´s brain.
        if this node is associated with an output channel, it writes the calculated results out

**/
class Output : public FuzzyChannel
{
    Q_OBJECT
public:
    enum DefuzzMode {AVERAGE, MAX};
    Output(quint32 id, Brain *brain, const QString &channel, qreal min, qreal max);
//    QString getChannelName();
    void calculate();
    Output::DefuzzMode getDefuzzMode();
//    void setChannelName(QString channel);
    void setDefuzzMode( DefuzzMode mode);
    bool setResult(qreal result,bool emitChange=true);

//protected slots:
//    void channelDelete();

private:
    DefuzzMode m_defuzzMode;
};

#endif // OUTPUT_H
