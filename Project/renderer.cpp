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


void renderer::pixelbuffer(float w,float h){
    GLuint pbo;
    glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, pbo);
    glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
    glReadBuffer(GL_DEPTH_ATTACHMENT_EXT);


    cv::Mat img(h,w, CV_8UC3);

    //use fast 4-byte alignment (default anyway) if possible
    glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3) ? 1 : 4);

    //set length of one complete row in destination data (doesn't need to equal img.cols)
    glPixelStorei(GL_PACK_ROW_LENGTH, img.step/img.elemSize());

    glReadPixels(0, 0, img.cols, img.rows, GL_BGR, GL_UNSIGNED_BYTE, img.data);

    cv::namedWindow("Rendered Boxes");
    cv::imshow("Rendered Boxes",img);

    glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, 0);
}

cv::Point2i renderer::setupCamera(Camera &camera,Mesh* mesh, particle camPartikel, Shader* _program, Shader* _imageProgram, glm::mat4 m, glm::mat4 v, glm::mat4 p, GLuint mLoc,GLuint vLoc,GLuint pLoc, cv::Point2i loop,float w, float h, GLuint texID, GLuint pabufferList, QGLFramebufferObject *fbo){

    cv::Mat_<float> centerMat = camPartikel.getParticleCenterM();
    cv::Mat_<float> lookAtMat = camPartikel.getParticleLookAtM();
    //GLuint lists[100];

    for (int i = loop.x; i < centerMat.rows; i++) {
        cameraPos.x = centerMat(i,0);
        cameraPos.y = centerMat(i,1);
        cameraPos.z = centerMat(i,2);
        camera.setCenter(cameraPos);
        for (int j = loop.y; j < centerMat.rows; j++) {
            cameraLookAt.x = lookAtMat(i * centerMat.rows + j, 0);
            cameraLookAt.y = lookAtMat(i * centerMat.rows + j, 1);
            cameraLookAt.z = lookAtMat(i * centerMat.rows + j, 2);

            //lists[i * centerMat.rows + j]=i * centerMat.rows + j;
            //glNewList(pbufferList+(i * centerMat.rows + j), GL_COMPILE);
            GLuint pbufferList;
            pbufferList = glGenLists(1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glNewList(pbufferList, GL_COMPILE);
            {
                camera.setLookAt(cameraLookAt);

                _program->bind();

                m = mesh->computeModelMatrix();
                v = camera.getViewMatrix();

                glUniformMatrix4fv(mLoc,1,GL_FALSE,glm::value_ptr(m));
                glUniformMatrix4fv(vLoc,1,GL_FALSE,glm::value_ptr(v));
                glUniformMatrix4fv(pLoc,1,GL_FALSE,glm::value_ptr(p));
                if(mesh != nullptr)
                    mesh->draw(&cameraPos);
                _program->unbind();
                glPushMatrix();
            }
            glEndList();

            // render to the framebuffer object
            fbo->bind();
            //glListBase(pbufferList);
            //glCallLists(4, GL_INT, lists);
            glCallList(pbufferList);
            fbo->release();

            //  ------------------------- hierdurch werden die gerenderten boxen erst sichtbar
            glBindTexture(GL_TEXTURE_2D, fbo->texture());

            glDeleteLists(pbufferList,1);
        }
    }
    setIsDone(true);
    return loop;
}

void renderer::rendern(Mesh* mesh){

    //cout << cameraPos.x << "/" << cameraPos.y << "/" << cameraPos.z << endl;

}







