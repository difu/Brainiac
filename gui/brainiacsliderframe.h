#ifndef BRAINIACSLIDERFRAME_H
#define BRAINIACSLIDERFRAME_H

#include <QFrame>

class BrainiacSliderFrame : public QFrame
{
    Q_OBJECT
public:
    BrainiacSliderFrame(QWidget *parent);

    void setVolume(qreal param_value);
    void markZero(qreal param_zero) {m_zeroMark=param_zero;}
    void setSliderColor(QColor param_col) {m_sliderColor=param_col;}
    void setText(QString text);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    int m_pressedXPos;
    qreal m_value;
    bool m_mousePressed;
    float m_zeroMark;
    QColor m_sliderColor;
    QString m_text;
signals:
    void valueChanged(qreal value);
    void valueChangedAccepted(qreal value);
};

#endif // BRAINIACSLIDERFRAME_H
