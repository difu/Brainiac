#include "scene.h"

Scene::Scene(QObject *parent) :
    QObject(parent)
{
}


bool Scene::openConfig(QString fileName)
{
    return true;
}


bool Scene::saveConfig()
{
    return true;
}
