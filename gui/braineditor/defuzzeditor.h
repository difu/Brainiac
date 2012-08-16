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


#ifndef DEFUZZEDITOR_H
#define DEFUZZEDITOR_H

#include "gui/itemeditorwidgetsbase.h"
#include <QWidget>


class AgentManager;
class BrainiacSlider;
class FuzzyDefuzz;

namespace Ui {
    class DefuzzEditor;
}

class DefuzzEditor : public QWidget, public ItemEditorWidgetsBase
{
    Q_OBJECT

public:
    explicit DefuzzEditor(Scene *scene, QWidget *parent = 0);
    void setDefuzzConfig(AgentManager *manager, quint32 id);
    void updateEditor();
    ~DefuzzEditor();

protected:
    void changeEvent(QEvent *e);
    AgentManager *m_agentManager;
    quint32 m_id;
    FuzzyDefuzz *m_defuzz;
    BrainiacSlider *m_fuzzValSlider;
    BrainiacSlider *m_fuzzResultSlider;

protected slots:
    void manualDefuzzValueChange(qreal value);
    void manualNameChange();
    void manualIsElseChange(bool isChecked);
signals:
    void updateBrainEditor(); //!< emitted when editors should update
    void updateGLContent(); //!< emitted when openGL content should update

private:
    Ui::DefuzzEditor *ui;
};

#endif // DEFUZZEDITOR_H
