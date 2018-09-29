#ifndef V4L2_CAMERA_H
#define V4L2_CAMERA_H

#include <QTimer>
#include <QDebug>

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
#include <time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <linux/videodev2.h>
#include <jpeglib.h>
#include <libv4l2.h>
#include <signal.h>
#include <stdint.h>
#include <inttypes.h>



#define VIDIOC_REQBUFS_COUNT   2
#define JPEG_QUALITY           70

#define CLEAR(x) memset (&(x), 0, sizeof (x))


struct buffer
{
    void                   *start;
    size_t                  length;
};

class v4l2_camera
{
private:
    int  mmapInit(void);
    int  xioctl(int fd, int request, void *argp);
    int  frameRead(void);
    int  save_rgb_to_jpg(char *soureceData, int imgWidth, int imgHeight, char *fileName);

public:
    v4l2_camera();

    int deviceOpen();
    int deviceClose();
    int  deviceInit();
    int  deviceUninit(void);
    int  captureStart(void);
    int  captureStop(void);

    int  mainloop();
    int  save_jpeg(char *soureceData, int imgWidth, int imgHeight, char *fileName);

    int fd;
    int fps;
    unsigned int width ;
    unsigned int height ;
    int continuous;
    buffer          *buffers;
    unsigned int     n_buffers;

    char * deviceName;

    struct v4l2_buffer buf;
};

#endif // V4L2_CAMERA_H
