#-------------------------------------------------
#
# Project created by QtCreator 2014-03-11T20:33:36
#
#-------------------------------------------------
QT       += core gui opengl xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LineDescriptor
TEMPLATE = app


SOURCES += main.cpp\
    openglwidget.cpp \
    mainwindow.cpp \
    objectloader.cpp \
    Shader.cpp \
    mesh.cpp \
    camera.cpp \
    editcamerawidget.cpp \
    Line.cpp \
    line3d.cpp \
    editobjectwidget.cpp \
    Particle.cpp \
    renderer.cpp

HEADERS  += \
    openglwidget.h \
    mainwindow.h \
    objectloader.h \
    Shader.h \
    mesh.h \
    camera.h \
    editcamerawidget.h \
    editobjectwidget.h \
    Line.h \
    line3d.h \
    Particle.h \
    renderer.h

FORMS    += \
    mainwindow.ui \
    editcamerawidget.ui \
    editobjectwidget.ui


RESOURCES +=
	
	
INCLUDEPATH += ..\..\glm-0.9.5.4\glm\

win32: LIBS += -L$$PWD/../../glew-1.10.0/lib/Release/Win32/ -lglew32

INCLUDEPATH += $$PWD/../../glew-1.10.0/include
DEPENDPATH += $$PWD/../../glew-1.10.0/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../opencv/libs -lopencv_core300d -lopencv_calib3d300d -lopencv_flann300d -lopencv_features2d300d -lopencv_highgui300d -lopencv_imgproc300d -lopencv_photo300d -lopencv_video300d -lopencv_objdetect300d -lopencv_legacy300d
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../opencv/libs -lopencv_core300d -lopencv_calib3d300d -lopencv_flann300d -lopencv_features2d300d -lopencv_highgui300d -lopencv_imgproc300d -lopencv_photo300d -lopencv_video300d -lopencv_objdetect300d -lopencv_legacy300d

INCLUDEPATH += $$PWD/../../../../opencv/include
DEPENDPATH += $$PWD/../../../../opencv/include
