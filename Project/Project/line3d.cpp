#include "line3d.h"
#include "Line.h"

Line3D::Line3D()
{
    _normal = _normalSec = glm::vec3(0.f,0.f,0.f);
}

Line *Line3D::project(glm::mat4 &M, glm::mat4 &V, glm::mat4 &P, int w, int h)
{
    int w2 = w/2;
    int h2 = h/2;
    glm::mat4 VPTranform = glm::mat4(w2,0 ,0 ,w2,
                                     0 ,h2,0 ,h2,
                                     0, 0, 0.5, 0.5,
                                     0, 0, 0, 1);

    glm::vec4 pixelCoordA = glm::transpose(VPTranform)*P*V*M*glm::vec4(_p0.x,_p0.y,_p0.z,1);
    pixelCoordA = pixelCoordA / pixelCoordA[3];
    pixelCoordA[1] = h - pixelCoordA[1];

    glm::vec4 pixelCoordB = glm::transpose(VPTranform)*P*V*M*glm::vec4(_p1.x,_p1.y,_p1.z,1);
    pixelCoordB = pixelCoordB / pixelCoordB[3];
    pixelCoordB[1] = h - pixelCoordB[1];

    _projected = new Line(cv::Vec4i(pixelCoordA[0],pixelCoordA[1],pixelCoordB[0],pixelCoordB[1]));
    _projected->setLineIn3D(this);

    return _projected;
}

bool Line3D::isVisible(glm::vec3 &cameraPos, glm::mat4 &M)
{
    glm::vec4 n1 = M*glm::vec4(_normal,0);
    glm::vec4 n2 = M*glm::vec4(_normalSec,0);

    glm::vec4 p0 = M*glm::vec4(_center,1);

    p0 -= glm::vec4(cameraPos,1);

    float r1 = glm::dot(glm::vec3(p0),glm::vec3(n1));
    float r2 = glm::dot(glm::vec3(p0),glm::vec3(n2));

    if(r1 < 0 || r2 < 0)
        return true;

    return false;
}

void Line3D::setStartEnd(cv::Point3f p0, cv::Point3f p1)
{
    _p0 = p0;
    _p1 = p1;
}

glm::vec3 Line3D::center() const
{
    return _center;
}

void Line3D::setCenter(const glm::vec3 &center)
{
    _center = center;
}

cv::Vec2i Line3D::indices() const
{
    return _indices;
}

void Line3D::setIndices(const cv::Vec2i &indices)
{
    _indices = indices;
}
glm::vec3 Line3D::normal() const
{
    return _normal;
}

void Line3D::setNormal(const glm::vec3 &normal)
{
    _normal = normal;
}

glm::vec3 Line3D::normalSec() const
{
    return _normalSec;
}

void Line3D::setNormalSec(const glm::vec3 &normalSec)
{
    _normalSec = normalSec;
}
Line *Line3D::projected() const
{
    return _projected;
}

void Line3D::setProjected(Line *projected)
{
    _projected = projected;
}
cv::Point3f Line3D::p0() const
{
    return _p0;
}

void Line3D::setP0(const cv::Point3f &p0)
{
    _p0 = p0;
}
cv::Point3f Line3D::p1() const
{
    return _p1;
}

void Line3D::setP1(const cv::Point3f &p1)
{
    _p1 = p1;
}








