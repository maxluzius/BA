#include "renderer.h"

renderer::renderer()
{
}

int renderer::count(cv::Mat img){
    int count = 0;
    for (int x = 0; x < 1280; ++x) {
        for (int y = 0; y < 720; ++y) {
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

void renderer::renderMeshes(Camera &camera,Mesh* mesh, particle camPartikel, Shader* _program, Shader* _meshProgram, glm::mat4 m, glm::mat4 v, glm::mat4 p, GLuint mLoc,GLuint vLoc,GLuint pLoc, QGLFramebufferObject *fbo, GLuint handle){

    max = 0.0;
    numberPos = 0;
    numberLookAt = 0;
    relVal = 0.0;
    cv::Mat_<float> centerMat = camPartikel.getParticleCenterM();
    cv::Mat_<float> lookAtMat = camPartikel.getParticleLookAtM();
    GLuint QueryID[2];
    glGenQueries(2, QueryID);

    texLoc = glGetUniformLocation(handle,"tex");

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

            GLuint pbufferList[2];
            pbufferList[0] = glGenLists(2);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glNewList(pbufferList[0], GL_COMPILE);
            {


                _program->bind();
                glActiveTexture(GL_TEXTURE0);
                glUniform1i(texLoc,0);
                glBindTexture(GL_TEXTURE_2D,handle);

                m = mesh->computeModelMatrix();
                v = camera.getViewMatrix();

                glUniformMatrix4fv(mLoc,1,GL_FALSE,glm::value_ptr(m));
                glUniformMatrix4fv(vLoc,1,GL_FALSE,glm::value_ptr(v));
                glUniformMatrix4fv(pLoc,1,GL_FALSE,glm::value_ptr(p));
                if(mesh != nullptr)
                    mesh->draw(&cameraPos);
                glBindTexture(GL_TEXTURE_2D,0);
                _program->unbind();

                glPushMatrix();
            }
            glEndList();


            glNewList(pbufferList[1], GL_COMPILE);
            {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                _meshProgram->bind();

                m = mesh->computeModelMatrix();
                v = camera.getViewMatrix();

                glUniformMatrix4fv(mLoc,1,GL_FALSE,glm::value_ptr(m));
                glUniformMatrix4fv(vLoc,1,GL_FALSE,glm::value_ptr(v));
                glUniformMatrix4fv(pLoc,1,GL_FALSE,glm::value_ptr(p));

                glm::vec3 cameraPos = camera.center();

                if(mesh != nullptr)
                    mesh->draw(&cameraPos);

                _meshProgram->unbind();

                glPushMatrix();
            }
            glEndList();



            // render to the framebuffer object
            fbo->bind();
            //starts counting the Pixels which are set
            glBeginQuery(GL_SAMPLES_PASSED, QueryID[0]);

            glCallList(pbufferList[0]);

            //end counting
            glEndQuery(GL_SAMPLES_PASSED);
            glGetQueryObjectuiv(QueryID[0], GL_QUERY_RESULT, &PixelCountSet);

            //starts counting all Pixels of the Mesh
            glBeginQuery(GL_SAMPLES_PASSED, QueryID[1]);

            glCallList(pbufferList[1]);

            //end counting
            glEndQuery(GL_SAMPLES_PASSED);
            glGetQueryObjectuiv(QueryID[1], GL_QUERY_RESULT, &PixelCountAll);

            fbo->release();

            relVal = PixelCountSet/(float)PixelCountAll;
            if(relVal > max){
                max = relVal;
                numberPos = i;
                numberLookAt = i * centerMat.rows + j;
            }
            cout << PixelCountSet << "/" << PixelCountAll << "/" << relVal << endl;
            glDeleteQueries(2, QueryID);

            glDeleteLists(pbufferList[0],2);
        }
    }
    if(max >= bestRelVal){
    cameraPos.x = centerMat(numberPos,0);
    cameraPos.y = centerMat(numberPos,1);
    cameraPos.z = centerMat(numberPos,2);
    camera.setCenter(cameraPos);

    cameraLookAt.x = lookAtMat(numberLookAt, 0);
    cameraLookAt.y = lookAtMat(numberLookAt, 1);
    cameraLookAt.z = lookAtMat(numberLookAt, 2);
    camera.setLookAt(cameraLookAt);
    }
}

void renderer::rendern(Mesh* mesh){

    //cout << cameraPos.x << "/" << cameraPos.y << "/" << cameraPos.z << endl;

}







