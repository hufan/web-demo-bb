#-------------------------------------------------
#
# Project created by QtCreator 2018-08-31T18:27:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = face_detect
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui


#LIBS +=/opt/qt5-opencv3.1/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/lib -lopencv_calib3d  -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgproc  -lopencv_ml  -lopencv_objdetect  -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_imgcodecs -lopencv_video -lopencv_videostab -latomic -ltbb   -lrt -lpthread -lm -ldl  -fPIC -lrt -pthread

INCLUDEPATH += /opt/qt5-opencv3.1/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/include/ \
opt/qt5-opencv3.1/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/include/opencv \
opt/qt5-opencv3.1/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/include/opencv2
#/home/hape_predictor_68_face_landmarks/dlib-18.17/dlib

LIBS += /opt/qt5-opencv3.1/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/lib/libopencv_calib3d.so \
/opt/qt5-opencv3.1/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/lib/libopencv_core.so \
/opt/qt5-opencv3.1/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/lib/libopencv_features2d.so \
/opt/qt5-opencv3.1/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/lib/libopencv_flann.so \
/opt/qt5-opencv3.1/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/lib/libopencv_highgui.so \
/opt/qt5-opencv3.1/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/lib/libopencv_imgcodecs.so \
/opt/qt5-opencv3.1/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/lib/libopencv_imgproc.so \
/opt/qt5-opencv3.1/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/lib/libopencv_ml.so \
/opt/qt5-opencv3.1/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/lib/libopencv_objdetect.so \
/opt/qt5-opencv3.1/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/lib/libopencv_photo.so \
/opt/qt5-opencv3.1/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/lib/libopencv_shape.so \
/opt/qt5-opencv3.1/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/lib/libopencv_stitching.so \
/opt/qt5-opencv3.1/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/lib/libopencv_superres.so \
/opt/qt5-opencv3.1/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/lib/libopencv_videoio.so \
/opt/qt5-opencv3.1/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/lib/libopencv_video.so



LIBS += -lv4lconvert

