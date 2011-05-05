#ifndef SCENE_H
#define SCENE_H

#include <QObject>

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

class Generator;

class Scene : public QObject
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);
    bool openConfig(const QString & fileName);
    bool saveConfig(const QString & fileName);

protected:
    QString m_fileName;
    QXmlStreamWriter m_streamWriter;
    QXmlStreamReader m_steamReader;
    QList<Generator *> m_generators;

signals:

public slots:

};

#endif // SCENE_H
