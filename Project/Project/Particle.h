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
    void initParticle(Camera& camera);
    void genParticles(glm::vec3 particleV);
    void genParticles(glm::vec3 particleV, glm::vec3 partCenter, int count);
    void writeFile(glm::vec3 partCenter, glm::vec3 partLookAt, QString name);
    void setMaxRange(float x, float y, float z);
    void setMinRange(float x, float y, float z);
    float getMaxRangeX();
    float getMaxRangeY();
    float getMaxRangeZ();
    float* getMaxRange();
    float* getMinRange();
    cv::Mat_<float> getParticleCenterM();
    cv::Mat_<float> getParticleLookAtM();
    void setRanges(float x, float y, float z, float r);
    void print(float x, float y, float z);
    void generateViews(glm::vec3 center, glm::vec3 lookAt, int i);
    glm::mat4 * getViewArray();

private:
    int dim;
    int nParticles;


    float xRange;
    float yRange;
    float zRange;
    float maxRange[3];
    float minRange[3];
    cv::Mat_<float> particleCenterM;
    cv::Mat_<float> particleLookAtM;
    glm::vec3 centerCamera;
    glm::vec3 lookAtCamera;
    glm::mat4 * viewArray;
    glm::mat4 newViewMatrix;

};

#endif // PARTICLE_H
