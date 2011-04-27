#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class Scene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void setScene(Scene *scene) { m_scene=scene; } //! Setter method for the scene to be handled
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);
    Scene *m_scene; //! the scene to be handled

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
