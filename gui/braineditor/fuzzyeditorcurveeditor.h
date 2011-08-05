#ifndef FUZZYEDITORCURVEEDITOR_H
#define FUZZYEDITORCURVEEDITOR_H

#include "core/agent/brain/fuzzyfuzz.h"
#include <QGraphicsScene>
#include <QGraphicsPathItem>

class FuzzyEditorCurvePoint;
class AgentManager;

/** \brief  Editor to modify the points that define the membership function of a fuzz rule
  @sa FuzzyFuzz
  @sa FuzzyEditor
**/
class FuzzyEditorCurveEditor : public QGraphicsScene
{
    Q_OBJECT
public:
    FuzzyEditorCurveEditor();

    /** \brief  returns the id of the currently selected membership point
    **/
    quint32 getPointId() const { return m_pointId; }

    /** \brief  sets the configuration of the fuzz rule node to be edited
            @param manager the Agentmanager
            @param fuzz pointer to the fuzz rule
            @sa FuzzyFuzz
    **/
    void setConfig(AgentManager *manager, FuzzyFuzz *fuzz);

    /** \brief  updates the editor
            it triggers a redraw of the curve of the fuzz node and also all connected fuzz nodes
    **/
    void updateEditor();

protected:
    FuzzyFuzz* m_fuzz; //!< the fuzz to be edited
    AgentManager *m_agentManager; //!< AgentManager that holds all agents that have the fuzz
    QGraphicsPathItem *m_fuzzCurve; //!< the curve of the membership function of the fuzz node
    FuzzyEditorCurvePoint *m_curvePoint; //!< draggable point to edit membership curve
    quint32 m_pointId; //!< point id of selected point
    quint32 m_points; //!< number of points of current selected membership function
    qreal m_reducedWidth; //!< reduced width of scene. Necessary to fit last point into scene
    void keyPressEvent(QKeyEvent *event); //!< reveiving cursor keys to change m_curvePoint
    /** \brief  adjusts the y ordinate of the curve to the boundaries of the editor
            @returns the adjusted y ordinate
    **/
    qreal mapYintoEditor(qreal yPos);

protected slots:
    /** \brief called, wenn a membership point is moved
            the changed point is written to the node of the master agent of the agentmanager, NOT to the other agents. \n
            For performance reasons this is done when the point is released
            @sa FuzzyEditorCurveEditor::pointReleased
    **/
    void pointMoved(qreal xPos);

    /** \brief called, wenn a membership point is released
            the changed point is written to the node of the master agent of the agentmanager and to all other agents.
    **/
    void pointReleased();

signals:
    void pointChange(); //!< emitted when a membership point is changed
};

#endif // FUZZYEDITORCURVEEDITOR_H
