#ifndef SYSTEMCONTENTWIDGET_H
#define SYSTEMCONTENTWIDGET_H

#include <QMainWindow>
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QList>
#include <QSplitter>
#include <QLabel>
#include <QVBoxLayout>
#include <QTableView>
//#include "mxde.h"

#include <QGroupBox>
#include <QSplitter>
#include <QLabel>
#include <QVBoxLayout>
#include <QTableView>
#include <QPushButton>

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QList>
#include <QTextEdit>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QHeaderView>

#include "mxapplication.h"
#include "basewidget.h"

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


#include <jpeglib.h>



#if !defined(IO_READ) && !defined(IO_MMAP) && !defined(IO_USERPTR)
#define IO_READ
#define IO_MMAP
#define IO_USERPTR
#endif


#define CLEAR(x) memset (&(x), 0, sizeof (x))

#ifndef VERSION
#define VERSION "unknown"
#endif

#if defined(IO_MMAP) || defined(IO_USERPTR)
// minimum number of buffers to request in VIDIOC_REQBUFS call
#define VIDIOC_REQBUFS_COUNT 2
#endif

typedef enum {
#ifdef IO_READ
        IO_METHOD_READ,
#endif
#ifdef IO_MMAP
        IO_METHOD_MMAP,
#endif
#ifdef IO_USERPTR
        IO_METHOD_USERPTR,
#endif
} io_method;

struct buffer {
        void *                  start;
        size_t                  length;
};




#define DETECT_FACE  1
//#define DETECT_EYE  1

#define CAMERA_DEVICE "/dev/video1"

#define NB_BUFFER           4
#define HEADERFRAME1     0xaf
#define VIDEO_FORMAT     V4L2_PIX_FMT_YUYV
#define BUFFER_COUNT     1

#define  WIDTH    800
#define  HEIGHT   600

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

class SystemContentWidget : public BaseWidget
{
    Q_OBJECT
public:
//    MxDE    *m_mxde;
    SystemContentWidget(QWidget *parent = 0,MxApplication *obj = 0, int width = 0, int height = 0);
    ~SystemContentWidget();
    void setApplication(MxApplication *app);
//    void setDbusProxy(MxDE *mxde);
    void setParentWindow(QWidget *parent);
    void initUI();
    void initConnection();
    void display();


    QTimer *testTimer;
    int fffff;
    int test_int;

    struct control_data cd;
    unsigned char *tmpbuffer;
    unsigned char *framebuffer;
    void *mem[4];

    struct v4l2_requestbuffers reqbuf;
    struct v4l2_buffer buf;
    VideoBuffer framebuf1[BUFFER_COUNT];
    QTimer* time1;
    QImage *frame;

    int init_v4l2();
    int v4l2_enbale();
    int v4l2_disable();
    int update_video();
    int disaply_image();
    QImage convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width,unsigned int height);
    int convert_yuv_to_rgb_pixel(int y, int u, int v);
	int xioctl(int fd, int request, void* argp);
	void mmapInit(void);
	void userptrInit(unsigned int buffer_size);
	void readInit(unsigned int buffer_size);
	void errno_exit(const char* s);
	void captureStart(void);
	void captureStop(void);
	int frameRead(void);
	int save_rgb_to_jpg(char *soureceData, int imgWidth, int imgHeight, char * fileName);
	void deviceUninit(void);

	

public slots:
    void timerDone();
    int clickInsertData_sqlite();
//    int clickDeleteData_sqlite();

private:
    QWidget                 *m_parent;
    MxApplication           *m_app;
    int                      m_width;
    int                      m_height;

    QStringList          infoList;
    QLabel               *m_show_pic;
//    QGroupBox            *m_Grbox_Group;
//    QGridLayout          *m_Grbox_Layout;
//    QGridLayout          *mainLayout;



    QPushButton          *m_insert_Button;
    QPushButton          *m_delete_Button;
    QPushButton          *m_update_Button;

    QGroupBox            *m_Grbox_Group;
    QGroupBox            *m_Grbox_Group2;
    QGridLayout          *m_Grbox_Layout;
    QGridLayout          *mainLayout;

    QTableView           *m_tableview;



	
	io_method        io;
	int              fd;
	buffer *         buffers;
	unsigned int     n_buffers;
	int 		     continuous;
	unsigned int 	 fps;
	unsigned char    jpegQuality;
	char* 			 deviceName;
	int width;
	int height;

	
};

#endif // SYSTEMCONTENTWIDGET_H
