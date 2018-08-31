#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <jpeglib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <linux/videodev2.h>

#include<QImage>
#include<QDebug>
#include<QTimer>


#define NB_BUFFER           4
#define HEADERFRAME1     0xaf
#define VIDEO_FORMAT     V4L2_PIX_FMT_YUYV
#define BUFFER_COUNT     1



typedef struct VideoBuffer {
    void   *start;
    size_t  length;
} VideoBuffer;


struct vdIn {
    int fd;
//    char *videodevice;
//    char *status;
//    char *pictName;
    struct v4l2_capability cap;
    struct v4l2_format fmt;
    struct v4l2_buffer buf;
    struct v4l2_requestbuffers rb;
//    void *mem[2];
//    unsigned char *tmpbuffer;
//    unsigned char *framebuffer;
    int isstreaming;
    int grabmethod;
    unsigned int width;
    unsigned int height;
    int fps;
    int formatIn;
    int formatOut;
    unsigned int framesizeIn;
    int signalquit;
    int toggleAvi;
    int getPict;
    int rawFrameCapture;
    /* raw frame capture */
    unsigned int fileCounter;
    /* raw frame stream capture */
    unsigned int rfsFramesWritten;
    unsigned int rfsBytesWritten;
    /* raw stream capture */
//    FILE *captureFile;
    int framecount;
    int recordstart;
    int recordtime;
};

struct control_data {
  struct vdIn videoIn;
  int width;
  int height;
  int video_dev;
  int quality;
  int fps, daemon;
  int format;
//  char *filename;
//  pthread_t tcam;
//  pthread_t trecorder;
};


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	int init_v4l2();
	int v4l2_enbale();
	int v4l2_disable();
//	int update_video();
	int disaply_image();


/* convert */
	int convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width,unsigned int height);
	int convert_yuv_to_rgb_pixel(int y, int u, int v);


	struct control_data cd;
	unsigned char *tmpbuffer;
	unsigned char *framebuffer;
	void *mem[4];

	struct v4l2_requestbuffers reqbuf;
	struct v4l2_buffer buf;
	VideoBuffer framebuf1[BUFFER_COUNT];
	QTimer* time1;
	QImage *frame;

public slots:
//     int disaply_image();
	 int update_video();

private:
    Ui::MainWindow *ui;
	
};

#endif // MAINWINDOW_H
