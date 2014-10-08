#include <iostream>
#include "openglwidget.h"
#include <QCoreApplication>
#include <QMainWindow>
#include "QTimer"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cube.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <regex>


#include "mainwindow.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) :
    QGLWidget(parent),
    _isPerspective(true),
    _nearPlane(0.1f),
    _farPlane(50.0f),
    _fov(60.0f)
{
    _aspect = width() / height();

    QGLFormat format;
    setFormat(format);

    isPlaying = false;
    img = nullptr;
    videoMats = nullptr;
    mesh = nullptr;

    parentWindow = (MainWindow*)parent;

    setMouseTracking(true); // Enable mouse continuous tracking

    timer = nullptr;

}

OpenGLWidget::~OpenGLWidget()
{
    delete _program;
    delete _imageProgram;
}

void OpenGLWidget::setCameraPose(float centerX, float centerY, float centerZ,
                                 float lookAtX, float lookAtY, float lookAtZ,
                                 float upX,     float upY,     float upZ)
{
    camera.setCenter(glm::vec3(centerX,centerY,centerZ));
    camera.setLookAt(glm::vec3(lookAtX,lookAtY,lookAtZ));
    camera.setUp(glm::vec3(upX,upY,upZ));

//    float transX = mesh->getTranslate().x;
//    float transY = mesh->getTranslate().y;
//    float transZ = mesh->getTranslate().z;
//    camPartikel.print(transX, transY, transZ);
    camPartikel.initParticle(camera);  //Partikelerzeugung
}

void OpenGLWidget::setProjection(float fov, float nearPlane, float farPlane, bool isPerspective)
{
    _fov = fov;
    camera.setFov(fov);
    _nearPlane = nearPlane;
    _farPlane = farPlane;
    _isPerspective = isPerspective;

    perspectiveProjection();
}

void OpenGLWidget::getCameraPose(float &centerX, float &centerY, float &centerZ, float &lookAtX, float &lookAtY, float &lookAtZ, float &upX, float &upY, float &upZ)
{
    centerX = camera.center().x;
    centerY = camera.center().y;
    centerZ = camera.center().z;

    lookAtX = camera.lookAt().x;
    lookAtY = camera.lookAt().y;
    lookAtZ = camera.lookAt().z;

    upX = camera.up().x;
    upY = camera.up().y;
    upZ = camera.up().z;
}

void OpenGLWidget::getProjection(float &fov, float &nearPlane, float &farPlane, bool &isPerspective)
{
    fov = camera.fov();
    nearPlane = _nearPlane;
    farPlane = _farPlane;
    isPerspective = _isPerspective;
}

void OpenGLWidget::getTransformation(float &transX, float &transY, float &transZ,
                                     float &pitch,  float &yaw,    float &roll,
                                     float &scaleX, float &scaleY, float &scaleZ)
{
    transX = mesh->getTranslate().x;
    transY = mesh->getTranslate().y;
    transZ = mesh->getTranslate().z;

    pitch = mesh->getPitch();
    yaw = mesh->getYaw();
    roll = mesh->getRoll();

    scaleX = mesh->getScale().x;
    scaleY = mesh->getScale().y;
    scaleZ = mesh->getScale().z;

}

void OpenGLWidget::setObjectPath(std::string filename)
{
    objectPath = filename;
}

std::string& OpenGLWidget::getObjectPath()
{
    return objectPath;
}

void OpenGLWidget::initScene()
{
    if(videoMats != nullptr)
    {
        delete[] videoMats;
    }
    else
    {
        if(img != nullptr)
        {
            delete img;
        }
    }
    videoMats = nullptr;
    img = new cv::Mat;

    _frame = 0;
    isPlaying = false;

    if(getSceneType() == TYPE_IMAGE)
    {
         *img = cv::imread(filename);
    }
    else if(getSceneType() == TYPE_VIDEO)
    {
         video = cv::VideoCapture(filename);
         if (!video.isOpened())
         {
              QMessageBox::warning(this,"Error","Could not open input video");
         }

         _fpsVideo = video.get(cv::CAP_PROP_FPS);
         _frameCountVideo = video.get(cv::CAP_PROP_FRAME_COUNT);

         // Load video in cpu memory. The first frame seems to be black so load it and overwrite it afterwards.
         int numOfMats = _frameCountVideo-1;
         videoMats = new cv::Mat[numOfMats];
        // cv::namedWindow("A");
         video.read(videoMats[0]);
         QDateTime t = QDateTime::currentDateTime();
         for(int i=0; i<numOfMats; i++)
         {
            if(!video.read(videoMats[i]))
                break;
         }
    }
    initGLTexture();
    nextFrame();
    updateTex();

    // init obj
    mesh->init();

    // resize parent
    w = img->cols;
    h = img->rows;

    mesh->setVPHeight(h);
    mesh->setVPWidth(w);
    mesh->setV(&camera.getViewMatrix());
    mesh->setP(&p);

    float aspectRatio = (float)w / (float)h;
    setFixedSize(512*aspectRatio,512);
    ((QMainWindow*)parent())->adjustSize();

    if(timer)
    {
        delete timer;
    }

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()), this, SLOT(updateGL()));

    timer->start(1.0f / _fpsVideo * 1000.f);

    camera.init();
}

void OpenGLWidget::incFrame()
{
    _frame++;

    if(_frame < 0)
        _frame = 0;
    if(_frame >= _frameCountVideo-1)
        _frame = _frameCountVideo;

    nextFrame();
    updateTex();
}

void OpenGLWidget::decFrame()
{
    _frame--;

    if(_frame < 0)
        _frame = 0;
    if(_frame >= _frameCountVideo-1)
        _frame = _frameCountVideo;

    nextFrame();
    updateTex();
}

void OpenGLWidget::setObjectType(std::string objectType)
{
    this->objectType = objectType;
}

std::string& OpenGLWidget::getObjectType()
{
    return objectType;
}

void OpenGLWidget::setFilename(std::string filename)
{
    this->filename = filename;
}

std::string& OpenGLWidget::getFilename()
{
    return filename;
}

int OpenGLWidget::getSceneType()
{
    return sceneType;
}

void OpenGLWidget::setSceneType(int type)
{
    sceneType = type;
}

void OpenGLWidget::perspectiveProjection()
{
    p = glm::perspective(camera.fov(), _aspect, _nearPlane, _farPlane);
}

bool OpenGLWidget::getIsPlaying() const
{
    return isPlaying;
}

void OpenGLWidget::setIsPlaying(bool value)
{
    isPlaying = value;
}

void OpenGLWidget::paintGL()
{
    draw();
}

void OpenGLWidget::resizeGL(int w, int h)
{
    _viewportWidth = w;//*devicePixelRatio();
    _viewportHeight = h;//*devicePixelRatio();
    glViewport(0,0,_viewportWidth, _viewportHeight);
    _aspect = (float)w / (float)(h);

    perspectiveProjection();
}

void OpenGLWidget::initGLTexture()
{
    if(screenFillingTri.texID > 0){
        glDeleteTextures(1,&screenFillingTri.texID);
        screenFillingTri.texID = 0;
    }

    glGenTextures(1,&screenFillingTri.texID);
    glBindTexture(GL_TEXTURE_2D,screenFillingTri.texID);

    // No filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Set texture clamping method
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void OpenGLWidget::updateTex()
{
    qframe = QImage((const unsigned char*)(img->ptr()), img->cols, img->rows, img->step, QImage::Format_RGB888).rgbSwapped();
    qframe = QGLWidget::convertToGLFormat(qframe);

    glBindTexture(GL_TEXTURE_2D,screenFillingTri.texID);

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, qframe.width(), qframe.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qframe.bits() );

    glBindTexture(GL_TEXTURE_2D,0);
}

void OpenGLWidget::nextFrame()
{
    if(sceneType == TYPE_VIDEO)
        *img = videoMats[_frame];
}

Camera* OpenGLWidget::getCamera()
{
    return &camera;
}

void OpenGLWidget::setCamera(const Camera &value)
{
    camera = value;
}

Mesh *OpenGLWidget::getMesh()
{
    return mesh;
}

void OpenGLWidget::setMesh(Mesh *value)
{
    if(mesh != nullptr)
        delete mesh;
    mesh = value;
}

void OpenGLWidget::setFrame(int frame)
{
    _frame = frame;
    if(_frame < 0)
        _frame = 0;
    if(_frame >= _frameCountVideo-1)
        _frame = _frameCountVideo;

    nextFrame();
    updateTex();
}


void OpenGLWidget::initializeGL()
{
    makeCurrent();
    fbo = new QGLFramebufferObject(_viewportWidth, _viewportHeight);

    glewExperimental = true;

    GLenum err = glewInit();

    if(GLEW_OK != err)
    {
        throw std::exception("Failed to initialize GLEW!");
    }

    glClearColor(0,0,0,1);
    glEnable(GL_DEPTH_TEST);
    _program = new Shader("../Shaders/vertexshader.glsl","../Shaders/fragmentshader.glsl");
    _imageProgram = new Shader("../Shaders/screenFillingTriVert.glsl","../Shaders/showImageFrag.glsl");

    screenFillingTri.texLoc = glGetUniformLocation(_imageProgram->id(),"tex");
    mLoc = glGetUniformLocation(_program->id(),"M");
    vLoc = glGetUniformLocation(_program->id(),"V");
    pLoc = glGetUniformLocation(_program->id(),"P");

    screenFillingTri.vertices[0] = -1;
    screenFillingTri.vertices[1] = -1;

    screenFillingTri.vertices[2] =  3;
    screenFillingTri.vertices[3] = -1;

    screenFillingTri.vertices[4] = -1;
    screenFillingTri.vertices[5] =  3;

    glGenVertexArrays(1,&screenFillingTri.vao);
    glBindVertexArray(screenFillingTri.vao);


    GLuint buffer;
    glGenBuffers(1,&buffer);
    glBindBuffer(GL_ARRAY_BUFFER,buffer);
    glBufferData(GL_ARRAY_BUFFER,6*sizeof(float),
                 screenFillingTri.vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);


}

void OpenGLWidget::draw()
{
    updateScene();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /////////////////////
    // Draw Background //
    /////////////////////
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        _imageProgram->bind();

        if(getSceneType() == TYPE_VIDEO && isPlaying){
            _frame++;
        }

        glBindVertexArray(screenFillingTri.vao);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(screenFillingTri.texLoc,0);
        glBindTexture(GL_TEXTURE_2D,screenFillingTri.texID);

        glDrawArrays(GL_TRIANGLES,0,3);

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D,0);

        _imageProgram->unbind();
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);


    ///////////////////////////
    // Draw Geometry Particle//
    ///////////////////////////

        //lists[1]=1;
        pbufferList = glGenLists(10);
        loop = renderObj.setupCamera(camera, mesh, camPartikel,_program, _imageProgram, m, v, p, mLoc, vLoc, pLoc, loop, _viewportWidth,_viewportHeight, screenFillingTri.texID, pbufferList, fbo);


//        // render to the framebuffer object
//        fbo->bind();
//        glListBase(pbufferList);
//        glCallLists(2, GL_INT, lists);
//        fbo->release();
//        //  ------------------------- hierdurch werden die gerenderten boxen erst sichtbar
//        glBindTexture(GL_TEXTURE_2D, fbo->texture());


        //cout << loop.x << "/" << loop.y << endl;


//        _program->bind();

//        m = mesh->computeModelMatrix();
//        v = camera.getViewMatrix();
//        glm::vec3 cameraPos = camera.center();
//        glUniformMatrix4fv(mLoc,1,GL_FALSE,glm::value_ptr(m));
//        glUniformMatrix4fv(vLoc,1,GL_FALSE,glm::value_ptr(v));
//        glUniformMatrix4fv(pLoc,1,GL_FALSE,glm::value_ptr(p));

//        if(mesh != nullptr)
//            mesh->draw(&cameraPos);


//        _program->unbind();

    if(getSceneType() == TYPE_VIDEO && isPlaying){
        if(_frame >= _frameCountVideo-1)
        {
            _frame = 0;
            video.set(cv::CAP_PROP_POS_FRAMES,_frame);
        }


       nextFrame();
       updateTex();
    }



    //camPartikel.genParticles(camera);
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    camera.mouseEventOccured(event);
}

void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    camera.keyEventOccured(event);
}

void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
    camera.wheelEventOccured(event);
}

void OpenGLWidget::setCameraEdit(bool editable)
{
    camera.setIsEditable(editable);
}

void OpenGLWidget::updateScene()
{
   perspectiveProjection();
}

