#ifndef SCENE_H
#define SCENE_H

#include <QObject>

//class QString;

class Scene : public QObject
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);
    bool openConfig( QString fileName );
    bool saveConfig();

protected:
    QString m_fileName;

signals:

public slots:

};

#endif // SCENE_H
