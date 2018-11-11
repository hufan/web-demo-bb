#include "v4l2_camera.h"

v4l2_camera::v4l2_camera()
{

}

//#define  deviceName11   "/dev/video2"
int v4l2_camera::deviceOpen()
{
    fd = v4l2_open(deviceName, O_RDWR /* required */ | O_NONBLOCK, 0);
    if(fd < 0)  return -1;

    return 0;
}
int v4l2_camera::deviceClose()
{
    ::close(fd);
}
int  xioctl(int fd, int request, void *argp)
{
    int r;
    do r = v4l2_ioctl(fd, request, argp);
    while (-1 == r && EINTR == errno);
    return r;
}

int  v4l2_camera::deviceInit()
{
    struct v4l2_capability cap;
    struct v4l2_cropcap cropcap;
    struct v4l2_crop crop;
    struct v4l2_format fmt;
    struct v4l2_streamparm frameint;
    unsigned int min;
    int ret = 0;
    n_buffers = 0;

   if(width<=0)
	   width=320;
   if(height<=0)
	   height=240;
   if(fps<=0)
	   fps=15;

   continuous=0;

    ret = xioctl(fd, VIDIOC_QUERYCAP, &cap);
    if(ret < 0)
        qDebug()  << "devices is no V4L2 device";

    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
    {
        fprintf(stderr, "%s is no video capture device\n", deviceName);
        //        exit(EXIT_FAILURE);
        qDebug()  << "device is no video capture device";
    }

    if (!(cap.capabilities & V4L2_CAP_READWRITE))
    {
        fprintf(stderr, "%s does not support read i/o\n", deviceName);
        //        exit(EXIT_FAILURE);
        qDebug()  << "devicedoes not support read i/o ";
    }

    /* Select video input, video standard and tune here. */
    CLEAR(cropcap);
    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (0 == xioctl(fd, VIDIOC_CROPCAP, &cropcap))
    {
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c = cropcap.defrect; /* reset to default */

        if (-1 == xioctl(fd, VIDIOC_S_CROP, &crop))
        {
            switch (errno)
            {
            case EINVAL:
                /* Cropping not supported. */
                break;
            default:
                /* Errors ignored. */
                break;
            }
        }
    }
    else
    {
        /* Errors ignored. */
    }

    CLEAR(fmt);
    // v4l2_format
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = width;
    fmt.fmt.pix.height = height;
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
    //fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;  //V4L2_PIX_FMT_RGB24
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;

    ret = xioctl(fd, VIDIOC_S_FMT, &fmt);
    if(ret < 0)
    {
        qDebug()  << "VIDIOC_S_FMT ERROR";
        //        exit(EXIT_FAILURE);
    }
    //    if (fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_YUV420) {
    //        fprintf(stderr,"Libv4l didn't accept YUV420 format. Can't proceed.\n");
    //        exit(EXIT_FAILURE);
    //    }
    /* Note VIDIOC_S_FMT may change width and height. */
    if (width != fmt.fmt.pix.width)
    {
        width = fmt.fmt.pix.width;
        fprintf(stderr, "Image width set to %i by device %s.\n", width, deviceName);
    }
    if (height != fmt.fmt.pix.height)
    {
        height = fmt.fmt.pix.height;
        fprintf(stderr, "Image height set to %i by device %s.\n", height, deviceName);
    }

    /* If the user has set the fps to -1, don't try to set the frame interval */
    if (fps != -1)
    {
        CLEAR(frameint);
        /* Attempt to set the frame interval. */
        frameint.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        frameint.parm.capture.timeperframe.numerator = 1;
        frameint.parm.capture.timeperframe.denominator = fps;
        if (-1 == xioctl(fd, VIDIOC_S_PARM, &frameint))
            fprintf(stderr, "Unable to set frame interval.\n");
    }
    /* Buggy driver paranoia. */
    min = fmt.fmt.pix.width * 2;
    if (fmt.fmt.pix.bytesperline < min)
        fmt.fmt.pix.bytesperline = min;
    min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
    if (fmt.fmt.pix.sizeimage < min)
        fmt.fmt.pix.sizeimage = min;

    mmapInit();
    return 0;
}

int v4l2_camera::xioctl(int fd, int request, void *argp)
{
    int r;

    do r = v4l2_ioctl(fd, request, argp);
    while (-1 == r && EINTR == errno);

    return r;
}
int  v4l2_camera::mmapInit(void)
{
    struct v4l2_requestbuffers req;

    CLEAR(req);

    req.count = 2;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req))
    {
        if (EINVAL == errno)
        {
            fprintf(stderr, "%s does not support memory mapping\n", deviceName);
            //            exit(EXIT_FAILURE);
            qDebug()  << "does not support memory mapping  ";
        }
        else
        {
            //            errno_exit("VIDIOC_REQBUFS");
            qDebug()  << "VIDIOC_REQBUFS ERROR 8";
        }
    }

    if (req.count < 2)
    {
        fprintf(stderr, "Insufficient buffer memory on %s\n", deviceName);
        //        exit(EXIT_FAILURE);
        qDebug()  << "Insufficient buffer memory ";
    }

    buffers = (buffer *)calloc(req.count, sizeof(*buffers));

    if (!buffers)
    {
        fprintf(stderr, "Out of memory\n");
        //        exit(EXIT_FAILURE);
        qDebug()  << "Out of memory ";
    }

    for (n_buffers = 0; n_buffers < req.count; ++n_buffers)
    {
        struct v4l2_buffer buf;

        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = n_buffers;

        if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
        {
            qDebug()  << "VIDIOC_REQBUFS ERROR 777";
            return -1;
        }

        buffers[n_buffers].length = buf.length;
        buffers[n_buffers].start = v4l2_mmap(NULL /* start anywhere */, buf.length, PROT_READ | PROT_WRITE /* required */, MAP_SHARED /* recommended */, fd, buf.m.offset);

        if (MAP_FAILED == buffers[n_buffers].start)
        {
			qDebug()  << "mmap  \n";
			return -1;
		}

    }
    return 0;
}
int  v4l2_camera::captureStart(void)
{
    unsigned int i;
    enum v4l2_buf_type type;
    struct v4l2_buffer buf;

    for (i = 0; i < n_buffers; ++i)
    {
        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
            qDebug()  << "VIDIOC_QBUF VIDIOC_STREAMON ";
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
    {
        qDebug()  << "VIDIOC_STREAMON ERROR  9";
    }

    return 0;
}
int  v4l2_camera::captureStop(void)
{
    enum v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (-1 == xioctl(fd, VIDIOC_STREAMOFF, &type))
    {
        qDebug()  << "VIDIOC_STREAMOFF ERROR  ";
        return -1;
    }
    return 0;
}
int  v4l2_camera::deviceUninit(void)
{
    unsigned int i;
    for (i = 0; i < n_buffers; ++i)
        if (-1 == v4l2_munmap(buffers[i].start, buffers[i].length))
            //            errno_exit("munmap");
            qDebug()  << "munmap ERROR  ";
}

int  v4l2_camera::frameRead(void)
{
    //    struct v4l2_buffer buf;
    CLEAR(buf);

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;

    if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf))
    {
        qDebug()  << " VIDIOC_DQBUF ERROR";
    }
    assert(buf.index < n_buffers);
    //imageProcess(buffers[buf.index].start,buf.timestamp);
    //save_rgb_to_jpg((char *)buffers[buf.index].start, width, height, "/home/root/h11115555.jpg");

    if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
    {
        qDebug()  << "VIDIOC_QBUF ERROR";
        return -1;
    }

    //    QImage image = QImage((unsigned char *)buffers[buf.index].start, width,height, QImage::Format_RGB888);
    //    image.scaled(ui->label->size(), Qt::KeepAspectRatio);
    //    ui->label->setScaledContents(true);
    //    ui->label->setPixmap(QPixmap::fromImage(image));
    return 1;
}

int  v4l2_camera::mainloop()
{
    int count;
    int ret = 0;
    unsigned int numberOfTimeouts;

    numberOfTimeouts = 0;
    count = 1;

    while (count-- > 0)
    {
        for (;;)
        {
            fd_set fds;
            struct timeval tv;
            int r;
            FD_ZERO(&fds);
            FD_SET(fd, &fds);
            /* Timeout. */
            tv.tv_sec = 0;
            tv.tv_usec = 300;
            r = select(fd + 1, &fds, NULL, NULL, &tv);
            if (-1 == r)
            {
                if (EINTR == errno)
                    continue;
                //errno_exit("select");
                exit(EXIT_FAILURE);
            }
            if (0 == r)
            {
                if (numberOfTimeouts <= 0)
                {
                    count++;
                }
                else
                {
                    fprintf(stderr, "select timeout\n");
                    exit(EXIT_FAILURE);
                }
            }
            if(continuous == 1)
            {
                count = 1;
            }
            if (frameRead())
            {
                ret = 1;
                break;
            }

            /* EAGAIN - continue select loop. */
        }
    }
    return ret;
}

int v4l2_camera::save_rgb_to_jpg(char *soureceData, int imgWidth, int imgHeight, char *fileName)
{
    int depth = 3;//1 for gray, 3 for color
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *outfile;
    JSAMPROW row_pointer[1];        // pointer to JSAMPLE row[s]
    int row_stride = imgWidth;      // physical row width in image buffer

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    if ((outfile = fopen(fileName, "wb")) == NULL)
    {
        fprintf(stderr, "can't open %s\n", fileName);
        return -1;
    }
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = imgWidth;      // image width and height, in pixels
    cinfo.image_height = imgHeight;
    cinfo.input_components = depth;    // of color components per pixel
    cinfo.in_color_space = JCS_RGB;    // or JCS_GRAYSCALE;   colorspace of input image

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, JPEG_QUALITY, TRUE ); /* limit to baseline-JPEG values */

    jpeg_start_compress(&cinfo, TRUE);

    row_stride = imgWidth; /* JSAMPLEs per row in image_buffer */

    while (cinfo.next_scanline < cinfo.image_height)
    {
        row_pointer[0] = (JSAMPROW)&soureceData[cinfo.next_scanline * row_stride * depth];
        (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    fclose(outfile);

    return 0;
}

int  v4l2_camera::save_jpeg(char *soureceData, int imgWidth, int imgHeight, char *fileName)
{
    save_rgb_to_jpg(soureceData, imgWidth, imgHeight, fileName);
}
