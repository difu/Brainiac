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

#ifndef BRAINMANAGER_H
#define BRAINMANAGER_H

#include <QObject>
#include <QPoint>
#include <QHash>
#include <core/brainiacglobals.h>

class AgentManager;

class BrainManager : public QObject
{
    Q_OBJECT
public:
    struct FuzzyProperties {
        QString name;
        QPoint editorLocation;
        qreal min;
        qreal max;
    };

    explicit BrainManager(QObject *parent);
    AgentManager *getAgentManager() const;
    const QPoint& getEditorFuzzyLocation( BrainiacGlobals::BrainiacId id) const;
    BrainiacGlobals::BrainiacId getFuzzyId(const QString& name) const;
    const QString& getFuzzyName(BrainiacGlobals::BrainiacId id) const;
    void setFuzzyEditorTranslation(BrainiacGlobals::BrainiacId, qint32 x, qint32 y);
    void setFuzzyName(BrainiacGlobals::BrainiacId id, const QString& name);
private:
    void insertNewFuzzy(BrainiacGlobals::BrainiacId id);
    QHash<BrainiacGlobals::BrainiacId, FuzzyProperties *> m_fuzzyProperties;


signals:

public slots:

};

#endif // BRAINMANAGER_H
