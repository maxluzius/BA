#ifndef PARTICLE_H
#define PARTICLE_H

#include "camera.h"
#include "GL/glew.h"
#include <iostream>
#include <vector>
#include <QFile>
#include <QTextStream>
#include <time.h>

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
    void genParticlesPos(Camera& camera);
    void genParticlesLookAt(cv::Point3f position);
    void writeFile(cv::Point3f position, cv::Point3f lookAt, QString name);
    void setNewPose(cv::Point3f newPose);
    void setMaxRange(float x, float y, float z);
    void setMinRange(float x, float y, float z);
    float getMaxRangeX();
    float getMaxRangeY();
    float getMaxRangeZ();
    float* getMaxRange();
    float getMinRange();
    void setRanges(float x, float y, float z, float r);

private:
    int dim;
    int nParticles;

    float xRange;
    float yRange;
    float zRange;
    float maxRange[3];
    float minRange[3];
};

#endif // PARTICLE_H
