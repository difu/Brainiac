#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindowLayout;
class Scene;
class QComboBox;


/** \brief  Main User Interface

        This class is the main GUI of the application.
        It provides places for e.g. brain editor, animations etc.

**/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Scene *scene, QWidget *parent = 0);
    enum EditMode{ BRAIN, BODY, SCENE };
    EditMode getEditMode() { return m_editMode; }
    void setScene(Scene *scene) { m_scene=scene; } //!< Setter method for the scene to be handled
    void setEditMode(EditMode em);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);
    void createEditModeWidgets();
    void createEditorItemBars();
    Scene *m_scene; //!< the scene to be handled
    EditMode m_editMode;
    QComboBox *m_editModeComboBox;
    MainWindowLayout *m_layout;

    QWidget *m_editorItems;
    QWidget *m_brainEditorItems;
    QWidget *m_bodyEditorItems;
    QWidget *m_sceneEditorItems;

protected slots:
    void editModeComboChange(int index);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
