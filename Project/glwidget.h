/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "GL/glew.h"
#define QT_NO_OPENGL_ES_2

#include <QtOpenGL/QGLFramebufferObject>
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

class GLWidget : public QGLWidget
{
public:
    GLWidget(QWidget *parent);
    ~GLWidget();
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void timerEvent(QTimerEvent *) { update(); }
    void mousePressEvent(QMouseEvent *) { killTimer(timerId); }
    void mouseReleaseEvent(QMouseEvent *) { timerId = startTimer(20); }

    void draw();

private:
    GLfloat rot[3], xOffs[3], yOffs[3], xInc[3];
    GLuint pbufferList;
    GLuint cubeTexture;
    int timerId;

    QGLFramebufferObject *fbo;

    Shader* _program;
    Shader* _imageProgram;
    glm::mat4 m,v,p; ///< Model, View, Projectionmatrix
    GLuint mLoc,pLoc,vLoc; ///< Location of Model, View, Projectionmatrix in the shader program
    Mesh* mesh; ///< the loaded mesh
    Camera camera; ///< Camera
    float _aspect;
    float _fov,_nearPlane,_farPlane,_isPerspective;
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

