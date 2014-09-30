#include "particle.h"
#include <Windows.h>

 vector<cv::Point3f> cameraV, newCameraV;

particle::particle()
{
    dim = 3;
    nParticles = 7;
}

void particle::setCenter(Camera& camera){
    centerCameraV.x = camera.center().x;
    centerCameraV.y = camera.center().y;
    centerCameraV.z = camera.center().z;
}

void particle::setLookAt(Camera &camera){
    lookAtCameraV.x = camera.lookAt().x;
    lookAtCameraV.y = camera.lookAt().y;
    lookAtCameraV.z = camera.lookAt().z;
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

float particle::getMinRange(){
    return 30.0;
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

void particle::initParticle(Camera& camera){
    setCenter(camera);
    setLookAt(camera);
    genParticles(centerCameraV);
}

glm::vec3 particle::getParticleCenter(){
    return particleCenterV;
}

glm::vec3 particle::getParticleLookAt(){
    return particleLookAtV;
}

void particle::setParticleCenter(cv::Point3f point){
    particleCenterV.x = point.x;
    particleCenterV.y = point.y;
    particleCenterV.z = point.z;
}

void particle::setParticleLookAt(cv::Point3f point){
    particleLookAtV.x = point.x;
    particleLookAtV.y = point.y;
    particleLookAtV.z = point.z;
}

void particle::genParticles(glm::vec3 particleV)
{
    //Bereich der Partikelstreuung
    setRanges(particleV.x, particleV.y, particleV.z, 10.0);

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
         float diffX = (particleV.x - condens->flSamples[i][0])/xRange;
         float diffY = (particleV.y - condens->flSamples[i][1])/yRange;
         float diffZ = (particleV.z - condens->flSamples[i][2])/zRange;
         condens->flConfidence[i] = 1.0 / (sqrt(diffX * diffX + diffY * diffY + diffZ * diffZ));

         // Partikelstreuung werde ich benötigen
         cv::Point3f partPt(condens->flSamples[i][0], condens->flSamples[i][1], condens->flSamples[i][2]);

         setParticleCenter(partPt);
         genParticles(lookAtCameraV, partPt);
         //cout << "PartikelPos: X-Achse: " << condens->flSamples[i][0] << "/" << lastCam(0) << " Y-Achse: " << condens->flSamples[i][1] << "/" << lastCam(1)<< " Z-Achse: " << condens->flSamples[i][2] << "/" << lastCam(2)<< endl;
         //writeFile(condens->flSamples[i][0], condens->flSamples[i][1], condens->flSamples[i][2], "particlePos.txt");

       }

       cvConDensUpdateByTime(condens);

       //Bester Partikel, ist aber keine der Partikelpositionen
       cv::Point3f statePt(condens->State[0], condens->State[1], condens->State[2]);
       newCameraV.push_back(statePt);
       //cout << "NeuePose: X-Achse: " << condens->State[0] << "/" << lastCam(0) << " Y-Achse: " << condens->State[1] << "/" << lastCam(1)<< " Z-Achse: " << condens->State[2] << "/" << lastCam(2)<< endl;
}

void particle::genParticles(glm::vec3 particleV, cv::Point3f partPt)
{
    //Bereich der Partikelstreuung
    setRanges(particleV.x, particleV.y, particleV.z, 5.0);

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
         float diffX = (particleV.x - condens->flSamples[i][0])/xRange;
         float diffY = (particleV.y - condens->flSamples[i][1])/yRange;
         float diffZ = (particleV.z - condens->flSamples[i][2])/zRange;
         condens->flConfidence[i] = 1.0 / (sqrt(diffX * diffX + diffY * diffY + diffZ * diffZ));

         // Partikelstreuung werde ich benötigen
         cv::Point3f lookAtPt(condens->flSamples[i][0], condens->flSamples[i][1], condens->flSamples[i][2]);
         setParticleLookAt(lookAtPt);
         writeFile(partPt, lookAtPt, "particle.txt");
         //cout << "PartikelPos: X-Achse: " << condens->flSamples[i][0] << "/" << lastCam(0) << " Y-Achse: " << condens->flSamples[i][1] << "/" << lastCam(1)<< " Z-Achse: " << condens->flSamples[i][2] << "/" << lastCam(2)<< endl;
         //writeFile(condens->flSamples[i][0], condens->flSamples[i][1], condens->flSamples[i][2], "particlePos.txt");

       }

       cvConDensUpdateByTime(condens);

       //Bester Partikel, ist aber keine der Partikelpositionen
       cv::Point3f statePt(condens->State[0], condens->State[1], condens->State[2]);
       newCameraV.push_back(statePt);
       //cout << "NeuePose: X-Achse: " << condens->State[0] << "/" << lastCam(0) << " Y-Achse: " << condens->State[1] << "/" << lastCam(1)<< " Z-Achse: " << condens->State[2] << "/" << lastCam(2)<< endl;
}


void particle::writeFile(cv::Point3f position, cv::Point3f lookAt, QString name){
    QFile file(name);
    //if(file.exists()){
    file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QTextStream out(&file);
    //gezeichnet wird der vector von position zu lookAt + position
    //Ausgabe ist x/z/y da die Y und Z Achse in gnuplot vertauscht ist
    out << position.x << "   " << position.z << "   " << position.y << "   " << lookAt.x - position.x << "   " << lookAt.z - position.z << "   " << lookAt.y - position.y << "\n";

    file.close();
    //}
}

void particle::print(float x, float y, float z){
        cout << x << "/" << y << "/" << z << endl;

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


