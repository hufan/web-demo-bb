#include "systemcontentwidget.h"

#include <QDebug>
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QList>
#include <QVBoxLayout>
#include <QLabel>
#include <QStandardItemModel>
#include <QFile>
#include <QTextCodec>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTimer>
//#include <QTimer>
#include <iostream>

#include <qpainter.h>

#include "systemcontentwidget.h"
#include "constant.h"
#include "mxapplication.h"


#define JPEG_QUALITY  70

SystemContentWidget::SystemContentWidget(QWidget *parent, MxApplication *obj, int width , int height )
            :BaseWidget(parent,obj)
{
    
	
    m_width = width;
    if(m_width <=0){
        m_width = DEFAULT_SCREEN_WIDTH;
    }
    m_height = height;
    if(m_height <= 0){
        m_height = DEFAULT_SCREEN_HEIGHT;
    }

    this->setFixedSize(m_width, m_height);
	
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(Qt::white));
    this->setPalette(palette);


	char deviceName1[30] = "/dev/video1";
	fps = 30;
	jpegQuality = 70;
	int continuous = 0;
    deviceName = deviceName1;
	io          = IO_METHOD_MMAP;

	
	width=800;
	height=600;

	
//    init_v4l2();

    /*init camera  and time */
//    testTimer = new QTimer(this);
//    connect( testTimer, SIGNAL(timeout()),this, SLOT(timerDone()) );
//    testTimer->start( 100 );

}
SystemContentWidget::~SystemContentWidget()
{
    v4l2_disable();
   ::close(cd.videoIn.fd);
}
int SystemContentWidget::xioctl(int fd, int request, void* argp){
	int r;

	do r = ioctl(fd, request, argp);
	while (-1 == r && EINTR == errno);

	return r;

}
#define HUFAN 1
int SystemContentWidget::init_v4l2(){
#if HUFAN

	struct stat st;

	struct v4l2_capability cap;
	struct v4l2_cropcap cropcap;
	struct v4l2_crop crop;
	struct v4l2_format fmt;
	struct v4l2_streamparm frameint;
	unsigned int min;
	QTextStream out(stdout);

	out << "The current cnt is 111111111111"  << endl;
	/* device open */
	// stat file
	if (-1 == stat("/dev/video1", &st)) {
		fprintf(stderr, "Cannot identify '%s': %d, %s\n", "/dev/video1", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	// check if its device
	if (!S_ISCHR(st.st_mode)) {
		fprintf(stderr, "%s is no device\n", "/dev/video1");
		exit(EXIT_FAILURE);
	}

	// open device
	//fd = v4l2_open("/dev/video1", O_RDWR /* required */ | O_NONBLOCK, 0);
	fd = open("/dev/video1", O_RDWR /* required */ | O_NONBLOCK, 0);

	// check if opening was successfull
	if (-1 == fd) {
		fprintf(stderr, "Cannot open '%s': %d, %s\n", "/dev/video1", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}


    /* init devices */

		if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) {
		if (EINVAL == errno) {
			fprintf(stderr, "%s is no V4L2 device\n","/dev/video1");
			exit(EXIT_FAILURE);
		} else {
			errno_exit("VIDIOC_QUERYCAP");
		}
	}

	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		fprintf(stderr, "%s is no video capture device\n","/dev/video1");
		exit(EXIT_FAILURE);
	}
	out << "The current cnt is 22222222222222"  << endl;
	

	switch (io) {
#ifdef IO_READ
		case IO_METHOD_READ:
			if (!(cap.capabilities & V4L2_CAP_READWRITE)) {
				fprintf(stderr, "%s does not support read i/o\n","/dev/video1");
				exit(EXIT_FAILURE);
			}
			break;
#endif

#ifdef IO_MMAP
		case IO_METHOD_MMAP:
#endif
#ifdef IO_USERPTR
		case IO_METHOD_USERPTR:
#endif
#if defined(IO_MMAP) || defined(IO_USERPTR)
      			if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
				fprintf(stderr, "%s does not support streaming i/o\n","/dev/video1");
				exit(EXIT_FAILURE);
			}
			break;
#endif
	}
		out << "The current cnt is 44444444444"  << endl;

	/* Select video input, video standard and tune here. */
	CLEAR(cropcap);

	cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (0 == xioctl(fd, VIDIOC_CROPCAP, &cropcap)) {
		crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		crop.c = cropcap.defrect; /* reset to default */

		if (-1 == xioctl(fd, VIDIOC_S_CROP, &crop)) {
			switch (errno) {
				case EINVAL:
					/* Cropping not supported. */
					break;
				default:
					/* Errors ignored. */
					break;
			}
		}
	} else {
		/* Errors ignored. */
	}
	out << "The current cnt is 66666666666666666"  << endl;

	CLEAR(fmt);

	// v4l2_format
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = width;
	fmt.fmt.pix.height = height;
	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;  //V4L2_PIX_FMT_RGB24
	//fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
	out << "The current cnt is 6767676767676767"  << endl;

	if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
		errno_exit("VIDIOC_S_FMT");
	out << "The current cnt is 67676767676767671111111111111111111111111111111111"  << endl;

	if (fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_YUV420) {
		fprintf(stderr,"Libv4l didn't accept YUV420 format. Can't proceed.\n");
		//exit(EXIT_FAILURE);
	}

	/* Note VIDIOC_S_FMT may change width and height. */
	if (width != fmt.fmt.pix.width) {
		width = fmt.fmt.pix.width;
		fprintf(stderr,"Image width set to %i by device %s.\n", width, "/dev/video1");
	}

	if (height != fmt.fmt.pix.height) {
		height = fmt.fmt.pix.height;
		fprintf(stderr,"Image height set to %i by device %s.\n", height, "/dev/video1");
	}
	
  /* If the user has set the fps to -1, don't try to set the frame interval */

	out << "The current cnt is 121212121212121212"  << endl;
	
  if (fps != -1)
  {
    CLEAR(frameint);
    
    /* Attempt to set the frame interval. */
    frameint.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    frameint.parm.capture.timeperframe.numerator = 1;
    frameint.parm.capture.timeperframe.denominator = fps;
    if (-1 == xioctl(fd, VIDIOC_S_PARM, &frameint))
      fprintf(stderr,"Unable to set frame interval.\n");
  }

	/* Buggy driver paranoia. */
	min = fmt.fmt.pix.width * 2;
	if (fmt.fmt.pix.bytesperline < min)
		fmt.fmt.pix.bytesperline = min;
	min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
	if (fmt.fmt.pix.sizeimage < min)
		fmt.fmt.pix.sizeimage = min;


    	out << "The current cnt is 131313131313131313131"  << endl;
	switch (io) {
#ifdef IO_READ
		case IO_METHOD_READ:
			readInit(fmt.fmt.pix.sizeimage);
			break;
#endif

#ifdef IO_MMAP
		case IO_METHOD_MMAP:
				out << "The current cnt is 1414141414141414114"  << endl;
			mmapInit();
			break;
#endif

#ifdef IO_USERPTR
		case IO_METHOD_USERPTR:
			userptrInit(fmt.fmt.pix.sizeimage);
			break;
#endif
	}


#endif

    out << "The current cnt is 3333333333333333" <<  endl;

#if 0
            int i,ret;
            /*init v4l2*/
	    QTextStream out(stdout);
//    out << "The current datetime is 111111111111" << << endl;
            int width=800,height=600;

            cd.format = V4L2_PIX_FMT_YUYV;
            cd.fps= 15;
            cd.daemon = 0;
            cd.width=width;
            cd.height=height;
            cd.quality = 40;

            cd.videoIn.recordtime = 0;
            cd.videoIn.recordstart = 0;
            cd.videoIn.getPict = 0;
            cd.videoIn.signalquit = 1;
            cd.videoIn.width = width;
            cd.videoIn.height = height;
            cd.videoIn.fps = 15;
            cd.videoIn.formatIn = V4L2_PIX_FMT_YUYV;
            cd.videoIn.grabmethod = 1;
            cd.videoIn.fileCounter = 0;
            cd.videoIn.rawFrameCapture = 0;
            cd.videoIn.rfsBytesWritten = 0;
            cd.videoIn.rfsFramesWritten = 0;

            cd.videoIn.recordtime = 0;
            cd.videoIn.framecount = 0;
            cd.videoIn.getPict = 0;
            cd.videoIn.isstreaming=0;

            if ((cd.videoIn.fd = open(CAMERA_DEVICE, O_RDWR)) == -1) {
                     perror("ERROR opening V4L interface \n");
                        printf("init v4l2  34343");
                     exit(1);
           }

           out << "The current datetime is 111111111111" << endl;
           memset(&cd.videoIn.cap, 0, sizeof(struct v4l2_capability));
           ret = ioctl(cd.videoIn.fd, VIDIOC_QUERYCAP, &cd.videoIn.cap);
           if (ret < 0) {
             printf("Error opening device  unable to query device.\n");
             goto error;
           }

           if ((cd.videoIn.cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == 0) {
             printf("Error opening device  unable to query device.\n");
             goto error;
           }
           if (cd.videoIn.grabmethod) {
             if (!(cd.videoIn.cap.capabilities & V4L2_CAP_STREAMING)) {
             printf("Error opening device  unable to query device.\n");
               goto error;
             }
           } else {
             if (!(cd.videoIn.cap.capabilities & V4L2_CAP_READWRITE)) {
             printf("Error opening device  unable to query device.\n");
               goto error;
             }
           }

           out << "The current datetime is 222222222222222222222222" <<  endl;
          /*
           * set format in
           */
          memset(&cd.videoIn.fmt, 0, sizeof(struct v4l2_format));
          cd.videoIn.fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
          cd.videoIn.fmt.fmt.pix.width = cd.videoIn.width;
          cd.videoIn.fmt.fmt.pix.height = cd.videoIn.height;
          cd.videoIn.fmt.fmt.pix.pixelformat = cd.videoIn.formatIn;
          cd.videoIn.fmt.fmt.pix.field = V4L2_FIELD_ANY;
          ret = ioctl(cd.videoIn.fd, VIDIOC_S_FMT, &cd.videoIn.fmt);
          if (ret < 0) {
            printf("Unable to set format: %d.\n", errno);
            goto error;
          }
          if ((cd.videoIn.fmt.fmt.pix.width != cd.videoIn.width) ||
              (cd.videoIn.fmt.fmt.pix.height != cd.videoIn.height)) {
            printf("format asked unavailable get width %d height %d \n",
                   cd.videoIn.fmt.fmt.pix.width, cd.videoIn.fmt.fmt.pix.height);
                cd.videoIn.width = cd.videoIn.fmt.fmt.pix.width;
                cd.videoIn.height = cd.videoIn.fmt.fmt.pix.height;
            /*
             * look the format is not part of the deal ???
             */
            /* cd.videoIn.formatIn = cd.videoIn.fmt.fmt.pix.pixelformat;*/
          }

          /*
           * set framerate
           */
          struct v4l2_streamparm *setfps;
          setfps = (struct v4l2_streamparm *)malloc(sizeof(struct v4l2_streamparm));
          memset(setfps, 0, sizeof(struct v4l2_streamparm));
          setfps->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

          setfps->parm.capture.timeperframe.numerator = 1;
          setfps->parm.capture.timeperframe.denominator = cd.videoIn.fps;
          ret = ioctl(cd.videoIn.fd, VIDIOC_S_PARM, setfps);

          /*
           * request buffers
           */
          memset(&cd.videoIn.rb, 0, sizeof(struct v4l2_requestbuffers));
          cd.videoIn.rb.count = NB_BUFFER;
          cd.videoIn.rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
          cd.videoIn.rb.memory = V4L2_MEMORY_MMAP;

          ret = ioctl(cd.videoIn.fd, VIDIOC_REQBUFS, &cd.videoIn.rb);
          if (ret < 0) {
            printf("Unable to allocate buffers: %d.\n", errno);
            goto error;
          }
           out << "The current datetime is 3333333333333333333333333333" <<  endl;
          /*
           * map the buffers
           */
          for (i = 0; i < NB_BUFFER; i++) {
            memset(&cd.videoIn.buf, 0, sizeof(struct v4l2_buffer));
            cd.videoIn.buf.index = i;
            cd.videoIn.buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            cd.videoIn.buf.memory = V4L2_MEMORY_MMAP;
            ret = ioctl(cd.videoIn.fd, VIDIOC_QUERYBUF, &cd.videoIn.buf);
            if (ret < 0) {
              printf("Unable to query buffer (%d).\n", errno);
              goto error;
            }
         // if (debug)
         //  printf("length: %u offset: %u\n", cd.videoIn.buf.length, cd.videoIn.buf.m.offset);
            mem[i] = mmap(0 /* start anywhere */ ,
                              cd.videoIn.buf.length, PROT_READ, MAP_SHARED, cd.videoIn.fd,
                              cd.videoIn.buf.m.offset);
            if (mem[i] == MAP_FAILED) {
              printf("Unable to map buffer (%d)\n", errno);
              goto error;
            }
            //if (debug)
            //	  printf("Buffer mapped at address %p.\n", mem[i]);

          }
           out << "The current datetime is 44444444444444444444444" <<  endl;
          /*
           * Queue the buffers.
           */
          for (i = 0; i < NB_BUFFER; ++i) {
            memset(&cd.videoIn.buf, 0, sizeof(struct v4l2_buffer));
            cd.videoIn.buf.index = i;
            cd.videoIn.buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            cd.videoIn.buf.memory = V4L2_MEMORY_MMAP;
            ret = ioctl(cd.videoIn.fd, VIDIOC_QBUF, &cd.videoIn.buf);
            if (ret < 0) {
              printf("Unable to queue buffer (%d).\n", errno);
              goto error;
            }
          }

            /*
             * alloc a temp buffer to reconstruct the pict
             */
            cd.videoIn.framesizeIn = (cd.videoIn.width * cd.videoIn.height << 1);
            switch (cd.videoIn.formatIn) {
            case V4L2_PIX_FMT_JPEG:
            case V4L2_PIX_FMT_MJPEG:
          //	cd.videoIn.tmpbuffer = (unsigned char *) calloc(1, (size_t) cd.videoIn.framesizeIn);
                tmpbuffer = (unsigned char *) calloc(1, (size_t) cd.videoIn.framesizeIn);
              if (!tmpbuffer)
                goto error;
          //	cd.videoIn.framebuffer =
          //		(unsigned char *) calloc(1, (size_t) cd.videoIn.width * (cd.videoIn.height + 8) * 2);
              framebuffer =  (unsigned char *) calloc(1, (size_t) cd.videoIn.width * (cd.videoIn.height + 8) * 2);
              break;
            case V4L2_PIX_FMT_YUYV:
            case V4L2_PIX_FMT_SRGGB8:

          //	cd.videoIn.framebuffer = (unsigned char *) malloc((size_t) cd.videoIn.framesizeIn);
                  framebuffer = (unsigned char *) malloc((size_t) cd.videoIn.framesizeIn);
                  tmpbuffer = (unsigned char *) calloc(1, (size_t) cd.videoIn.framesizeIn);
    //			  tmpbuffer = (unsigned char *) malloc((size_t) cd.videoIn.framesizeIn);
           out << "The current datetime is 555555555555555555555555555555555" << endl;
              break;
            default:
              printf("Unknown format: should never arrive exit fatal !!\n");
              goto error;
              break;
            }

    error:
        free(framebuffer);
        free(tmpbuffer);
        ::close(cd.videoIn.fd);
    return -1;

#endif 

}

#ifdef IO_READ
void SystemContentWidget::readInit(unsigned int buffer_size)
{
	buffers = (buffer *)calloc(1, sizeof(*buffers));

	if (!buffers) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}

	buffers[0].length = buffer_size;
	buffers[0].start = malloc(buffer_size);

	if (!buffers[0].start) {
		fprintf (stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}
}
#endif

/**
	Print error message and terminate programm with EXIT_FAILURE return code.

	\param s error message to print
*/
void SystemContentWidget::errno_exit(const char* s)
{
	fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
	exit(EXIT_FAILURE);
}

#ifdef IO_USERPTR
void SystemContentWidget::userptrInit(unsigned int buffer_size)
{
	struct v4l2_requestbuffers req;
	unsigned int page_size;

	page_size = getpagesize();
	buffer_size = (buffer_size + page_size - 1) & ~(page_size - 1);

	CLEAR(req);

	req.count = VIDIOC_REQBUFS_COUNT;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_USERPTR;

	if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
		if (EINVAL == errno) {
			fprintf(stderr, "%s does not support user pointer i/o\n", "/dev/video1");
			exit(EXIT_FAILURE);
		} else {
			errno_exit("VIDIOC_REQBUFS");
		}
	}

	buffers = (buffer *)calloc(4, sizeof(*buffers));

	if (!buffers) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}

	for (n_buffers = 0; n_buffers < 4; ++n_buffers) {
		buffers[n_buffers].length = buffer_size;
		buffers[n_buffers].start = memalign(/* boundary */ page_size, buffer_size);

		if (!buffers[n_buffers].start) {
			fprintf(stderr, "Out of memory\n");
			exit(EXIT_FAILURE);
		}
	}
}
#endif

#ifdef IO_MMAP
void SystemContentWidget::mmapInit(void)
{
	struct v4l2_requestbuffers req;
		QTextStream out(stdout);

	CLEAR(req);


	
	out << "The current cnt is 9999999999999999999999999"  << endl;

	req.count = VIDIOC_REQBUFS_COUNT;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;

	if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
		if (EINVAL == errno) {
			fprintf(stderr, "%s does not support memory mapping\n", "/dev/video1");
			exit(EXIT_FAILURE);
		} else {
			errno_exit("VIDIOC_REQBUFS");
		}
	}

	out << "The current cnt is 1616161616161616161616116"  << endl;

	

	if (req.count < 2) {
		fprintf(stderr, "Insufficient buffer memory on %s\n", "/dev/video1");
		exit(EXIT_FAILURE);
	}

	buffers = (buffer *)calloc(req.count, sizeof(*buffers));

	if (!buffers) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}

	for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
		struct v4l2_buffer buf;

		CLEAR(buf);

		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = n_buffers;

		if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
			errno_exit("VIDIOC_QUERYBUF");

		buffers[n_buffers].length = buf.length;
		buffers[n_buffers].start = mmap(NULL /* start anywhere */, buf.length, PROT_READ | PROT_WRITE /* required */, MAP_SHARED /* recommended */, fd, buf.m.offset);

		if (MAP_FAILED == buffers[n_buffers].start)
			errno_exit("mmap");
	}
}
#endif
/**
  start capturing
*/
void SystemContentWidget::captureStart(void)
{
	unsigned int i;
	enum v4l2_buf_type type;

	switch (io) {
#ifdef IO_READ
		case IO_METHOD_READ:
			/* Nothing to do. */
			break;
#endif

#ifdef IO_MMAP
		case IO_METHOD_MMAP:
			for (i = 0; i < n_buffers; ++i) {
				struct v4l2_buffer buf;

				CLEAR(buf);

				buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
				buf.memory = V4L2_MEMORY_MMAP;
				buf.index = i;

				if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
					errno_exit("VIDIOC_QBUF");
				}

			type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

			if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
				errno_exit("VIDIOC_STREAMON");

			break;
#endif

#ifdef IO_USERPTR
		case IO_METHOD_USERPTR:
			for (i = 0; i < n_buffers; ++i) {
				struct v4l2_buffer buf;

			CLEAR (buf);

			buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			buf.memory = V4L2_MEMORY_USERPTR;
			buf.index = i;
			buf.m.userptr = (unsigned long) buffers[i].start;
			buf.length = buffers[i].length;

			if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
				errno_exit("VIDIOC_QBUF");
			}

			type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

			if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
				errno_exit("VIDIOC_STREAMON");

			break;
#endif
	}
}
void SystemContentWidget::deviceUninit(void)
{
	unsigned int i;

	switch (io) {
#ifdef IO_READ
		case IO_METHOD_READ:
			free(buffers[0].start);
			break;
#endif

#ifdef IO_MMAP
		case IO_METHOD_MMAP:
			for (i = 0; i < n_buffers; ++i)
				if (-1 == munmap(buffers[i].start, buffers[i].length))
					errno_exit("munmap");
			break;
#endif

#ifdef IO_USERPTR
		case IO_METHOD_USERPTR:
			for (i = 0; i < n_buffers; ++i)
				free(buffers[i].start);
			break;
#endif
	}

	free(buffers);
}

void SystemContentWidget::captureStop(void)
{
	enum v4l2_buf_type type;

	switch (io) {
#ifdef IO_READ
		case IO_METHOD_READ:
			/* Nothing to do. */
			break;
#endif

#ifdef IO_MMAP
		case IO_METHOD_MMAP:
#endif
#ifdef IO_USERPTR
		case IO_METHOD_USERPTR:
#endif
#if defined(IO_MMAP) || defined(IO_USERPTR)
			type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

			if (-1 == xioctl(fd, VIDIOC_STREAMOFF, &type))
			errno_exit("VIDIOC_STREAMOFF");

			break;
#endif
	}
}

int SystemContentWidget::convert_yuv_to_rgb_pixel(int y, int u, int v)    /*yuv鏍煎紡杞崲涓簉gb鏍煎紡*/
{
     unsigned int pixel32 = 0;
     unsigned char *pixel = (unsigned char *)&pixel32;
     int r, g, b;
     r = y + (1.370705 * (v-128));
     g = y - (0.698001 * (v-128)) - (0.337633 * (u-128));
     b = y + (1.732446 * (u-128));
     if(r > 255) r = 255;
     if(g > 255) g = 255;
     if(b > 255) b = 255;
     if(r < 0) r = 0;
     if(g < 0) g = 0;
     if(b < 0) b = 0;
     pixel[0] = r ;//* 220 / 256;
     pixel[1] = g ;//* 220 / 256;
     pixel[2] = b ;//* 220 / 256;
     return pixel32;
}

QImage SystemContentWidget::convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width,unsigned int height)
{
     unsigned int in, out = 0;
     unsigned int pixel_16;
     unsigned char pixel_24[3];
     unsigned int pixel32;
     int y0, u, y1, v;
//	 printf("0909090900--\n");
     for(in = 0; in < width * height * 2; in += 4)
     {
       pixel_16 =
                  yuv[in + 3] << 24 |
                  yuv[in + 2] << 16 |
                  yuv[in + 1] <<  8 |
                  yuv[in + 0];//YUV422姣忎釜鍍忕礌2瀛楄妭锛屾瘡涓や釜鍍忕礌鍏辩敤涓�涓狢r,Cb鍊硷紝鍗硊鍜寁锛孯GB24姣忎釜鍍忕礌3涓瓧鑺�
       y0 = (pixel_16 & 0x000000ff);
       u  = (pixel_16 & 0x0000ff00) >>  8;
       y1 = (pixel_16 & 0x00ff0000) >> 16;
       v  = (pixel_16 & 0xff000000) >> 24;
       pixel32 = convert_yuv_to_rgb_pixel(y0, u, v);
       pixel_24[0] = (pixel32 & 0x000000ff);
       pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
       pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
       rgb[out++] = pixel_24[0];
       rgb[out++] = pixel_24[1];
       rgb[out++] = pixel_24[2];//rgb鐨勪竴涓儚绱�
       pixel32 = convert_yuv_to_rgb_pixel(y1, u, v);
       pixel_24[0] = (pixel32 & 0x000000ff);
       pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
       pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
       rgb[out++] = pixel_24[0];
       rgb[out++] = pixel_24[1];
       rgb[out++] = pixel_24[2];
     }

    QImage image = QImage(rgb, width,height, QImage::Format_RGB888); //绠�鍗曞湴鎹竴涓嬩负Image瀵硅薄锛宺gbSwapped鏄负浜嗘樉绀烘晥鏋滆壊褰╁ソ涓�浜涖��


    return  image;

//	image.scaled(ui->label->size(), Qt::KeepAspectRatio);
//	ui->label->setScaledContents(true);
//	ui->label->setPixmap(QPixmap::fromImage(image));

}
int SystemContentWidget::v4l2_enbale()
{
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int ret;

    ret = ioctl(cd.videoIn.fd, VIDIOC_STREAMON, &type);
    if (ret < 0) {
      printf("Unable to  %s capture: %d.\n", "start", errno);
      return ret;
    }
    cd.videoIn.isstreaming = 1;
    return 0;

}

int SystemContentWidget::v4l2_disable()
{
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int ret;

    ret = ioctl(cd.videoIn.fd, VIDIOC_STREAMOFF, &type);
    if (ret < 0) {
      printf("Unable to %s capture: %d.\n", "stop", errno);
      return ret;
    }
    cd.videoIn.isstreaming = 0;
    return 0;

}
int SystemContentWidget::update_video(){

    int ret;
    if (!cd.videoIn.isstreaming) {
        if (v4l2_enbale()){
            //            goto err;
            return -1;
        }
    }

    memset(&cd.videoIn.buf, 0, sizeof(struct v4l2_buffer));
    cd.videoIn.buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    cd.videoIn.buf.memory = V4L2_MEMORY_MMAP;

    ret = ioctl(cd.videoIn.fd, VIDIOC_DQBUF, &cd.videoIn.buf);
    if (ret < 0) {
        printf("Unable to dequeue buffer (%d).\n", errno);
        return 0;
        //           goto err;
    }

    switch (cd.videoIn.formatIn) {
        case V4L2_PIX_FMT_JPEG:
        case V4L2_PIX_FMT_MJPEG:
            if (cd.videoIn.buf.bytesused <= HEADERFRAME1) {
                /* Prevent crash on empty image */
                printf("Ignoring empty buffer ...\n");
                return 0;
            }
            cd.videoIn.framesizeIn = cd.videoIn.buf.bytesused;
            memcpy(tmpbuffer, mem[cd.videoIn.buf.index], cd.videoIn.buf.bytesused);
            break;

        case V4L2_PIX_FMT_SRGGB8:
        case V4L2_PIX_FMT_YUYV:
            if(cd.videoIn.buf.bytesused > cd.videoIn.framesizeIn) {
                memcpy(framebuffer, mem[cd.videoIn.buf.index], (size_t) cd.videoIn.framesizeIn);
            } else {
                memcpy(framebuffer, mem[cd.videoIn.buf.index], (size_t) cd.videoIn.buf.bytesused);
            }
            break;

        default:
            //                goto err;
            return 0;
        break;
    }

    ret = ioctl(cd.videoIn.fd, VIDIOC_QBUF, &cd.videoIn.buf);
    if (ret < 0) {
        printf("Unable to requeue buffer (%d).\n", errno);
        //            goto err;
        return 0;
    }
    //          disaply_image();
    QImage image = convert_yuv_to_rgb_buffer(framebuffer,tmpbuffer,WIDTH,HEIGHT/*QWidget::width(),QWidget::height()*/);
    // update
    //   convert_yuv_to_rgb_buffer(framebuffer,tmpbuffer,WIDTH,HEIGHT/*QWidget::width(),QWidget::height()*/);

    image.scaled(m_show_pic->size(), Qt::KeepAspectRatio);
    m_show_pic->setScaledContents(true);
    m_show_pic->setPixmap(QPixmap::fromImage(image));
    return 0;

//    error:
  //      return -1;

}

int SystemContentWidget::save_rgb_to_jpg(char *soureceData, int imgWidth, int imgHeight, char * fileName)
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
	cinfo.in_color_space = JCS_RGB;    //or JCS_GRAYSCALE;   colorspace of input image 
 
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, JPEG_QUALITY, TRUE ); /* limit to baseline-JPEG values */
 
	jpeg_start_compress(&cinfo, TRUE);
 
	row_stride = imgWidth; /* JSAMPLEs per row in image_buffer */
 
	while (cinfo.next_scanline < cinfo.image_height) 
	{
		row_pointer[0] = (JSAMPROW)&soureceData[cinfo.next_scanline * row_stride*depth];
		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}
 
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);	
	fclose(outfile);
 
	return 0;
}

void SystemContentWidget::timerDone(){

    update_video();
}

int SystemContentWidget::frameRead(void)
{
	QTextStream out(stdout);

	struct v4l2_buffer buf;
#ifdef IO_USERPTR
	unsigned int i;
#endif

	switch (io) {
#ifdef IO_READ
		case IO_METHOD_READ:
			if (-1 == read(fd, buffers[0].start, buffers[0].length)) {
				switch (errno) {
					case EAGAIN:
						return 0;

					case EIO:
						// Could ignore EIO, see spec.
						// fall through

					default:
						errno_exit("read");
				}
			}

			struct timespec ts;
			struct timeval timestamp;
			clock_gettime(CLOCK_MONOTONIC,&ts);
			timestamp.tv_sec = ts.tv_sec;
			timestamp.tv_usec = ts.tv_nsec/1000;

			//imageProcess(buffers[0].start,timestamp);
			break;
#endif

#ifdef IO_MMAP
		case IO_METHOD_MMAP:
			CLEAR(buf);

			buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			buf.memory = V4L2_MEMORY_MMAP;

			if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
				switch (errno) {
					case EAGAIN:
						return 0;

					case EIO:
						// Could ignore EIO, see spec
						// fall through

					default:
						errno_exit("VIDIOC_DQBUF");
				}
			}

			assert(buf.index < n_buffers);

			out << "The current cnt is 888888888888888888"  << endl;

			//imageProcess(buffers[buf.index].start,buf.timestamp);
			save_rgb_to_jpg((char *)buffers[buf.index].start,800,600,"/home/root/new.jpg");

			if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
				errno_exit("VIDIOC_QBUF");

			break;
#endif

#ifdef IO_USERPTR
			case IO_METHOD_USERPTR:
				CLEAR (buf);

				buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
				buf.memory = V4L2_MEMORY_USERPTR;

				if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
					switch (errno) {
						case EAGAIN:
							return 0;

						case EIO:
							// Could ignore EIO, see spec.
							// fall through

						default:
							errno_exit("VIDIOC_DQBUF");
					}
				}

				for (i = 0; i < n_buffers; ++i)
					if (buf.m.userptr == (unsigned long)buffers[i].start && buf.length == buffers[i].length)
						break;

				assert (i < n_buffers);

				//imageProcess((void *)buf.m.userptr,buf.timestamp);

				if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
					errno_exit("VIDIOC_QBUF");
				break;
#endif
	}

	return 1;
}

void SystemContentWidget::initUI()
{
//    mainLayout = new QGridLayout();

//    m_Grbox_Group = new QGroupBox("Camera preview");
//    m_Grbox_Layout = new QGridLayout(m_Grbox_Group);

//    m_show_pic = new QLabel(m_Grbox_Group);
//    m_show_pic->setText(tr("camera show "));

//    QHBoxLayout *hLayout1 = new QHBoxLayout(m_Grbox_Group);
////    hLayout1->setContentsMargins(40,10,40,10);
////    hLayout1->setSpacing(120);
//    hLayout1->addWidget(m_show_pic);

//    m_Grbox_Layout->addLayout(hLayout1,0,0,1,10);
//    m_Grbox_Group->setLayout(m_Grbox_Layout);
//    mainLayout->addWidget(m_Grbox_Group, 0,0);
//    this->setLayout(mainLayout);

//    /*init*/
//    init_v4l2();

//    /*init camera  and time */
//    testTimer = new QTimer(this);
//    connect( testTimer, SIGNAL(timeout()),this, SLOT(timerDone()) );
//    testTimer->start( 100 );


    mainLayout = new QGridLayout();
    m_Grbox_Group = new QGroupBox("Camera");
    m_Grbox_Layout = new QGridLayout(m_Grbox_Group);

    m_insert_Button = new QPushButton();
    m_insert_Button->setObjectName("serialButton");
    m_insert_Button->setText(tr("Photograph"));
//     m_insert_Button->setMinimumSize(10,10);

    m_show_pic = new QLabel(m_Grbox_Group);
    m_show_pic->setText(tr("photo show "));

    m_delete_Button = new QPushButton();
//    m_delete_Button->setObjectName("sqlite3_delet");
     m_delete_Button->setObjectName("serialButton");
    m_delete_Button->setText(tr("Save"));

    QHBoxLayout *hLayout1 = new QHBoxLayout(m_Grbox_Group);
//    hLayout1->setContentsMargins(5,5,5,5);
//    hLayout1->setMargin(40);
    hLayout1->setSpacing(120);
    hLayout1->addStretch();
    hLayout1->addWidget(m_insert_Button);
    hLayout1->addStretch();
    hLayout1->addWidget(m_delete_Button);
    // hLayout1->addWidget(m_update_Button);
    hLayout1->addStretch();

    QHBoxLayout *hLayout2 = new QHBoxLayout(m_Grbox_Group);
    hLayout2->addWidget(m_show_pic);

//     QGridLayout *grid = new QGridLayout();
    m_Grbox_Layout->addLayout(hLayout1,0,0,1,10);
    m_Grbox_Layout->addLayout(hLayout2,1,0,1,10);
    m_Grbox_Group->setLayout(m_Grbox_Layout);
    mainLayout->addWidget(m_Grbox_Group, 0,0);
    this->setLayout(mainLayout);

    connect(m_insert_Button, SIGNAL(clicked()), this, SLOT(clickInsertData_sqlite()));
//    connect(m_delete_Button, SIGNAL(clicked()), this, SLOT(clickDeleteData_sqlite()));



	/*测试*/
	init_v4l2();
	captureStart();
	frameRead();

	captureStop();
	deviceUninit();
    ::close(fd);

}
int SystemContentWidget::clickInsertData_sqlite()
{

QImage image;
image.load("/home/root/HH.jpg");
image.scaled(m_show_pic->size(), Qt::KeepAspectRatio);
m_show_pic->setScaledContents(true);
m_show_pic->setPixmap(QPixmap::fromImage(image));

}
void SystemContentWidget::initConnection()
{

}

void SystemContentWidget::display()
{

}
void SystemContentWidget::setApplication(MxApplication *app)
{
    m_app = app;
}

//void SystemContentWidget::setDbusProxy(MxDE *mxde)
//{
//    m_mxde = mxde;
//}

void SystemContentWidget::setParentWindow(QWidget *parent)
{
    m_parent = parent;
}

