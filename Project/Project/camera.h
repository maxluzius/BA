#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include <QMouseEvent>
#include <QKeyEvent>

class EditCameraWidget;

/**
    \brief Class for representing an OpenGL Camera

    Class for representing an OpenGL Camera
*/

class Camera
{
public:
    /**
        Ctor of a Camera
    */
    Camera();


    /**
        Returns the center of the Camera
        @return the center point of the camera

    */
    glm::vec3 center() const;

    /**
         Sets the center point of the camera.
         @param center the new center point of the camera
    */
    void setCenter(const glm::vec3 &center);

    /**
         Returns the point the camera is looking at
         @return the look at point of the camera
    */
    glm::vec3 lookAt() const;

    /**
        Sets the new point the camera should look at.
        @param lookAt the new look at point
    */
    void setLookAt(const glm::vec3 &lookAt);

    /**
        Returns the up vector
        @return the up vector
    */
    glm::vec3 up() const;

    /**
         Sets the up vector
         @param up the new up vector
    */
    void setUp(const glm::vec3 &up);

    /**
        Returns the cameras view direction
        @return the cameras view direction
    */
    glm::vec3 getDirection();

    /**
        Event handler for keyboard input
        @param event the key event
    */
    void keyEventOccured(QKeyEvent *event);

    /**
        Event handler for mouse input
        @param event the mouse event
    */
    void mouseEventOccured(QMouseEvent *event);

    /**
         Event handler for mouse wheel input
         @param event the mouse wheel event
    */
    void wheelEventOccured(QWheelEvent* event);

    /**
        Is the Camera editable by the user?
        @return is the camera editable
    */
    bool isEditable() const;

    /**
       Defines whether or not the camera is editable
       @param isEditable
    */
    void setIsEditable(bool isEditable);

    /**
        Returns the field of view
        @return field of view
    */
    float fov() const;

    /**
         Sets the camera's field of view
         @param fov
    */
    void setFov(float fov);

    /**
        Changes only the camera's x Position.
        @param x
    */
    void setCenterX(float x);
    /**
        Changes only the camera's y Position.
        @param y
    */
    void setCenterY(float y);
    /**
        Changes only the camera's z Position.
        @param z
    */
    void setCenterZ(float z);

    /**
        Changes only the camera's x look at point.
        @param x
    */
    void setLookAtX(float x);

    /**
        Changes only the camera's y look at point.
        @param y
    */
    void setLookAtY(float y);

    /**
        Changes only the camera's z look at point.
        @param z
    */
    void setLookAtZ(float z);

    /**
        Changes only the camera's up vector x component.
        @param x
    */
    void setUpX(float x);

    /**
        Changes only the camera's up vector y component.
        @param y
    */
    void setUpY(float y);

    /**
        Changes only the camera's up vector z component.
        @param z
    */
    void setUpZ(float z);

    /**
        Is the trackball mode used?
        @return is the trackball mode used?
    */
    bool trackball() const;

    /**
        Sets whether or not the trackball mode should be used
        @param trackball
    */
    void setTrackball(bool trackball);

    /**
        Initialize the camera
    */
    void init();

    /**
        QT widget to edit camera properties
        @return the QT Widget
    */
    EditCameraWidget *editCameraWidget() const;

    /**
     * @brief sets the EditCameraWidget
     * @param editCameraWidget
     */
    void setEditCameraWidget(EditCameraWidget *editCameraWidget);

    /**
        Returns the 4x4 view matrix in glm format
        @return the view matrix
    */
    glm::mat4& getViewMatrix();

private:
    bool _trackball; ///< Trackball mode?

    glm::vec3 _center,_lookAt,_up; ///< extrinsic parameters
    float _fov; ///< The Field of View

    float _phi,_theta; ///< spherical coordinates

    float _phiSensititvity;
    float _thetaSensititvity;

    float _velocity;

    float _thetaEps; ///< avoid to look straight up/down

    glm::vec2 lastPos; ///< Mouse Position of the last frame

    bool _isEditable; ///< Is the camera editable by the user through a QT Widget?

    glm::mat4 _viewMatrix; ///< The view matrix

    EditCameraWidget* _editCameraWidget; ///< The QT Widget to edit the camera
};

#endif // CAMERA_H
