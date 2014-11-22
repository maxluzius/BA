#ifndef RENDER_H
#define RENDER_H

#include "camera.h"
#include "Particle.h"
#include "mesh.h"
#include <glm/glm.hpp>
#include "Shader.h"
#include "GL/glew.h"

#include <QWidget>
#include <QtOpenGL/QGLWidget>
#include <QPainter>
#include <QEvent>
#include <QFrame>
#include <QThreadPool>
#include <QThread>
#include "Particle.h"
#include "mesh.h"

#include <glm/glm.hpp>
#include "Shader.h"

#include "camera.h"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <QCoreApplication>
#include <QMainWindow>
#include "QTimer"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QtOpenGL/QGLFramebufferObject>

class renderer
{
public:
    renderer();
    void sobel();
    void rendern(Mesh* mesh);
    void renderMeshes(Camera &camera,Mesh* mesh, particle camPartikel, Shader* _program, Shader* _meshProgram, glm::mat4 m, glm::mat4 v, glm::mat4 p, GLuint mLoc,GLuint vLoc,GLuint pLoc, QGLFramebufferObject *fbo, GLuint handle, float &relMaxValue, glm::vec3 &newCenter, glm::vec3 &newLookAt);
    int count(cv::Mat img);
    void likelihood(int videoCount, int particleCount);
    void initQuery();

private:
    glm::vec3 cameraPos;
    glm::vec3 cameraLookAt;
    Camera camera;
    GLuint texLoc;
    int pixelCounterMesh;
    int maxLikelihood;
    GLuint PixelCountSet;
    GLuint PixelCountAll;
    GLuint isDone;
    float max;
    float bestRelVal;

};

#endif // RENDER_H
