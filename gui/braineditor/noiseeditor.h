#ifndef NOISEEDITOR_H
#define NOISEEDITOR_H

#include "gui/itemeditorwidgetsbase.h"
#include <QWidget>

class AgentManager;
class BrainiacSlider;
class Noise;

namespace Ui {
    class NoiseEditor;
}

class NoiseEditor : public QWidget, public ItemEditorWidgetsBase
{
    Q_OBJECT

public:
    explicit NoiseEditor(Scene *scene, QWidget *parent = 0);
    void setNoiseConfig(AgentManager *manager, quint32 id);
    void updateEditor();
    ~NoiseEditor();

protected:
    void changeEvent(QEvent *e);
    AgentManager *m_agentManager;
    quint32 m_id;
    Noise *m_noise;
    BrainiacSlider *m_sliderRate;
    BrainiacSlider *m_sliderResult;
protected slots:
    void manualRateChange(qreal value);
    void manualResultChange(qreal value);
    void manualNameChange();
signals:
    void updateBrainEditor(); //!< emitted when editors should update
    void updateGLContent(); //!< emitted when openGL content should update
private:
    Ui::NoiseEditor *ui;
};

#endif // NOISEEDITOR_H
