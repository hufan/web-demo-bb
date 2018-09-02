#include "mainwindow.h"
#include "ui_mainwindow.h"


//#include "highgui.h"
//#include "objdetect/objdetect.hpp"
//#include "imgproc/imgproc.hpp"
 

using namespace std;
using namespace cv;


cv::Mat QImage_to_cvMat( const QImage &image, bool inCloneImageData = true ) {
  switch ( image.format() )
  {
     // 8-bit, 4 channel
     case QImage::Format_RGB32:
     {
        cv::Mat mat( image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine() );
        return (inCloneImageData ? mat.clone() : mat);
     }

     // 8-bit, 3 channel
     case QImage::Format_RGB888:
     {
        if ( !inCloneImageData ) {
           qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning since we use a temporary QImage";
        }
        QImage swapped = image.rgbSwapped();
        return cv::Mat( swapped.height(), swapped.width(), CV_8UC3, const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine() ).clone();
     }

     // 8-bit, 1 channel
     case QImage::Format_Indexed8:
     {
        cv::Mat  mat( image.height(), image.width(), CV_8UC1, const_cast<uchar*>(image.bits()), image.bytesPerLine() );

        return (inCloneImageData ? mat.clone() : mat);
     }

     default:
//        qDebug("Image format is not supported: depth=%d and %d format\n", image.depth(), image.format(););
        break;
  }

  return cv::Mat();
}

QImage  Mat2QImage(cv::Mat cvImg)
{
    QImage qImg;
//	 channels color image
	 	
    if(cvImg.channels()==3)                             //3
    {

        cv::cvtColor(cvImg,cvImg,CV_BGR2RGB);
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols, cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
    }
    else if(cvImg.channels()==1)                    //grayscale image
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_Indexed8);
    }
    else
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
    }

    return qImg;

}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


//	int ret,i;

	init_v4l2();

	init_Classifier();


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
       QImage image = convert_yuv_to_rgb_buffer(framebuffer,tmpbuffer,WIDTH,HEIGHT/*QWidget::width(),QWidget::height()*/);

	//	QImage image = QImage(rgb, width,height, QImage::Format_RGB888); //ç®€å•åœ°æ¢ä¸€ä¸‹ä¸ºImageå¯¹è±¡ï¼ŒrgbSwappedæ˜¯ä¸ºäº†æ˜¾ç¤ºæ•ˆæžœè‰²å½©å¥½ä¸€äº›ã€‚
#if 0
		image.scaled(ui->label->size(), Qt::KeepAspectRatio);
		ui->label->setScaledContents(true);
		ui->label->setPixmap(QPixmap::fromImage(image));
#endif
		

	cv::Mat image_tmp1=QImage_to_cvMat(image,true);
	cv::Mat image_tmp2= face_detect(image_tmp1);
	QImage image_show=Mat2QImage(image_tmp2);

	image_show.scaled(ui->label->size(), Qt::KeepAspectRatio);
	ui->label->setScaledContents(true);
	ui->label->setPixmap(QPixmap::fromImage(image_show));

	return 0;
}

int MainWindow::convert_yuv_to_rgb_pixel(int y, int u, int v)    /*yuvæ ¼å¼è½¬æ¢ä¸ºrgbæ ¼å¼*/
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

QImage MainWindow::convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width,unsigned int height)
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
                  yuv[in + 0];//YUV422æ¯ä¸ªåƒç´ 2å­—èŠ‚ï¼Œæ¯ä¸¤ä¸ªåƒç´ å…±ç”¨ä¸€ä¸ªCr,Cbå€¼ï¼Œå³uå’Œvï¼ŒRGB24æ¯ä¸ªåƒç´ 3ä¸ªå­—èŠ‚
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
       rgb[out++] = pixel_24[2];//rgbçš„ä¸€ä¸ªåƒç´ 
       pixel32 = convert_yuv_to_rgb_pixel(y1, u, v);
       pixel_24[0] = (pixel32 & 0x000000ff);
       pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
       pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
       rgb[out++] = pixel_24[0];
       rgb[out++] = pixel_24[1];
       rgb[out++] = pixel_24[2];
     }
	 
	QImage image = QImage(rgb, width,height, QImage::Format_RGB888); //ç®€å•åœ°æ¢ä¸€ä¸‹ä¸ºImageå¯¹è±¡ï¼ŒrgbSwappedæ˜¯ä¸ºäº†æ˜¾ç¤ºæ•ˆæžœè‰²å½©å¥½ä¸€äº›ã€‚


    return  image;
	
//	image.scaled(ui->label->size(), Qt::KeepAspectRatio);
//	ui->label->setScaledContents(true);
//	ui->label->setPixmap(QPixmap::fromImage(image));

}


int MainWindow::init_Classifier()
{	
	if (!eye_Classifier.load("/home/root/haarcascade_eye.xml"))  
	{
		cout << "Load haarcascade_eye.xml failed!" << endl;
		return -1;
	}
	
	if (!face_cascade.load("/home/root/haarcascade_frontalface_alt.xml"))
	{
		cout << "Load haarcascade_frontalface_alt failed!" << endl;
		return -1;
	}
	return 0;
}

cv::Mat  MainWindow::face_detect(cv::Mat image)
{

#ifdef DETECT_EYS
	vector<Rect> eyeRect;		
#endif

#ifdef DETECT_FACE
	vector<Rect> faceRect;
#endif

//	Mat image_gray;
//	Mat mat;	


	cv::Mat  image_gray;      //¶¨ÒåÁ½¸öMat±äÁ¿£¬ÓÃÓÚ´æ´¢Ã¿Ò»Ö¡µÄÍ¼Ïñ

//	image = imread("F://1.png");

	cvtColor(image, image_gray, CV_BGR2GRAY);//×ªÎª»Ò¶ÈÍ¼
	equalizeHist(image_gray, image_gray);//Ö±·½Í¼¾ùºâ»¯£¬Ôö¼Ó¶Ô±È¶È·½±ã´¦Àí

//	cvtColor(&mat, image_gray, CV_BGR2GRAY);  
//	equalizeHist(image_gray, image_gray);



#ifdef DETECT_EYS
       //¼ì²â¹ØÓÚÑÛ¾¦²¿Î»Î»ÖÃ
       eye_Classifier.detectMultiScale(image_gray, eyeRect, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
       for (size_t eyeIdx = 0; eyeIdx < eyeRect.size(); eyeIdx++)
       {
           rectangle(image, eyeRect[eyeIdx], Scalar(0, 0, 255));   //ÓÃ¾ØÐÎ»­³ö¼ì²âµ½µÄÎ»ÖÃ
       }
#endif


#ifdef DETECT_FACE
       //¼ì²â¹ØÓÚÁ³²¿Î»ÖÃ
       face_cascade.detectMultiScale(image_gray, faceRect, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
       for (size_t i = 0; i < faceRect.size(); i++)
       {
           rectangle(image, faceRect[i], Scalar(0, 0, 255));      //ÓÃ¾ØÐÎ»­³ö¼ì²âµ½µÄÎ»ÖÃ
       }
#endif

	return image;

}



//cv::Mat QImage2cvMat(QImage image)
//{
//    cv::Mat mat;
//    qDebug() << image.format();
//    switch(image.format())
//    {
//    case QImage::Format_ARGB32:
//    case QImage::Format_RGB32:
//    case QImage::Format_ARGB32_Premultiplied:
//        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
//        break;
//    case QImage::Format_RGB888:
//        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
//        cv::cvtColor(mat, mat, CV_BGR2RGB);
//        break;
//    case QImage::Format_Indexed8:
//        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
//        break;
//    }
//    return mat;
//}

//Mat QImage2cvMat(QImage image)
//{
//    cv::Mat mat;
//    switch(image.format())
//    {
//    case QImage::Format_ARGB32:
//    case QImage::Format_RGB32:
//    case QImage::Format_ARGB32_Premultiplied:
//        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.bits(), image.bytesPerLine());
//        break;
//    case QImage::Format_RGB888:
//        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.bits(), image.bytesPerLine());
//        cv::cvtColor(mat, mat, CV_BGR2RGB);
//        break;
//    case QImage::Format_Indexed8:
//        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.bits(), image.bytesPerLine());
//        break;
//    }
//    return mat;
//}
