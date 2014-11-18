#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include "GL/glew.h"
#define QT_NO_OPENGL_ES_2

#include <QWidget>
#include <QtOpenGL/QGLWidget>
#include <QPainter>
#include <QEvent>
#include <QFrame>
#include <QThreadPool>
#include <QThread>
#include "Particle.h"
#include "mesh.h"
#include "renderer.h"

#include <glm/glm.hpp>
#include "Shader.h"

#include "camera.h"

#include <opencv2/opencv.hpp>
#include <QtOpenGL/QGLFramebufferObject>

class MainWindow;

/**
 * @brief The OpenGLWidget class
 * This class is the OGL Widget which is embedded in the main window of Qt.
 * The class provides the OGL context and initializes GLEW for OpenGL Extensions.
 * Qt Opengl Functions are not used
 */
class OpenGLWidget : public QGLWidget
{
    Q_OBJECT

public:
    /**
     * @brief OpenGLWidget ctr
     * @param parent parent widget
     */
    explicit OpenGLWidget(QWidget *parent = 0);

    /**
      * Dtr
      */
    ~OpenGLWidget();

    /**
     * @brief set the camera pose of this scene!
     * @param centerX
     * @param centerY
     * @param centerZ
     * @param lookAtX
     * @param lookAtY
     * @param lookAtZ
     * @param upX
     * @param upY
     * @param upZ
     */
    void setCameraPose(float centerX, float centerY, float centerZ,
                       float lookAtX, float lookAtY, float lookAtZ,
                       float upX,     float upY,     float upZ);

    /**
     * @brief set the Projection for this scene
     * @param fov
     * @param nearPlane
     * @param farPlane
     * @param isPerspective
     */
    void setProjection(float fov, float nearPlane, float farPlane, bool isPerspective = true);

    /**
     * @brief Returns the camera pose. Used for saving the scene to an XML file
     * @param centerX
     * @param centerY
     * @param centerZ
     * @param lookAtX
     * @param lookAtY
     * @param lookAtZ
     * @param upX
     * @param upY
     * @param upZ
     */
    void getCameraPose(float& centerX, float& centerY, float& centerZ,
                       float& lookAtX, float& lookAtY, float& lookAtZ,
                       float& upX,     float& upY,     float& upZ);

    /**
     * @brief Returns the camera projection. Used for saving the scene to an XML file
     * @param fov
     * @param nearPlane
     * @param farPlane
     * @param isPerspective
     */
    void getProjection(float& fov, float& nearPlane, float& farPlane, bool& isPerspective);

    /**
     * @brief Returns the Transformation of the object
     * @param transX
     * @param transY
     * @param transZ
     * @param pitch
     * @param yaw
     * @param roll
     * @param scaleX
     * @param scaleY
     * @param scaleZ
     */
    void getTransformation(float& transX, float& transY, float& transZ,
                           float& pitch,  float& yaw,    float& roll,
                           float& scaleX, float& scaleY, float& scaleZ);

    enum
    {
      TYPE_IMAGE, ///< The background of the scene is an Image
      TYPE_VIDEO  ///< The background of the scene is a  Video
    };
    /**
     * @brief get Scene Type
     * @return the type of the scene TYPE_IMAGE or TYPE_VIDEO
     */
    int getSceneType();

    /**
     * @brief Set the type of the scene TYPE_IMAGE or TYPE_VIDEO
     * @param type
     */
    void setSceneType(int type);

    /**
     * @brief Set the filename of the scene
     * @param filename
     */
    void setFilename(std::string filename);

    /**
     * @brief Get the name of the file
     * @return
     */
    std::string& getFilename();

    /**
     * @brief set the type of the object("obj" for example)
     * @param objectType
     */
    void setObjectType(std::string objectType);

    /**
     * @brief get the data type of the object ("obj")
     * @return the data type of the object
     */
    std::string& getObjectType();

    /**
     * @brief set the path to the object
     * @param filename
     */
    void setObjectPath(std::string filename);

    /**
     * @brief Get the path to the object
     * @return
     */
    std::string& getObjectPath();

    /**
     * @brief Initialize the scene
     */
    void initScene();

    /**
     * @brief increase the current frame
     */
    void incFrame();

    /**
     * @brief decrease the current frame
     */
    void decFrame();

    /**
     * @brief set the current Frame
     * @param frame
     */
    void setFrame(int frame);

    /**
     * @brief Is the video playing?
     * @return true if the video is playing
     */
    bool getIsPlaying() const;

    /**
     * @brief start or stop the video
     * @param value
     */
    void setIsPlaying(bool value);

    /**
     * @brief Get the Mesh rendered in the scene
     * @return
     */
    Mesh *getMesh();

    /**
     * @brief set the Mesh to render
     * @param value
     */
    void setMesh(Mesh *value);

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

    /**
     * @brief event handling for mouse input
     * @param event
     */
    void mouseMoveEvent(QMouseEvent * event);

    /**
     * @brief If true the camera is editable
     * @param editable
     */
    void setCameraEdit(bool editable);

    /**
     * @brief Get the Camera used in the scene
     * @return the camera
     */
    Camera* getCamera();

    /**
     * @brief Set the camera for the scene
     * @param value
     */
    void setCamera(const Camera &value);

    void generateTextureMap();

private:
    float _fov,_nearPlane,_farPlane,_isPerspective;
    particle camPartikel;
    renderer renderObj;
    cv::Point2i loop;
    GLuint pbufferList;
    float relMaxValue;
    glm::vec3 newCenter;
    glm::vec3 newLookAt;
    GLuint framebufferHandle;
    GLuint handle;

    QGLFramebufferObject *fbo;

    /**
     * @brief Do a perspective projection
     */
    void perspectiveProjection();

    Shader* _program; ///< Shader program for the mesh
    Shader* _imageProgram; ///< Shader program for the screen filling triangle
    Shader* _sobelProgram; ///< Shader program for sobel filtering
    Shader* _meshProgram; ///< Shader program for the teaboxmesh
    Shader* _thinningProgram; ///< Shader program for thinning
    Shader* _medianProgram; ///< Shader program for blur

    int sceneType; ///< the type of the scene TYPE_IMAGE or TYPE_VIDEO
    std::string filename; ///< the filename of the scene
    std::string objectType; ///< Type of the Object ("obj")
    std::string objectPath; ///< Path to the object

    bool isPlaying; ///< Is a video playing?

    /**
     * @brief Qt function. Just calls the draw function
     */
    void paintGL();

    /**
     * @brief Qt function. Viewport and perspective transform
     * @param w
     * @param h
     */
    void resizeGL(int w, int h);

    /**
     * @brief Qt function. Initialize OpenGL context and GLEW
     */
    void initializeGL();

    /**
     * @brief draws the scene
     */
    virtual void draw();

    MainWindow* parentWindow;

    /**
     * @brief update some Scene values
     */
    void updateScene();

    cv::Mat* img; ///< The image (or current frame)
    cv::VideoCapture video; ///< Video caputre - if any
    QImage qframe; ///< Frame of Qt used to transform image data in an OpenGL Texture
    cv::Mat input;
    cv::Mat out;
    /**
     * @brief Creates and initializes a texture for OpenGL
     */
    void initGLTexture();

    /**
     * @brief update texture information
     */
    void updateTex();

    /**
     * @brief next Frame in the video: Undistort image, do image processing, Compute LEHFs of new frame and match lines
     */
    void nextFrame();

    float _aspect; ///< The aspect ratio of the camera
    float _viewportWidth; ///< Width of the VP
    float _viewportHeight; ///< Height of the VP

    int w,h; ///< Image Width Height

    int _fpsVideo; ///< The FPS of the video - if any
    int _frameCountVideo; ///< the Number of video frames (video only)
    int _frame; ///< The current frame (video only)

    cv::Mat* videoMats; ///< The complete video as images. This is HUGE - be careful with your memory here :)

    glm::mat4 m,v,p; ///< Model, View, Projectionmatrix
    GLuint mLoc,pLoc,vLoc; ///< Location of Model, View, Projectionmatrix in the shader program

    Mesh* mesh; ///< the loaded mesh
    Camera camera; ///< Camera

    QTimer* timer;  // Qt timer to frequently call updateGL()

    /**
     * @brief The SFT (Screen Filling Triangle) struct
     * Combines all information an SFT needs
     */
    struct SFT
    {
        float vertices[6];
        GLuint vao;
        GLuint texID;
        GLuint texLoc;
    };
    SFT screenFillingTri; ///< Screen Filling Triangle

};

#endif // OPENGLWIDGET_H
