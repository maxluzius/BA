#include "particle.h"
#include "camera.h"
#include "GL/glew.h"
#include <iostream>
#include <vector>
#include <QFile>
#include <QTextStream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/legacy/legacy.hpp>

using namespace std;

vector<cv::Point3i> cameraV, newCameraV;

particle::particle()
{
    dim = 3;
    nParticles = 20;
}

//void particle::setMaxRange(float x, float y, float z){
//    xRange = x;
//    yRange = y;
//    zRange = z;
//    maxRange[0] = x;
//    maxRange[1] = y;
//    maxRange[2] = z;
//}

//void particle::setMinRange(float x, float y, float z){

//}

void particle::genParticles(Camera& camera)
{
    particle writeParticle;
    cv::Mat_<float> lastCam(3,1);
    lastCam(0) = camera.center().x;
    lastCam(1) = camera.center().y;
    lastCam(2) = camera.center().z;

    float xRange = 40.0;
    float yRange = 40.0;
    float zRange = 40.0;

    float minRange[] = { -40.0, -40.0, -40.0 };
    float maxRange[] = { xRange, yRange, zRange };
    CvMat LB, UB;
    cvInitMatHeader(&LB, 3, 1, CV_32FC1, minRange);
    cvInitMatHeader(&UB, 3, 1, CV_32FC1, maxRange);

      CvConDensation* condens = cvCreateConDensation(dim, dim, nParticles);

      cvConDensInitSampleSet(condens, &LB, &UB);

      //Einheitsmatrix
      condens->DynamMatr[0] = 1.0;
      condens->DynamMatr[1] = 0.0;
      condens->DynamMatr[2] = 0.0;

      condens->DynamMatr[3] = 0.0;
      condens->DynamMatr[4] = 1.0;
      condens->DynamMatr[5] = 0.0;

      condens->DynamMatr[6] = 0.0;
      condens->DynamMatr[7] = 0.0;
      condens->DynamMatr[8] = 1.0;

      cameraV.clear();
      newCameraV.clear();

      //Echte Pose
      cv::Point3i measPt(lastCam(0),lastCam(1),lastCam(2));
      cameraV.push_back(measPt);

      for (int i = 0; i < condens->SamplesNum; i++) {

         //Berechnung der Abweichung
         float diffX = (lastCam(0) - condens->flSamples[i][0])/xRange;
         float diffY = (lastCam(1) - condens->flSamples[i][1])/yRange;
         float diffZ = (lastCam(2) - condens->flSamples[i][2])/zRange;
         condens->flConfidence[i] = 1.0 / (sqrt(diffX * diffX + diffY * diffY + diffZ * diffZ));

         // Partikelstreuung werde ich benötigen
         cv::Point3i partPt(condens->flSamples[i][0], condens->flSamples[i][1], condens->flSamples[i][2]);
         //cout << "Partikel: X-Achse: " << condens->flSamples[i][0] << "/" << lastCam(0) << " Y-Achse: " << condens->flSamples[i][1] << "/" << lastCam(1)<< " Z-Achse: " << condens->flSamples[i][2] << "/" << lastCam(2)<< endl;
       }

       cvConDensUpdateByTime(condens);

       //Bester Partikel, ist aber keine der Partikelpositionen
       cv::Point3i statePt(condens->State[0], condens->State[1], condens->State[2]);
       newCameraV.push_back(statePt);
       cout << "NeuePose: X-Achse: " << condens->State[0] << "/" << lastCam(0) << " Y-Achse: " << condens->State[1] << "/" << lastCam(1)<< " Z-Achse: " << condens->State[2] << "/" << lastCam(2)<< endl;

       writeParticle.writeFile();
}

void particle::writeFile(){
    QFile file("particle.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << "Partikel nummero eins\n";

    file.close();
}

void particle::paint()
    {
     glClearColor (1, 1, 1, 0);
     glColor3f (0, 1, 0);
     glPointSize (4);

     glBegin (GL_POINTS);
     glVertex2f (0, 0);

     glEnd ();


    }


