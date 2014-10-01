#include "renderer.h"

renderer::renderer()
{
    isDone = false;
}

void renderer::setIsDone(bool var){
    isDone = var;
}

bool renderer::getIsDone(){
    return isDone;
}

cv::Point2i renderer::setupCamera(Camera &camera,Mesh* mesh, particle camPartikel, Shader* _program, glm::mat4 m, glm::mat4 v, glm::mat4 p, GLuint mLoc,GLuint vLoc,GLuint pLoc, cv::Point2i loop){

    cv::Mat_<float> centerMat = camPartikel.getParticleCenterM();
    cv::Mat_<float> lookAtMat = camPartikel.getParticleLookAtM();


    for (int i = loop.x; i < centerMat.rows; i++) {
        cameraPos.x = centerMat(i,0);
        cameraPos.y = centerMat(i,1);
        cameraPos.z = centerMat(i,2);
        camera.setCenter(cameraPos);
        for (int j = loop.y; j < centerMat.rows; j) {
            cameraLookAt.x = lookAtMat(i * centerMat.rows + j, 0);
            cameraLookAt.y = lookAtMat(i * centerMat.rows + j, 1);
            cameraLookAt.z = lookAtMat(i * centerMat.rows + j, 2);
            cout << i * centerMat.rows + j << ": " << cameraLookAt.x << "/" << cameraLookAt.y << "/" << cameraLookAt.z << endl;
            camera.setLookAt(cameraLookAt);

            _program->bind();

            m = mesh->computeModelMatrix();
            v = camera.getViewMatrix();

            glUniformMatrix4fv(mLoc,1,GL_FALSE,glm::value_ptr(m));
            glUniformMatrix4fv(vLoc,1,GL_FALSE,glm::value_ptr(v));
            glUniformMatrix4fv(pLoc,1,GL_FALSE,glm::value_ptr(p));
            loop.x = i;
            loop.y = j +1;
            return loop;
        }
        loop.x += 1;
        if(loop.y = centerMat.rows)
            loop.y = 0;
    }
    setIsDone(true);
    return loop;
}

void renderer::rendern(Mesh* mesh){
    if(mesh != nullptr)
        mesh->draw(&cameraPos);
    //cout << cameraPos.x << "/" << cameraPos.y << "/" << cameraPos.z << endl;

}







