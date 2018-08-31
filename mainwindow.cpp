#include "mainwindow.h"
#include "ui_mainwindow.h"


#define CAMERA_DEVICE "/dev/video1"

#define  WIDTH    800
#define  HEIGHT   600


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


//	int ret,i;

	
	init_v4l2();


	time1 = new QTimer(this);
	connect(time1, SIGNAL(timeout()), this, SLOT(update_video()));
	time1->start(100);

}

MainWindow::~MainWindow()
{
    v4l2_disable();
    ::close(cd.videoIn.fd);
    delete ui;
}

int MainWindow::init_v4l2()
{
	int i,ret;

	/*init v4l2*/

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
				 exit(1);
			   }
		
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
		  break;
		default:
		  printf("Unknown format: should never arrive exit fatal !!\n");
		  goto error;
		  break;
		}


error:
//    	free(framebuffer);
//	free(tmpbuffer);
	
//	::close(cd.videoIn.fd);
	return -1;
}


int MainWindow::v4l2_enbale()
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

int MainWindow::v4l2_disable()
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

int MainWindow::update_video()
{
	int ret;

	if (!cd.videoIn.isstreaming) {
	  if (v4l2_enbale()){
		goto err;
	   }
	}
	
	  memset(&cd.videoIn.buf, 0, sizeof(struct v4l2_buffer));
	  cd.videoIn.buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	  cd.videoIn.buf.memory = V4L2_MEMORY_MMAP;
	
	  ret = ioctl(cd.videoIn.fd, VIDIOC_DQBUF, &cd.videoIn.buf);
	  if (ret < 0) {
		printf("Unable to dequeue buffer (%d).\n", errno);
		goto err;
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
	//		  memcpy(cd.videoIn.tmpbuffer, cd.videoIn.mem[cd.videoIn.buf.index], cd.videoIn.buf.bytesused);
			memcpy(tmpbuffer, mem[cd.videoIn.buf.index], cd.videoIn.buf.bytesused);
		break;
	
		case V4L2_PIX_FMT_SRGGB8:
		case V4L2_PIX_FMT_YUYV:
			if(cd.videoIn.buf.bytesused > cd.videoIn.framesizeIn) {
	//			  memcpy(cd.videoIn.framebuffer, cd.videoIn.mem[cd.videoIn.buf.index], (size_t) cd.videoIn.framesizeIn);
				memcpy(framebuffer, mem[cd.videoIn.buf.index], (size_t) cd.videoIn.framesizeIn);
					
			} else {
	//			  memcpy(cd.videoIn.framebuffer, cd.videoIn.mem[cd.videoIn.buf.index], (size_t) cd.videoIn.buf.bytesused);
				 memcpy(framebuffer, mem[cd.videoIn.buf.index], (size_t) cd.videoIn.buf.bytesused);	
			}
			
		break;
	
		default:
			goto err;
		break;
	  }
	
	  ret = ioctl(cd.videoIn.fd, VIDIOC_QBUF, &cd.videoIn.buf);
	  if (ret < 0) {
		printf("Unable to requeue buffer (%d).\n", errno);
		goto err;
	  }

	  disaply_image();

// update
//   convert_yuv_to_rgb_buffer(framebuffer,tmpbuffer,WIDTH,HEIGHT/*QWidget::width(),QWidget::height()*/);


err:
	return -1;
}

int MainWindow::disaply_image()
{
	// update
       convert_yuv_to_rgb_buffer(framebuffer,tmpbuffer,WIDTH,HEIGHT/*QWidget::width(),QWidget::height()*/);
	
	return 0;
}

int MainWindow::convert_yuv_to_rgb_pixel(int y, int u, int v)    /*yuv格式转换为rgb格式*/
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

int MainWindow::convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width,unsigned int height)
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
                  yuv[in + 0];//YUV422每个像素2字节，每两个像素共用一个Cr,Cb值，即u和v，RGB24每个像素3个字节
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
       rgb[out++] = pixel_24[2];//rgb的一个像素
       pixel32 = convert_yuv_to_rgb_pixel(y1, u, v);
       pixel_24[0] = (pixel32 & 0x000000ff);
       pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
       pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
       rgb[out++] = pixel_24[0];
       rgb[out++] = pixel_24[1];
       rgb[out++] = pixel_24[2];
     }
	QImage image = QImage(rgb, width,height, QImage::Format_RGB888); //简单地换一下为Image对象，rgbSwapped是为了显示效果色彩好一些。
	image.scaled(ui->label->size(), Qt::KeepAspectRatio);
	ui->label->setScaledContents(true);
	ui->label->setPixmap(QPixmap::fromImage(image));
     return 0;
}




