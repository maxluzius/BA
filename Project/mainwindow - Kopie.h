#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QXmlStreamReader>

#include "editobjectwidget.h"
#include "editcamerawidget.h"


namespace Ui {
class MainWindow;
}

/**
 * @brief The MainWindow class
 * This class represents the main QT Window and the Menu
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Ui::MainWindow *getUi();
    void setUi(Ui::MainWindow *value);

    EditCameraWidget *getEditWidget();
    void setEditWidget(EditCameraWidget *value);

    // Just menu trigger
private slots:
    void on_actionLoad_Scene_triggered();

    void on_actionSave_Scene_triggered();

    void on_actionSave_Scene_As_triggered();

    void on_actionQuit_triggered();

    void on_stopFrameButton_clicked();

    void on_prevFrameButton_clicked();

    void on_nextFrameButton_clicked();

    void on_playButton_clicked(bool checked);

    void on_actionHelp_triggered();

    void on_actionEditCamera_triggered(bool checked);

    void on_actionEditObject_triggered(bool checked);

    void on_actionDo_Visibility_Check_triggered(bool checked);

private:
    Ui::MainWindow *ui;

    /**
     * @brief parseScene parses an XML file
     * @param XMLfile
     * @return true if parsing was succesful
     */
    bool parseScene(QFile& XMLfile);

    /**
     * @brief Reads the background image or video out of an XML Stream
     * @param reader
     * @return true if succesful
     */
    bool readBackgroundFile(QXmlStreamReader& reader);

    /**
     * @brief Reads camera information out of an XML Stream
     * @param reader
     * @return true if succesful
     */
    bool readCamera(QXmlStreamReader& reader);

    /**
     * @brief reads information of a vec3 out of an XML Stream
     * @param reader
     * @param x
     * @param y
     * @param z
     * @return true if succesful
     */
    bool readVec3(QXmlStreamReader& reader, float& x, float& y, float& z);


    /**
     * @brief read information out of an XML Stream which mesh is used in the scene
     * @param reader
     * @return true if succesful
     */
    bool readObject(QXmlStreamReader& reader);

    /**
     * @brief read information about the perspective transformation out of an XML Stream
     * @param reader
     * @return true if succesful
     */
    bool readProjection(QXmlStreamReader& reader);

    /**
     * @brief reads some data path out of an XML Stream
     * @param reader
     * @param path
     * @return true if succesful
     */
    bool readPath(QXmlStreamReader& reader, QString& path);

    /**
     * @brief save the current scene in an XML file
     * @param XMLfile
     * @return true if succesful
     */
    bool saveScene(QFile& XMLfile);

    /**
     * @brief readNext skips white space as QXMLStreamReader does not skip white space
     * @param reader
     */
    void readNext(QXmlStreamReader& reader);

    /**
     * @brief event handling for keyboard input
     * @param event
     */
    void keyPressEvent(QKeyEvent* event);

    /**
     * @brief event handling for mouse wheel input
     * @param event
     */
    void wheelEvent(QWheelEvent* event);

    std::string openFile; ///< file name of the current opened scene

    EditCameraWidget* editCameraWidget;
    EditObjectWidget* editObjectWidget;
    bool isSceneLoaded; ///< is a scene loaded?
};

#endif // MAINWINDOW_H

