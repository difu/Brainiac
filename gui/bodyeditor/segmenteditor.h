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


#ifndef SEGMENTEDITOR_H
#define SEGMENTEDITOR_H

#include <QWidget>
#include "gui/itemeditorwidgetsbase.h"


class AgentManager;
class BrainiacSlider;

namespace Ui {
class SegmentEditor;
}

/**
 * @brief
 @todo Limits DOF
 *
 * @class SegmentEditor segmenteditor.h "gui/bodyeditor/segmenteditor.h"
 */
class SegmentEditor : public QWidget, public ItemEditorWidgetsBase
{
    Q_OBJECT

public:
    explicit SegmentEditor(Scene *scene, QWidget *parent = 0);
    ~SegmentEditor();
    bool editSymetric() const { return m_editSymetric; }
    void setSegmentConfig(AgentManager *manager, quint32 id);
    void updateEditor();

public slots:
    /**
     * @brief enables/disables symetric editing
     *
     * @fn editSymetric
     * @param editSymetric
     */
    void editSymetric(bool editSymetric);

protected:

    /**
     * @brief creates all sliders for setting the rest rotation of this segment
     *
     * @fn createRestRxSliders
     */
    void createRestRxSliders();

    /**
     * @brief creates all sliders for setting the rest translation of this segment
     *
     * @fn createRestTxSliders
     */
    void createRestTxSliders();

    /**
     * @brief creates all rotation sliders
     *
     * @fn createRxSliders
     */
    void createRxSliders();

    /**
     * @brief creates all translation sliders
     *
     * @fn createTxSliders
     */
    void createTxSliders();

    /**
     * @brief creates all dimensionsliders
     * ,Includes radius, diameter, length slider of sphere and tube primitive
     *
     * @fn createDimensionSliders
     */
    void createDimensionSliders();

    quint32 getSymetricSegmentId() const;

    AgentManager *m_agentManager;
    quint32 m_id;
    bool m_editSymetric;

    BrainiacSlider *m_SliderRestRx;
    BrainiacSlider *m_SliderRestRy;
    BrainiacSlider *m_SliderRestRz;

    BrainiacSlider *m_SliderRx;
    BrainiacSlider *m_SliderRy;
    BrainiacSlider *m_SliderRz;

    BrainiacSlider *m_SliderTx;
    BrainiacSlider *m_SliderTy;
    BrainiacSlider *m_SliderTz;

    BrainiacSlider *m_SliderRestTx;
    BrainiacSlider *m_SliderRestTy;
    BrainiacSlider *m_SliderRestTz;

    BrainiacSlider *m_SliderDimensionX;
    BrainiacSlider *m_SliderDimensionY;
    BrainiacSlider *m_SliderDimensionZ;

    BrainiacSlider *m_SliderSphereRadius;

    BrainiacSlider *m_SliderTubeLength;
    BrainiacSlider *m_SliderTubeDiameter;

protected slots:
    /**
     * @brief called when any rest translation has changed e.g. by slider
     *
     * @fn manualRestTransChanged
     * @param value
     */
    void manualRestTransChanged(qreal value);
    /**
     * @brief called when any rest rotation has changed e.g. by slider
     *
     * @fn manualRestRotChanged
     * @param value
     */
    void manualRestRotChanged(qreal value);
    /**
     * @brief called when any rotation has changed e.g. by slider
     *
     * @fn manualRotChanged
     * @param value
     */
    void manualRotChanged(qreal value);
    /**
     * @brief called when any translation has changed e.g. by slider
     *
     * @fn manualTransChanged
     * @param value
     */
    void manualTransChanged(qreal value);

    /**
     * @brief alled when any dimension (except radius!) has changed e.g. by slider
     *
     * @fn manualDimensionsChanged
     * @param value
     */
    void manualDimensionsChanged(qreal value);
private:
    Ui::SegmentEditor *ui;

signals:
    void updateBodyEditor(); //!< emitted when editors should update
    void updateGLContent(); //!< emitted when openGL content should update
};

#endif // SEGMENTEDITOR_H
