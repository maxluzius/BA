#include "renderer.h"

renderer::renderer()
{
}

int renderer::count(cv::Mat img){
    int count = 0;
    for (int x = 0; x < 720; ++x) {
        for (int y = 0; y < 480; ++y) {
            uchar color = img.at<uchar>(y, x);
            if((int)color == 255)
               count++;
        }
    }
    return count;
}

void renderer::likelihood(int videoCount, int particleCount){
    int compare = abs(videoCount - particleCount);
    if(compare < maxLikelihood)
        maxLikelihood = compare;
}

void renderer::renderMeshes(Camera &camera,Mesh* mesh, particle camPartikel, Shader* _likelihood, Shader* _meshProgram, glm::mat4 m, glm::mat4 v, glm::mat4 p, GLuint mLoc,GLuint vLoc,GLuint pLoc, QGLFramebufferObject *fbo, GLuint handle, float &relMaxValue, glm::vec3 &newCenter, glm::vec3 &newLookAt){

    max = 0.0;
    cv::Mat_<float> centerMat = camPartikel.getParticleCenterM();
    cv::Mat_<float> lookAtMat = camPartikel.getParticleLookAtM();
    //querys to count the pixels
    const int size = 25;
    GLuint queryArray[size];
    glGenQueries(size, queryArray);
    float meshsRelValue[size] = {0.0};

    //bind texture with edgepicture
    texLoc = glGetUniformLocation(handle,"tex");
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(texLoc,0);
    glBindTexture(GL_TEXTURE_2D,handle);

    //bind likelihoodshader
    _likelihood->bind();

    m = mesh->computeModelMatrix();

    glUniformMatrix4fv(mLoc,1,GL_FALSE,glm::value_ptr(m));
    glUniformMatrix4fv(pLoc,1,GL_FALSE,glm::value_ptr(p));

    //funktioniert nicht
    //glm::mat4 * view = new glm::mat4[size];
    //view = camPartikel.getViewArray();
    for (int i = 0; i < centerMat.rows; i++) {
        cameraPos.x = centerMat(i,0);
        cameraPos.y = centerMat(i,1);
        cameraPos.z = centerMat(i,2);
        camera.setCenter(cameraPos);
        for (int j = 0; j < centerMat.rows; j++) {
            cameraLookAt.x = lookAtMat(i * centerMat.rows + j, 0);
            cameraLookAt.y = lookAtMat(i * centerMat.rows + j, 1);
            cameraLookAt.z = lookAtMat(i * centerMat.rows + j, 2);
            camera.setLookAt(cameraLookAt);

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                v = camera.getViewMatrix();
                glUniformMatrix4fv(vLoc,1,GL_FALSE,glm::value_ptr(v));

                glBeginQuery(GL_SAMPLES_PASSED, queryArray[i]);
                if(mesh != nullptr)
                    mesh->draw(&cameraPos);
                glEndQuery(GL_SAMPLES_PASSED);
                glGetQueryObjectuiv(queryArray[i], GL_QUERY_RESULT, &PixelCountSet);

                meshsRelValue[i * centerMat.rows + j] = PixelCountSet;

                //cout << "PixelCount für " << i * centerMat.rows + j << ": " << PixelCountSet << endl;
                        }
                }

    glBindTexture(GL_TEXTURE_2D,0);
    _likelihood->unbind();

    //bind meshshader
    _meshProgram->bind();

    for (int i = 0; i < centerMat.rows; i++) {
        cameraPos.x = centerMat(i,0);
        cameraPos.y = centerMat(i,1);
        cameraPos.z = centerMat(i,2);
        camera.setCenter(cameraPos);
        for (int j = 0; j < centerMat.rows; j++) {
            cameraLookAt.x = lookAtMat(i * centerMat.rows + j, 0);
            cameraLookAt.y = lookAtMat(i * centerMat.rows + j, 1);
            cameraLookAt.z = lookAtMat(i * centerMat.rows + j, 2);
            camera.setLookAt(cameraLookAt);

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                v = camera.getViewMatrix();
                glUniformMatrix4fv(vLoc,1,GL_FALSE,glm::value_ptr(v));

                glBeginQuery(GL_SAMPLES_PASSED, queryArray[i]);
                if(mesh != nullptr)
                    mesh->draw(&cameraPos);
                glEndQuery(GL_SAMPLES_PASSED);
                glGetQueryObjectuiv(queryArray[i], GL_QUERY_RESULT, &PixelCountSet);

                meshsRelValue[i * centerMat.rows + j] /= PixelCountSet;

                if(meshsRelValue[i * centerMat.rows + j] > max){
                    max = meshsRelValue[i * centerMat.rows + j];
                    newCenter.x = centerMat(i,0);
                    newCenter.y = centerMat(i,1);
                    newCenter.z = centerMat(i,2);
                    newLookAt.x = lookAtMat(i * centerMat.rows + j, 0);
                    newLookAt.y = lookAtMat(i * centerMat.rows + j, 1);
                    newLookAt.z = lookAtMat(i * centerMat.rows + j, 2);
                }

                cout << "PixelCount für " << i * centerMat.rows + j << ": " << meshsRelValue[i * centerMat.rows + j] << endl;
        }
    }


    _meshProgram->unbind();
    glDeleteQueries(size, queryArray);

    //set camerapose of the best new position
    camera.setCenter(newCenter);
    camera.setLookAt(newLookAt);

//    for (int i = 0; i < centerMat.rows; i++) {
//        cameraPos.x = centerMat(i,0);
//        cameraPos.y = centerMat(i,1);
//        cameraPos.z = centerMat(i,2);
//        camera.setCenter(cameraPos);
//        for (int j = 0; j < centerMat.rows; j++) {
//            cameraLookAt.x = lookAtMat(i * centerMat.rows + j, 0);
//            cameraLookAt.y = lookAtMat(i * centerMat.rows + j, 1);
//            cameraLookAt.z = lookAtMat(i * centerMat.rows + j, 2);
//            camera.setLookAt(cameraLookAt);

//            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//            //starts counting the Pixels which are set
//            glBeginQuery(GL_SAMPLES_PASSED, QueryID[0]);

//            _likelihood->bind();
//            glActiveTexture(GL_TEXTURE0);
//            glUniform1i(texLoc,0);
//            glBindTexture(GL_TEXTURE_2D,handle);

//            m = mesh->computeModelMatrix();
//            v = camera.getViewMatrix();

//            glUniformMatrix4fv(mLoc,1,GL_FALSE,glm::value_ptr(m));
//            glUniformMatrix4fv(vLoc,1,GL_FALSE,glm::value_ptr(v));
//            glUniformMatrix4fv(pLoc,1,GL_FALSE,glm::value_ptr(p));
//            if(mesh != nullptr)
//                mesh->draw(&cameraPos);
//            glBindTexture(GL_TEXTURE_2D,0);
//            _likelihood->unbind();;

//            //end counting
//            glEndQuery(GL_SAMPLES_PASSED);
//            glGetQueryObjectuiv(QueryID[0], GL_QUERY_RESULT, &PixelCountSet);

//            //starts counting all Pixels of the Mesh
//            glBeginQuery(GL_SAMPLES_PASSED, QueryID[1]);

//            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//            _meshProgram->bind();

//            m = mesh->computeModelMatrix();
//            v = camera.getViewMatrix();

//            glUniformMatrix4fv(mLoc,1,GL_FALSE,glm::value_ptr(m));
//            glUniformMatrix4fv(vLoc,1,GL_FALSE,glm::value_ptr(v));
//            glUniformMatrix4fv(pLoc,1,GL_FALSE,glm::value_ptr(p));

//            cameraPos = camera.center();

//            if(mesh != nullptr)
//                mesh->draw(&cameraPos);

//            _meshProgram->unbind();

//            //end counting
//            glEndQuery(GL_SAMPLES_PASSED);
//            glGetQueryObjectuiv(QueryID[1], GL_QUERY_RESULT, &PixelCountAll);

//            relVal = PixelCountSet/(float)PixelCountAll;
//            if(relVal > max){
//                max = relVal;
//                if(max >= (relMaxValue - 0.1)){
//                    relMaxValue = max;
//                    newCenter.x = centerMat(i,0);
//                    newCenter.y = centerMat(i,1);
//                    newCenter.z = centerMat(i,2);
//                    newLookAt.x = lookAtMat(i * centerMat.rows + j, 0);
//                    newLookAt.y = lookAtMat(i * centerMat.rows + j, 1);
//                    newLookAt.z = lookAtMat(i * centerMat.rows + j, 2);
//                    cout << relVal << "/" << max << "/" << relMaxValue << endl;
//                }
//            }
//            //cout << relVal << "/" << max << "/" << relMaxValue << endl;
//            glDeleteQueries(2, QueryID);
//        }
//    }
//    camera.setCenter(newCenter);
//    camera.setLookAt(newLookAt);



}

void renderer::rendern(Mesh* mesh){

    //cout << cameraPos.x << "/" << cameraPos.y << "/" << cameraPos.z << endl;

}







