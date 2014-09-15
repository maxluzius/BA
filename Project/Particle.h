#ifndef PARTICLE_H
#define PARTICLE_H
#include "camera.h"

class particle
{
public:
    particle();
    void paint();
    void genParticles(Camera& camera);
    void writeFile();
//    void setMaxRange(float x, float y, float z);
//    void setMinRange(float x, float y, float z);
//    float getMaxRange();
//    float getMinRange();

private:
    int dim;
    int nParticles;
//    float xRange;
//    float yRange;
//    float zRange;
//    float maxRange[3];
//    float minRange[3];
};

#endif // PARTICLE_H
