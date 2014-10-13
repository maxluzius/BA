#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QMessageBox"
#include <iostream>
#include "QTextStream"
#include "objectloader.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    editObjectWidget = nullptr;
    editCameraWidget = nullptr;

    ui->setupUi(this);
    isSceneLoaded = false;

    ui->actionEditObject->setEnabled(isSceneLoaded);
    ui->actionEditCamera->setEnabled(isSceneLoaded);

    this->setWindowTitle(tr("Qt Project"));
}

MainWindow::~MainWindow()
{
    delete ui;
}



bool MainWindow::readBackgroundFile(QXmlStreamReader& reader)
{
    QString type = reader.attributes().value("type").toString();
    QString path;

    readNext(reader);

    if(reader.name() == "path")
    {
        path = reader.readElementText();
        if(type == "image")
            ui->widget->setSceneType(OpenGLWidget::TYPE_IMAGE);

        else if(type == "video")
            ui->widget->setSceneType(OpenGLWidget::TYPE_VIDEO);

        ui->widget->setFilename(path.toStdString());

        return true;
    }
    else
        return false;
}

bool MainWindow::readCamera(QXmlStreamReader &reader)
{
    float centerX,centerY,centerZ;
    float lookAtX,lookAtY,lookAtZ;
    float upX,upY,upZ;

    readNext(reader);

    if(reader.name() != "center")
        return false;

    if(!readVec3(reader,centerX,centerY,centerZ))
        return false;


    if(reader.name() != "lookat")
        return false;

    if(!readVec3(reader,lookAtX,lookAtY,lookAtZ))
        return false;

    if(reader.name() != "up")
        return false;

    if(!readVec3(reader,upX,upY,upZ))
        return false;

    ui->widget->setCameraPose(centerX,centerY,centerZ,
                              lookAtX,lookAtY,lookAtZ,
                              upX,upY,upZ);

    return true;
}

bool MainWindow::readVec3(QXmlStreamReader& reader, float &x, float &y, float &z)
{
    readNext(reader);
    if(reader.name() != "float")
        return false;

    x = reader.readElementText().toFloat();

    reader.readNextStartElement();

    if(reader.name() != "float")
        return false;

    y = reader.readElementText().toFloat();

    reader.readNextStartElement();

    if(reader.name() != "float")
        return false;

    z = reader.readElementText().toFloat();

    readNext(reader); readNext(reader);

    return true;
}


void MainWindow::readNext(QXmlStreamReader &reader)
{
    // As QXMLStreamReader does not skip white space

    reader.readNext();
    while(reader.name() == "" && !reader.atEnd())
        reader.readNext();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    ui->widget->keyPressEvent(event);
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    ui->widget->wheelEvent(event);
}

EditCameraWidget *MainWindow::getEditWidget()
{
    return editCameraWidget;
}

void MainWindow::setEditWidget(EditCameraWidget *value)
{
    editCameraWidget = value;
}


bool MainWindow::readObject(QXmlStreamReader& reader)
{
    QString type = reader.attributes().value("type").toString();
    ui->widget->setObjectType(type.toStdString());
     if(type == "obj")
     {
         QString path;
         readPath(reader,path);
         Mesh* mesh = new Mesh;
         if(!ObjectLoader::loadObjectFromFile(path.toStdString().c_str(),mesh))
         {
            QMessageBox::warning(this,"Error","Failed to load Object data at path " + path);
            return false;
         }

         readNext(reader);

         float tx,ty,tz;
         float rx,ry,rz;
         float sx,sy,sz;

         if(reader.name() != "translation")
             return false;

         if(!readVec3(reader,tx,ty,tz))
             return false;


         if(reader.name() != "rotation")
             return false;

         if(!readVec3(reader,rx,ry,rz))
             return false;

         if(reader.name() != "scale")
             return false;

         if(!readVec3(reader,sx,sy,sz))
             return false;

         mesh->setRotate(glm::vec3(rx,ry,rz)*57.2957795f);
         mesh->setTranslate(glm::vec3(tx,ty,tz));
         mesh->setScale(glm::vec3(sx,sy,sz));

         ui->widget->setMesh(mesh);
         ui->widget->setObjectPath(path.toStdString());
         ui->widget->setObjectType("obj");

         return true;
     }

     return false;
}

bool MainWindow::readProjection(QXmlStreamReader &reader)
{
    QString type = reader.attributes().value("type").toString();
    bool isPerspective = true;
    if(type == "orthographic")
        isPerspective = false;

    float fov, nearplane, farplane;

    if(!readVec3(reader,fov,nearplane,farplane))
        return false;

    ui->widget->setProjection(fov,nearplane,farplane,isPerspective);

    return true;
}

bool MainWindow::readPath(QXmlStreamReader &reader, QString &path)
{
    readNext(reader);
    path = reader.readElementText();

    return true;
}

bool MainWindow::parseScene(QFile& XMLfile)
{
    if(!XMLfile.open(QIODevice::ReadOnly | QFile::Text))
        return false;

    QXmlStreamReader reader;
    reader.setDevice(&XMLfile);

    readNext(reader);
    while(!reader.atEnd())
    {
        if(reader.isStartElement())
        {
            if(reader.name() == "scene")
            {
                QFileInfo fileInf(XMLfile.fileName());
                setWindowTitle(fileInf.fileName());
            }
            else if(reader.name() == "file")
            {
               if(!readBackgroundFile(reader))
                   return false;
            }
            else if(reader.name() == "object")
            {
                if(!readObject(reader))
                    return false;
            }
            else if(reader.name() == "camera")
            {
                if(!readCamera(reader))
                    return false;
            }
            else if(reader.name() == "projection")
            {
                if(!readProjection(reader))
                    return false;
            }
            readNext(reader);
        }
        else
        {
            readNext(reader);
        }
    }

    ui->widget->initScene();

    return true;
}

bool MainWindow::saveScene(QFile& XMLfile)
{
    QXmlStreamWriter writer(&XMLfile);
    OpenGLWidget* scene = ui->widget;
    float centerX,centerY,centerZ;
    float lookatX,lookatY,lookatZ;
    float upX,upY,upZ;
    scene->getCameraPose(centerX,centerY,centerZ,
                         lookatX,lookatY,lookatZ,
                         upX,upY,upZ
                         );

    float transX, transY,transZ,pitch,yaw,roll,scaleX,scaleY,scaleZ;
    scene->getTransformation(transX,transY,transZ,
                             pitch, yaw,   roll,
                             scaleX,scaleY,scaleZ);

    float nearPlane,farPlane,fov;
    bool isPerspective;
    scene->getProjection(fov,nearPlane,farPlane,isPerspective);

    writer.setAutoFormatting(true);
    writer.writeStartDocument();
        writer.writeStartElement("scene");

            writer.writeStartElement("file");
                switch(scene->getSceneType())
                {
                    case OpenGLWidget::TYPE_IMAGE:
                        writer.writeAttribute("type","image");
                    break;
                    case OpenGLWidget::TYPE_VIDEO:
                        writer.writeAttribute("type","video");
                    break;
                }
                writer.writeTextElement("path",QString(scene->getFilename().c_str()));
            writer.writeEndElement();

        writer.writeStartElement("object");
            writer.writeAttribute("type",QString(scene->getObjectType().c_str()));
            writer.writeTextElement("path",QString(scene->getObjectPath().c_str()));

            writer.writeStartElement("translation");

                writer.writeTextElement("float",QString::number(transX));
                writer.writeTextElement("float",QString::number(transY));
                writer.writeTextElement("float",QString::number(transZ));

            writer.writeEndElement();
            writer.writeStartElement("rotation");

                writer.writeTextElement("float",QString::number(roll));
                writer.writeTextElement("float",QString::number(yaw));
                writer.writeTextElement("float",QString::number(pitch));

            writer.writeEndElement();
            writer.writeStartElement("scale");

                writer.writeTextElement("float",QString::number(scaleX));
                writer.writeTextElement("float",QString::number(scaleY));
                writer.writeTextElement("float",QString::number(scaleZ));

            writer.writeEndElement();

        writer.writeEndElement();

        writer.writeStartElement("camera");
            writer.writeStartElement("center");

                writer.writeTextElement("float",QString::number(centerX));
                writer.writeTextElement("float",QString::number(centerY));
                writer.writeTextElement("float",QString::number(centerZ));

            writer.writeEndElement();
            writer.writeStartElement("lookat");

                writer.writeTextElement("float",QString::number(lookatX));
                writer.writeTextElement("float",QString::number(lookatY));
                writer.writeTextElement("float",QString::number(lookatZ));

            writer.writeEndElement();
            writer.writeStartElement("up");

                writer.writeTextElement("float",QString::number(upX));
                writer.writeTextElement("float",QString::number(upY));
                writer.writeTextElement("float",QString::number(upZ));

            writer.writeEndElement();
        writer.writeEndElement();

        writer.writeStartElement("projection");
            if(isPerspective)
                writer.writeAttribute("type","perspective");
            else
                writer.writeAttribute("type","orthographic");

            writer.writeStartElement("float");
                writer.writeAttribute("type","fov");
                writer.writeCharacters(QString::number(fov));
            writer.writeEndElement();
            writer.writeStartElement("float");
                writer.writeAttribute("type","near");
                writer.writeCharacters(QString::number(nearPlane));
            writer.writeEndElement();
            writer.writeStartElement("float");
                writer.writeAttribute("type","far");
                writer.writeCharacters(QString::number(farPlane));
            writer.writeEndElement();
        writer.writeEndElement();
    writer.writeEndDocument();

    return true;
}

void MainWindow::on_actionLoad_Scene_triggered()
{
    on_actionEditObject_triggered(false);
    on_actionEditCamera_triggered(false);
    QString filename = QFileDialog::getOpenFileName(this,tr("Open Scene File"),"",tr("Scene Files (*.scene);"));

    if(!filename.isEmpty())
    {
         QFile file(filename);

         ui->actionEditCamera->setChecked(false);
         ui->actionEditObject->setChecked(false);

         if(!parseScene(file))
         {
             QMessageBox::critical(this,tr("Error"),tr("Failed to load scene"));
             ui->actionEditObject->setEnabled(false);
             ui->actionEditCamera->setEnabled(false);
         }

         isSceneLoaded = true;
         ui->actionEditObject->setEnabled(isSceneLoaded);
         ui->actionEditCamera->setEnabled(isSceneLoaded);

         openFile = filename.toStdString();

         file.close();
    }
}

void MainWindow::on_actionSave_Scene_triggered()
{
    if(openFile == "")
        on_actionSave_Scene_As_triggered();
    else
    {
        QString filename(openFile.c_str());
        QFile file(filename);
        file.open(QFile::WriteOnly);

        if(!saveScene(file))
            QMessageBox::critical(this,tr("Error"),tr("Failed to save scene"));
        else
            QMessageBox::information(this,tr("Information"),tr("The scene was successfully saved"));

        file.close();
    }
}



void MainWindow::on_actionSave_Scene_As_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Save Scene"),"",tr("Scene Files (*.scene);"));

    if(!filename.isEmpty())
    {
         QFile file(filename);
         file.open(QFile::WriteOnly);

         if(!saveScene(file))
             QMessageBox::critical(this,tr("Error"),tr("Failed to save scene"));
         else
             QMessageBox::information(this,tr("Information"),tr("The scene was successfully saved"));

         openFile = filename.toStdString();
         QFileInfo fileInf(file);
         setWindowTitle(fileInf.fileName());

         file.close();
    }
}

void MainWindow::on_actionQuit_triggered()
{
    qApp->quit();
}

void MainWindow::on_stopFrameButton_clicked()
{
    ui->widget->setIsPlaying(false);
    ui->widget->setFrame(0);

    ui->playButton->setChecked(false);
}

void MainWindow::on_prevFrameButton_clicked()
{
    ui->widget->decFrame();
}

void MainWindow::on_nextFrameButton_clicked()
{
    ui->widget->incFrame();
}

void MainWindow::on_playButton_clicked(bool checked)
{
    if(checked)
        ui->widget->setIsPlaying(true);
    else
        ui->widget->setIsPlaying(false);
}

void MainWindow::on_actionHelp_triggered()
{
    QMessageBox::information(this,tr("Help"),tr("Activate the Object transformations through Edit->Object.\nActivate the Camera movement through Edit->Camera.\nDo not forget to save your scene after editing!"));
}

void MainWindow::on_actionEditCamera_triggered(bool checked)
{
    if(checked)
    {
        editCameraWidget = new EditCameraWidget();
        editCameraWidget->setCamera(ui->widget->getCamera());
        editCameraWidget->show();
        ui->widget->getCamera()->setEditCameraWidget(editCameraWidget);
    }
    else
    {
        if(editCameraWidget)
            delete editCameraWidget;
        editCameraWidget = nullptr;
    }
    ui->widget->setCameraEdit(checked);
}
Ui::MainWindow *MainWindow::getUi()
{
    return ui;
}

void MainWindow::setUi(Ui::MainWindow *value)
{
    ui = value;
}



void MainWindow::on_actionEditObject_triggered(bool checked)
{
    if(checked)
    {
        editObjectWidget = new EditObjectWidget();
        editObjectWidget->setMesh(ui->widget->getMesh());
        editObjectWidget->show();
        ui->widget->getMesh()->setEditObjectWidget(editObjectWidget);
    }
    else
    {
        if(editObjectWidget)
            delete editObjectWidget;
        editObjectWidget = nullptr;
    }
}

void MainWindow::on_actionDo_Visibility_Check_triggered(bool checked)
{
    ui->widget->getMesh()->doVisibilityCheck(checked);
}
