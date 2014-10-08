#include "mainwindow.h"
#include <QApplication>
#include "particle.h"
#include "GL/glew.h"
#include <QMessageBox>
#include "glwidget.h"
#include <QtOpenGL>

int main(int argc, char *argv[])
{
    printf("test2\n");

    Q_INIT_RESOURCE(framebufferobject2);

    QApplication a(argc, argv);
    if (!QGLFormat::hasOpenGL() || !QGLFramebufferObject::hasOpenGLFramebufferObjects()) {
        QMessageBox::information(0, "OpenGL framebuffer objects 2",
                                 "This system does not support OpenGL/framebuffer objects.");
        return -1;
    }

    MainWindow w;
    w.show();

//    GLWidget widget(0);
//    widget.resize(640, 480);
//    widget.show();


    return a.exec();
}
