#include "particle.h"
#include <Windows.h>

 vector<cv::Point3f> cameraV, newCameraV;

particle::particle()
{
    dim = 3;
    nParticles = 7;
}

void particle::setMaxRange(float x, float y, float z){
    xRange = x;
    yRange = y;
    zRange = z;
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

double particle::particleTime(){
    //Variablen
    LONGLONG g_Frequency, g_CurentCount, g_LastCount;

    //Frequenz holen
    if (!QueryPerformanceFrequency((LARGE_INTEGER*)&g_Frequency))
        std::cout << "Performance Counter nicht vorhanden" << std::endl;

    //1. Messung
    QueryPerformanceCounter((LARGE_INTEGER*)&g_CurentCount);

    Sleep(10);  // Sleep ist ungenau, darum wird nicht 10ms herauskommen

    //2. Messung
    QueryPerformanceCounter((LARGE_INTEGER*)&g_LastCount);

    double dTimeDiff = (((double)(g_LastCount-g_CurentCount))/((double)g_Frequency));

    return dTimeDiff;
}

void particle::genParticlesPos(Camera& camera)
{
    //Position der Kamera
    cv::Mat_<float> lastCam(3,1);
    lastCam(0) = camera.center().x;
    lastCam(1) = camera.center().y;
    lastCam(2) = camera.center().z;
    //LookAt der Kamera
    cv::Point3f LookAtPt(camera.lookAt().x, camera.lookAt().y, camera.lookAt().z);


    //Bereich der Partikelstreuung
    setRanges(lastCam(0), lastCam(1), lastCam(2), 10.0);

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
      cv::Point3f measPt(lastCam(0),lastCam(1),lastCam(2));
      cameraV.push_back(measPt);


      for (int i = 0; i < condens->SamplesNum; i++) {

         //Berechnung der Abweichung
         float diffX = (lastCam(0) - condens->flSamples[i][0])/xRange;
         float diffY = (lastCam(1) - condens->flSamples[i][1])/yRange;
         float diffZ = (lastCam(2) - condens->flSamples[i][2])/zRange;
         condens->flConfidence[i] = 1.0 / (sqrt(diffX * diffX + diffY * diffY + diffZ * diffZ));

         // Partikelstreuung werde ich benötigen
         cv::Point3f partPt(condens->flSamples[i][0], condens->flSamples[i][1], condens->flSamples[i][2]);
         particle genParticle;
         genParticle.genParticlesLookAt(partPt, camera);
         cout << "PartikelPos: X-Achse: " << condens->flSamples[i][0] << "/" << lastCam(0) << " Y-Achse: " << condens->flSamples[i][1] << "/" << lastCam(1)<< " Z-Achse: " << condens->flSamples[i][2] << "/" << lastCam(2)<< endl;
         //writeFile(condens->flSamples[i][0], condens->flSamples[i][1], condens->flSamples[i][2], "particlePos.txt");

       }

       cvConDensUpdateByTime(condens);

       //Bester Partikel, ist aber keine der Partikelpositionen
       cv::Point3f statePt(condens->State[0], condens->State[1], condens->State[2]);
       newCameraV.push_back(statePt);
       cout << "NeuePose: X-Achse: " << condens->State[0] << "/" << lastCam(0) << " Y-Achse: " << condens->State[1] << "/" << lastCam(1)<< " Z-Achse: " << condens->State[2] << "/" << lastCam(2)<< endl;
       glm::vec3 newCenter;
       newCenter.x = condens->State[0];
       newCenter.y = condens->State[1];
       newCenter.z = condens->State[2];
       camera.setCenter(newCenter);
}

void particle::genParticlesLookAt(cv::Point3f position, Camera &camera){
    //Position der Kamera
    cv::Mat_<float> lastCam(3,1);
    lastCam(0) = position.x;
    lastCam(1) = position.y;
    lastCam(2) = position.z;

    double time = particleTime();
    float r = 10.0 + time;
    //Bereich der Partikelstreuung
    setRanges(lastCam(0), lastCam(1), lastCam(2), r);

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
      cv::Point3f measPt(lastCam(0),lastCam(1),lastCam(2));
      cameraV.push_back(measPt);

      for (int i = 0; i < condens->SamplesNum; i++) {

         //Berechnung der Abweichung
         float diffX = (lastCam(0) - condens->flSamples[i][0])/xRange;
         float diffY = (lastCam(1) - condens->flSamples[i][1])/yRange;
         float diffZ = (lastCam(2) - condens->flSamples[i][2])/zRange;
         condens->flConfidence[i] = 1.0 / (sqrt(diffX * diffX + diffY * diffY + diffZ * diffZ));

         // Partikelstreuung werde ich benötigen
        //cout << condens->flSamples[i][0] << "/" << condens->flSamples[i][0] - position.x << "/" << position.x << endl;
         cv::Point3f lookAtPt( condens->flSamples[i][0] - position.x, condens->flSamples[i][1] - position.y, condens->flSamples[i][2] - position.z);
         cout << "PartikelLookAt: X-Achse: " << condens->flSamples[i][0] << "/" << lookAtPt.x <<
                                " Y-Achse: " << condens->flSamples[i][1] << "/" << lookAtPt.y <<
                                " Z-Achse: " << condens->flSamples[i][2] << "/" << lookAtPt.z << endl;
         writeFile(position, lookAtPt, "particle.txt");

       }

       cvConDensUpdateByTime(condens);

       //Bester Partikel, ist aber keine der Partikelpositionen
       cv::Point3f statePt(condens->State[0], condens->State[1], condens->State[2]);
       newCameraV.push_back(statePt);
       cout << "NeueLookAt: X-Achse: " << condens->State[0] << "/" << lastCam(0) << " Y-Achse: " << condens->State[1] << "/" << lastCam(1)<< " Z-Achse: " << condens->State[2] << "/" << lastCam(2)<< endl;
//       glm::vec3 newLookAt;
//       newLookAt.x = condens->State[0];
//       newLookAt.y = condens->State[1];
//       newLookAt.z = condens->State[2];
//       camera.setLookAt(newLookAt);
}

void particle::writeFile(cv::Point3f position, cv::Point3f lookAt, QString name){
    QFile file(name);
    //if(file.exists()){
    file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QTextStream out(&file);
    //gezeichnet wird der vector von position zu lookAt + position
    //Ausgabe ist x/z/y da die Y und Z Achse in gnuplot vertauscht ist
    out << position.x << "   " << position.z << "   " << position.y << "   " << lookAt.x << "   " << lookAt.z << "   " << lookAt.y << "\n";

    file.close();
    //}
}

void particle::setNewPose(cv::Point3f newPose){

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


