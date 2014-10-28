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

cv::Mat renderer::genImg(){
    //Bild zum Vergleich wird erstellt
    cv::Mat img(480, 720, CV_8UC3);

    //use fast 4-byte alignment (default anyway) if possible
    glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3) ? 1 : 4);
    //set length of one complete row in destination data (doesn't need to equal img.cols)
    glPixelStorei(GL_PACK_ROW_LENGTH, img.step/img.elemSize());
    //Pixel werden ausgelesen und im img gespeichert
    glReadPixels(0, 0, 720, 480, GL_BGR, GL_UNSIGNED_BYTE, img.data);

    //muss geflippt werden, da opencv images von top to bottom speichert
    cv::flip(img, img, 0);

    //cv::namedWindow( "Meshes", cv::WINDOW_AUTOSIZE);// Create a window for display.
    //cv::imshow( "Meshes", img );       // Show our image inside it.

    return img;
}

void renderer::likelihood(int videoCount, int particleCount){
    int compare = abs(videoCount - particleCount);
    if(compare < maxLikelihood)
        maxLikelihood = compare;
}

void renderer::renderMeshes(Camera &camera,Mesh* mesh, particle camPartikel, Shader* _program, glm::mat4 m, glm::mat4 v, glm::mat4 p, GLuint mLoc,GLuint vLoc,GLuint pLoc, QGLFramebufferObject *fbo, int pixelCounter, GLuint handle){

    cv::Mat_<float> centerMat = camPartikel.getParticleCenterM();
    cv::Mat_<float> lookAtMat = camPartikel.getParticleLookAtM();
    GLuint QueryID;
    glGenQueries(1, &QueryID);
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

            //lists[i * centerMat.rows + j]=i * centerMat.rows + j;
            //glNewList(pbufferList+(i * centerMat.rows + j), GL_COMPILE);
            GLuint pbufferList;
            pbufferList = glGenLists(1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glBeginQuery(GL_SAMPLES_PASSED, QueryID);
            glNewList(pbufferList, GL_COMPILE);
            {
                camera.setLookAt(cameraLookAt);

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

            // render to the framebuffer object
            fbo->bind();
            //glListBase(pbufferList);
            //glCallLists(4, GL_INT, lists);
            glCallList(pbufferList);
            fbo->release();
            glEndQuery(GL_SAMPLES_PASSED);
            glGetQueryObjectuiv(QueryID, GL_QUERY_RESULT, &PixelCount);
            cout << PixelCount << endl;
            glDeleteQueries(1, &QueryID);
            //  ------------------------- hierdurch werden die gerenderten boxen erst sichtbar
            glBindTexture(GL_TEXTURE_2D, fbo->texture());
            //cout << i * centerMat.rows + j << endl;

            //hier müsste der sobelfilter angewendet und das bild ausgelesen werden.
            glDeleteLists(pbufferList,1);
        }
    }
}

void renderer::rendern(Mesh* mesh){

    //cout << cameraPos.x << "/" << cameraPos.y << "/" << cameraPos.z << endl;

}







