#ifndef SCENE_H
#define SCENE_H

#include <QObject>

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

class Generator;
class Group;

class Scene : public QObject
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);
    void addGroup(Group *group);
    void clear(); //!< deletes all contents from this scene
    QList<Group *> getGroups() { return m_groups; }
    bool openConfig(const QString & fileName);
    bool saveConfig(const QString & fileName);
    bool saveConfig();

protected:
    QString m_fileName;
    QXmlStreamWriter m_streamWriter;
    QXmlStreamReader m_streamReader;
    QList<Generator *> m_generators;
    QList<Group *> m_groups;

signals:
    void groupAdded(Group *group);

public slots:

};

#endif // SCENE_H
