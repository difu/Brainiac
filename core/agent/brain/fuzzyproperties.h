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


#ifndef FUZZYPROPERTIES_H
#define FUZZYPROPERTIES_H

#include <QString>
#include <QPoint>

class FuzzyProperties
{
public:
    enum SoundChannelType {
        NOSOUNDCHANNEL,
        SOUND_X,
        SOUND_F,
        SOUND_D,
        SOUND_OX
    };

    FuzzyProperties();
    const QString& getName() const { return m_name; }
    void setName( const QString &name) { m_name=name; }

    qreal getMin() const;
    void setMin(const qreal &getMin);

    qreal getMax() const;
    void setMax(const qreal &getMax);

    const QPoint &getEditorLocation() const;
    void setEditorLocation(const QPoint &getEditorLocation);

    const QString& getChannelName() const;
    void setChannelName(const QString &getChannelName);

    SoundChannelType getSoundChannelType() const;

    bool isChannelSoundX() const;
    bool isChannelSoundF() const;
    bool isChannelSoundD() const;
    bool isChannelSoundOx() const;

private:
    QString m_channelName;
    QString m_name;
    QPoint m_editorLocation;
    qreal m_min;
    qreal m_max;
    SoundChannelType m_soundType;
};

#endif // FUZZYPROPERTIES_H
