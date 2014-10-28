#include "mesh.h"
#include "GL/glew.h"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/euler_angles.hpp>
#include "Line.h"
#include "line3d.h"

Mesh::Mesh()
{
    translate = glm::vec3(0.f,0.f,0.f);
    rotate = glm::vec3(0.f,0.f,0.f);
    scale = glm::vec3(1.f,1.f,1.f);

    _doVisibilityCheck = true;
}

Mesh::~Mesh()
{
    glDeleteBuffers(1,&_bufferVert);
    glDeleteBuffers(1,&_bufferIndices);
    glDeleteBuffers(1,&_vao);
}

void Mesh::init()
{
    glGenVertexArrays(1,&_vao);
    glBindVertexArray(_vao);

    glGenBuffers(1,&_bufferVert);
    glBindBuffer(GL_ARRAY_BUFFER,_bufferVert);
    glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(float),
                 vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

void Mesh::draw(glm::vec3 *camPos)
{
    glBindVertexArray(_vao);

    if(_doVisibilityCheck)
    {

        assert(camPos && V && P);
        _visibleIndices.clear();
        _visibleLines.clear();
        for(int i=0; i<lines.size(); i++)
        {
            if(lines[i]->isVisible(*camPos,M))
            {
                cv::Vec2i ind = lines[i]->indices();
                _visibleIndices.push_back(ind[0]);
                _visibleIndices.push_back(ind[1]);

                _visibleLines.push_back(lines[i]);
            }
        }

        //to debug GL_TRIANGLE_STRIP
        glDrawElements(GL_LINES,_visibleIndices.size(),GL_UNSIGNED_INT,_visibleIndices.data());
    }
    else
        glDrawElements(GL_LINES,indices.size(),GL_UNSIGNED_INT,indices.data());

    glBindVertexArray(0);
}
glm::vec3 Mesh::getTranslate() const
{
    return translate;
}

void Mesh::setTranslate(const glm::vec3 &value)
{
    translate = value;
}

void Mesh::setTranslateX(const float x)
{
    translate.x = x;
}

void Mesh::setTranslateY(const float y)
{
    translate.y = y;
}

void Mesh::setTranslateZ(const float z)
{
    translate.z = z;
}

glm::vec3 Mesh::getRotate() const
{
    return rotate;
}

void Mesh::setRotate(const glm::vec3 &value)
{
    rotate = value;
}

void Mesh::setRotateX(const float x)
{
    rotate.x = x;
}

void Mesh::setRotateY(const float y)
{
    rotate.y = y;
}

void Mesh::setRotateZ(const float z)
{
    rotate.z = z;
}

glm::vec3 Mesh::getScale() const
{
    return scale;
}

void Mesh::setScale(const glm::vec3 &value)
{
    scale = value;
}

void Mesh::setScaleX(const float x)
{
    scale.x = x;
}

void Mesh::setScaleY(const float y)
{
    scale.y = y;
}

void Mesh::setScaleZ(const float z)
{
    scale.z = z;
}

float Mesh::getYaw() const
{
    return rotate.y * 0.0174532925f;
}

float Mesh::getRoll() const
{
    return rotate.x * 0.0174532925f;
}

float Mesh::getPitch() const
{
    return rotate.z * 0.0174532925f;
}

EditObjectWidget *Mesh::editObjectWidget() const
{
    return _editObjectWidget;
}

void Mesh::setEditObjectWidget(EditObjectWidget *editObjectWidget)
{
    _editObjectWidget = editObjectWidget;
}

bool Mesh::hasNormals() const
{
    return _hasNormals;
}

void Mesh::setHasNormals(bool hasNormals)
{
    _hasNormals = hasNormals;
}

void Mesh::doVisibilityCheck(bool doCheck)
{
    _doVisibilityCheck =  doCheck;
}

glm::mat4& Mesh::computeModelMatrix()
{
    M = glm::mat4(1.0f);
    M = glm::translate(M,getTranslate());
    M *= glm::eulerAngleYXZ(getYaw(),getRoll(),getPitch());
    M = glm::scale(M,getScale());
    return M;
}

void Mesh::setVPWidth(int value)
{
    vpWidth = value;
}

void Mesh::setVPHeight(int value)
{
    vpHeight = value;
}

void Mesh::setP(glm::mat4 *value)
{
    P = value;
}

void Mesh::setV(glm::mat4 *value)
{
    V = value;
}












