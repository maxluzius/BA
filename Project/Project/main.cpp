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

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
