#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include "editcamerawidget.h"

const float PI = 3.1415926535f;
const float PI2 = 6.283185307f;

Camera::Camera()
{
    _phiSensititvity = 0.001f;
    _thetaSensititvity = 0.001f;
    _thetaEps = 0.0001f;
   _theta = 0.f;
   _phi = 0.f;
   _velocity = 0.1f;

    lastPos=glm::vec2(0.f,0.f);
    _isEditable = false;

    _trackball = true;
}


void Camera::setCenterX(float x)
{
    _center.x = x;
    init();
}

void Camera::setCenterY(float y)
{
    _center.y = y;
    init();
}

void Camera::setCenterZ(float z)
{
    _center.z = z;
    init();
}

void Camera::setLookAtX(float x)
{
    _lookAt.x = x;
    init();
}

void Camera::setLookAtY(float y)
{
    _lookAt.y = y;
    init();
}

void Camera::setLookAtZ(float z)
{
    _lookAt.z = z;
    init();
}

void Camera::setUpX(float x)
{
    _up.x = x;
    init();
}

void Camera::setUpY(float y)
{
    _up.y = y;
    init();
}

void Camera::setUpZ(float z)
{
    _up.z = z;
    init();
}
bool Camera::trackball() const
{
    return _trackball;
}

void Camera::setTrackball(bool trackball)
{
    _trackball = trackball;
}

void Camera::init()
{
    glm::vec3 direction = -glm::normalize(_lookAt-_center);

    _theta = (float)acos(direction.y);
    if(_theta < _thetaEps) _theta = _thetaEps;
    else if(_theta > PI - _thetaEps) _theta = PI - _thetaEps;

    _phi = (float)atan2(direction.z,direction.x);
    if(_phi < 0.f) _phi += PI2;
    else if(_phi > PI2) _phi -= PI2;
}
EditCameraWidget *Camera::editCameraWidget() const
{
    return _editCameraWidget;
}

void Camera::setEditCameraWidget(EditCameraWidget *editCameraWidget)
{
    _editCameraWidget = editCameraWidget;
}

glm::vec3 Camera::center() const
{
    return _center;
}

void Camera::setCenter(const glm::vec3 &center)
{
    _center = center;
}

glm::vec3 Camera::lookAt() const
{
    return _lookAt;
}

void Camera::setLookAt(const glm::vec3 &lookAt)
{
    _lookAt = lookAt;
}

glm::vec3 Camera::up() const
{
    return _up;
}

void Camera::setUp(const glm::vec3 &up)
{
    _up = up;
}

glm::vec3 Camera::getDirection()
{
    return glm::normalize(_lookAt-_center);
}

void Camera::keyEventOccured(QKeyEvent *event)
{
    if(!isEditable())
        return;

    glm::vec3 direction = getDirection();
    glm::vec3 right = glm::cross(direction,_up);

    int key = event->key();

    switch(key)
    {
    case Qt::Key_W:
        if(_trackball)
            key = Qt::Key_PageDown;
        else{
            _center += _velocity*direction;
            break;
        }
    case Qt::Key_PageDown:
        _center -= _velocity*_up;
        if(_trackball) _lookAt -= _velocity*_up;
        break;
    case Qt::Key_S:
        if(_trackball)
            key = Qt::Key_PageUp;
        else
        {
            _center -= _velocity*direction;
            break;
        }
    case Qt::Key_PageUp:
        _center += _velocity*_up;
        if(_trackball) _lookAt += _velocity*_up;
        break;
    case Qt::Key_A:
        if(_trackball)
        {
            _center += _velocity*right;
            _lookAt += _velocity*right;
        }
        else
            _center += _velocity*-right;
        break;
    case Qt::Key_D:
        if(_trackball)
        {
            _center -= _velocity*right;
            _lookAt -= _velocity*right;
        }
        else
            _center += _velocity*right;
        break;
    }

    if(!_trackball)
        setLookAt(_center + direction);

    if(_editCameraWidget)
         _editCameraWidget->setCamera(this);
}

void Camera::mouseEventOccured(QMouseEvent *event)
{
    if(!isEditable())
        return;

    glm::vec3 direction = _lookAt-_center;
    float distFromCenter = glm::length(direction);
    direction = glm::normalize(direction);


    if(!_trackball)
    {
        _theta = (float)acos(direction.y);
        if(_theta < _thetaEps) _theta = _thetaEps;
        else if(_theta > PI - _thetaEps) _theta = PI - _thetaEps;

        _phi = (float)atan2(direction.z,direction.x);
        if(_phi < 0.f) _phi += PI2;
        else if(_phi > PI2) _phi -= PI2;
    }


    float dx = event->x() - lastPos.x;
    float dy = event->y() - lastPos.y;

    if(_trackball)
        dy = -dy;

    if(event->buttons() & Qt::MiddleButton)
    {
        _theta += _thetaSensititvity * dy;
        if(_theta < _thetaEps) _theta = _thetaEps;
        else if(_theta > PI - _thetaEps) _theta = PI - _thetaEps;

        _phi += _phiSensititvity * dx;
        if(_phi < 0.f) _phi += PI2;
        else if(_phi > PI2) _phi -= PI2;

        if(!_trackball)
        {
            direction += glm::vec3(sin(_theta)*cos(_phi),cos(_theta),sin(_theta)*sin(_phi));
            setLookAt(_center + direction);
        }
        else
        {
            _center = glm::vec3(distFromCenter*sin(_theta)*cos(_phi),distFromCenter*cos(_theta),distFromCenter*sin(_theta)*sin(_phi));
            _center += _lookAt;
        }

        if(_editCameraWidget)
           _editCameraWidget->setCamera(this);
    }

    lastPos.x = event->x();
    lastPos.y = event->y();
}

void Camera::wheelEventOccured(QWheelEvent *event)
{
    if(!isEditable())
        return;

    if(!_trackball)
        return;

    float dir = (event->delta()<0) ? -0.1f : 0.1f;

    glm::vec3 direction = glm::normalize(_lookAt-_center);

    _center += dir*direction;

    if(_editCameraWidget)
         _editCameraWidget->setCamera(this);
}

bool Camera::isEditable() const
{
    return _isEditable;
}

void Camera::setIsEditable(bool isEditable)
{
    _isEditable = isEditable;
}
float Camera::fov() const
{
    return _fov;
}

void Camera::setFov(float fov)
{
    _fov = fov;
}

glm::mat4& Camera::getViewMatrix()
{
    _viewMatrix = glm::lookAt(_center, _lookAt, _up);
    return _viewMatrix;
}



