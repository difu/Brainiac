#include "animation.h"
#include "core/agent/body/animation/animationcurve.h"

Animation::Animation()
{
}

Animation::Animation(const Animation &animation)
{
    QHashIterator<QString, AnimationCurve *> i(animation.curves()) ;

    while(i.hasNext()) {
        i.next();
        AnimationCurve *newCurve= new AnimationCurve(*i.value());
        m_curves.insert(i.key(),newCurve);
    }
}

Animation::Animation(QHash<QString, AnimationCurve *> curves, QString name="")
{
    m_name=name;
    m_curves=curves;
    m_isLoopedAnimation=true;
}

Animation::~Animation()
{
    foreach(AnimationCurve *c,m_curves) {
        delete c;
    }
    m_curves.clear();
}
