#include "particle.h"
#include "GL/glew.h"
#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/legacy/legacy.hpp>

using namespace std;

particle::particle()
{
}

void particle::paint()
    {
     glClearColor (1, 1, 1, 0);
     glColor3f (0, 1, 0);
     glPointSize (4);

     glBegin (GL_POINTS);
     glVertex2f (0,0 );

     glEnd ();


    }

void particle::calcNext()
{

}


