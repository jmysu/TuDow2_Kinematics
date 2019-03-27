#-------------------------------------------------
#
# Project created by QtCreator 2019-03-18T14:53:52
#
#-------------------------------------------------

QT       += core gui xml opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = myGLKinematics
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    gl_viewer.cpp \
    kinematics.cpp \
    gl_teapot.cpp \
    gl_robot.cpp

HEADERS += \
        mainwindow.h \
    gl_viewer.h \
    kinematics.h \
    gl_teapotdata.h \
    gl_teapot.h \
    gl_robot.h

FORMS += \
        mainwindow.ui

win32: {
    LIBS += -lopengl32 -lglu32
    CONFIG(release, debug|release):  {
        LIBS += "$$PWD/../../libQGLViewer/QGLViewer/QGLViewer2.dll"
        message("Release!")
    } else {
        LIBS += "$$PWD/../../libQGLViewer/QGLViewer/QGLViewerd2.dll"
        message("Debug!")
        }
    }
else:mac: LIBS += -F$$PWD/../../libQGLViewer/QGLViewer/ -framework QGLViewer
else:unix: LIBS += -L$$PWD/../../libQGLViewer/QGLViewer/ -lQGLViewer

INCLUDEPATH += $$PWD/../../libQGLViewer/QGLViewer
DEPENDPATH += $$PWD/../../libQGLViewer/QGLViewer

#For Mac: remember to add QGLViewer framework path to DYLD_FRAMEWORK_PATH
#For Windows: remember copy QGLViewerxx.dll to Windows/System32
