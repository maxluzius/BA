#include "particle.h"
#include "glm/gtc/matrix_transform.hpp"
#include <Windows.h>

 vector<cv::Point3f> cameraV, newCameraV;



particle::particle()
{
    dim = 3;
    nParticles = 15;
}

void particle::setMaxRange(float x, float y, float z){
    xRange = x;  // Vor/zurück "Z-Achse" der Kamera
    yRange = y;  // Höhe "Up-Vektor"
    zRange = z;  // Links/Rechts
    maxRange[0] = x;
    maxRange[1] = y;
    maxRange[2] = z;
}

void particle::setMinRange(float x, float y, float z){
    minRange[0] = x;
    minRange[1] = y;
    minRange[2] = z;
}

float particle::getMaxRangeX(){
    return xRange;
}

float particle::getMaxRangeY(){
    return yRange;
}

float particle::getMaxRangeZ(){
    return zRange;
}

float *particle::getMaxRange(){
    return maxRange;
}

float *particle::getMinRange(){
    return minRange;
}

void particle::setRanges(float x, float y, float z, float r){
    float maxX = x + r;
    float maxY = y + r;
    float maxZ = z + r;
    float minX = x - r;
    float minY = y - r;
    float minZ = z - r;
    setMaxRange(maxX, maxY, maxZ);
    setMinRange(minX, minY, minZ);
}

cv::Mat_<float> particle::getParticleCenterM(){
    return particleCenterM;
}

cv::Mat_<float> particle::getParticleLookAtM(){
    return particleLookAtM;
}

void particle::initParticle(Camera& camera){

    particleCenterM = cv::Mat::zeros(nParticles,dim, CV_32F);
    particleLookAtM = cv::Mat::zeros(nParticles * nParticles, dim, CV_32F);

    centerCamera.x = camera.center().x;
    centerCamera.y = camera.center().y;
    centerCamera.z = camera.center().z;
    lookAtCamera.x = camera.lookAt().x;
    lookAtCamera.y = camera.lookAt().y;
    lookAtCamera.z = camera.lookAt().z;
    genParticles(centerCamera);

}


void particle::genParticles(glm::vec3 particleV)
{

    particleCenterM.setTo(cv::Scalar(0));
    //Bereich der Partikelstreuung
    setRanges(particleV.x, particleV.y, particleV.z, 0.5);

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

      for (int i = 0; i < condens->SamplesNum; i++) {

         //Berechnung der Abweichung
//         float diffX = (particleV.x - condens->flSamples[i][0])/xRange;
//         float diffY = (particleV.y - condens->flSamples[i][1])/yRange;
//         float diffZ = (particleV.z - condens->flSamples[i][2])/zRange;
//         condens->flConfidence[i] = 1.0 / (sqrt(diffX * diffX + diffY * diffY + diffZ * diffZ));

         // Partikelstreuung werde ich benötigen
         //cv::Point3f partPt(condens->flSamples[i][0], condens->flSamples[i][1], condens->flSamples[i][2]);
         glm::vec3 partCenter(condens->flSamples[i][0], condens->flSamples[i][1], condens->flSamples[i][2]);

         particleCenterM(i,0) = partCenter.x;
         particleCenterM(i,1) = partCenter.y;
         particleCenterM(i,2) = partCenter.z;
         genParticles(lookAtCamera, partCenter, i);
         //cout << "PartikelPos: X-Achse: " << condens->flSamples[i][0] << "/" << lastCam(0) << " Y-Achse: " << condens->flSamples[i][1] << "/" << lastCam(1)<< " Z-Achse: " << condens->flSamples[i][2] << "/" << lastCam(2)<< endl;
         //writeFile(condens->flSamples[i][0], condens->flSamples[i][1], condens->flSamples[i][2], "particlePos.txt");

       }

       //cvConDensUpdateByTime(condens);

       //Bester Partikel, ist aber keine der Partikelpositionen
       //cv::Point3f statePt(condens->State[0], condens->State[1], condens->State[2]);
       //newCameraV.push_back(statePt);
       //cout << "NeuePose: X-Achse: " << condens->State[0] << "/" << lastCam(0) << " Y-Achse: " << condens->State[1] << "/" << lastCam(1)<< " Z-Achse: " << condens->State[2] << "/" << lastCam(2)<< endl;
}

void particle::genParticles(glm::vec3 particleV, glm::vec3 partCenter, int j)
{
    //Bereich der Partikelstreuung
    setRanges(particleV.x, particleV.y, particleV.z, 0.5);

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

      for (int i = 0; i < condens->SamplesNum; i++) {

         //Berechnung der Abweichung
//         float diffX = (particleV.x - condens->flSamples[i][0])/xRange;
//         float diffY = (particleV.y - condens->flSamples[i][1])/yRange;
//         float diffZ = (particleV.z - condens->flSamples[i][2])/zRange;
//         condens->flConfidence[i] = 1.0 / (sqrt(diffX * diffX + diffY * diffY + diffZ * diffZ));

         // Partikelstreuung werde ich benötigen
         //cv::Point3f lookAtPt(condens->flSamples[i][0], condens->flSamples[i][1], condens->flSamples[i][2]);
         glm::vec3 partLookAt(condens->flSamples[i][0], condens->flSamples[i][1], condens->flSamples[i][2]);

         particleLookAtM(i + j * nParticles,0) = partLookAt.x;
         particleLookAtM(i + j * nParticles,1) = partLookAt.y;
         particleLookAtM(i + j * nParticles,2) = partLookAt.z;

         //writeFile(partCenter, partLookAt, "particle.txt");
         //generateViews(partCenter, partLookAt, i + j * nParticles);

       }

       //cvConDensUpdateByTime(condens);

       //Bester Partikel, ist aber keine der Partikelpositionen
       //cv::Point3f statePt(condens->State[0], condens->State[1], condens->State[2]);
       //newCameraV.push_back(statePt);
       //cout << "NeuePose: X-Achse: " << condens->State[0] << "/" << lastCam(0) << " Y-Achse: " << condens->State[1] << "/" << lastCam(1)<< " Z-Achse: " << condens->State[2] << "/" << lastCam(2)<< endl;
}


void particle::writeFile(glm::vec3 partCenter, glm::vec3 partLookAt, QString name){
    QFile file(name);
    //if(file.exists()){
    file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QTextStream out(&file);
    //gezeichnet wird der vector von position zu lookAt + position
    //Ausgabe ist x/z/y da die Y und Z Achse in gnuplot vertauscht ist
    out << partCenter.x << "   " << partCenter.z << "   " << partCenter.y << "   " << partLookAt.x - partCenter.x << "   " << partLookAt.z - partCenter.z << "   " << partLookAt.y - partCenter.y << "\n";

    file.close();
    //}
}

void particle::print(float x, float y, float z){
        cout << x << "/" << y << "/" << z << endl;

}

void particle::generateViews(glm::vec3 center, glm::vec3 lookAt, int i){
    viewArray = new glm::mat4[25];
    glm::vec3 up = glm::vec3(0,1,0);

    newViewMatrix = glm::lookAt(center, lookAt, up);
    viewArray[i] = newViewMatrix;
}

glm::mat4 * particle::getViewArray(){
    return viewArray;
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


