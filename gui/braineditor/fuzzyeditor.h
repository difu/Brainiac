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


#ifndef FUZZYEDITOR_H
#define FUZZYEDITOR_H

#include "gui/itemeditorwidgetsbase.h"

#include <QWidget>

class AgentManager;
class FuzzyFuzz;
class FuzzyEditorCurveEditor;


namespace Ui {
    class FuzzyEditor;
}

/** \brief  Editor to modify the properties of a fuzz rule
  @sa FuzzyFuzz
**/
class FuzzyEditor : public QWidget, public ItemEditorWidgetsBase
{
    Q_OBJECT

public:
    explicit FuzzyEditor(Scene *scene, QWidget *parent = 0);
    void setFuzzConfig(AgentManager *manager, quint32 id);
    void updateEditor();
    ~FuzzyEditor();

protected:
    void changeEvent(QEvent *e);
    AgentManager *m_agentManager;
    quint32 m_id;
    FuzzyFuzz *m_fuzz;
    FuzzyEditorCurveEditor *m_editor; //!< QGraphicsScene which is the editor of the membership curve

protected slots:
    void modeLinearPressed();
    void modeSinePressed();
    void modeQuadPressed();

    void modeDiracPressed();
    void modeActivatePressed();
    void modeDeactivatePressed();
    void modeTrianglePressed();
    void modeTrapezoidPressed();
    void manualNameChanged();
    void pointChanged();  //!< called when a membership point is moved, NOT finally released

signals:
    void updateBrainEditor(); //!< emitted when editors should update
    void updateGLContent(); //!< emitted when openGL content should update

private:
    Ui::FuzzyEditor *ui;
};

#endif // FUZZYEDITOR_H
