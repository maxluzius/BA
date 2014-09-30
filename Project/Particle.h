#ifndef PARTICLE_H
#define PARTICLE_H

#include "camera.h"
#include "GL/glew.h"
#include <iostream>
#include <vector>
#include <QFile>
#include <QTextStream>
#include <time.h>
#include "mesh.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/legacy/legacy.hpp>

using namespace std;

class particle
{
public:
    particle();
    void paint();
    void initParticle(Camera& camera);
    void genParticles(glm::vec3 particleV);
    void genParticles(glm::vec3 particleV, cv::Point3f partPt);
    void writeFile(cv::Point3f position, cv::Point3f lookAt, QString name);
    void setMaxRange(float x, float y, float z);
    void setMinRange(float x, float y, float z);
    float getMaxRangeX();
    float getMaxRangeY();
    float getMaxRangeZ();
    float* getMaxRange();
    float getMinRange();
    void setRanges(float x, float y, float z, float r);
    void print(float x, float y, float z);
    void setCenter(Camera& camera);
    void setLookAt(Camera& camera);
    glm::vec3 getParticleCenter();
    glm::vec3 getParticleLookAt();
    void setParticleCenter(cv::Point3f point);
    void setParticleLookAt(cv::Point3f point);

private:
    int dim;
    int nParticles;

    float xRange;
    float yRange;
    float zRange;
    float maxRange[3];
    float minRange[3];
    glm::vec3 centerCameraV;
    glm::vec3 lookAtCameraV;
    glm::vec3 particleCenterV;
    glm::vec3 particleLookAtV;
    Mesh* mesh;

};

#endif // PARTICLE_H
