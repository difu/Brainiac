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
    void setSegmentConfig(AgentManager *manager, quint32 id);
    void updateEditor();

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
     * @brief creates all dimensionsliders, also radius slider of sphere primitive
     *
     * @fn createDimensionSliders
     */
    void createDimensionSliders();

    AgentManager *m_agentManager;
    quint32 m_id;

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
