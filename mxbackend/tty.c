/******************************************************************************
* Copyright (C), 2016-2017, Sunny.Guo
* FileName: tty.c 
* Author: Sunny.Guo
* Version: 1.0
* Date: 2017年 01月 19日 星期四 10:12:50 CST
* Description: tty operation functions
*
* History:        
*  <author>  	<time>   	<version >   	<desc>
*  Sunny.Guo   	19/01/2017      1.0     	create this moudle  
*                                                                          
* Licensed under GPLv2 or later, see file LICENSE in this source tree.
*******************************************************************************/
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <net/if.h>
#include <cJSON.h>
#include "common.h"
#include "tty.h"

#define MAX_OPENED_TTY 5

int thread_tty_fd = 0;
int current_birate =0;
extern  char *board_cfg;

struct g_opened_tty opened_tty[MAX_OPENED_TTY];
int opened_num = 0;

pthread_t tty_thread_id[MAX_OPENED_TTY] = {0};
void *(*tty_thread[MAX_OPENED_TTY])(void *args);


int	tty_open( char * tty,struct g_opened_tty *opeded_tty_data)
{
    int fd = 0;
    int i = 0,j= 0;

    for(i = 0 ;i < MAX_OPENED_TTY;i++ )
    {
        if(opened_tty[i].device_name != NULL)
        {
            if(strcmp(opened_tty[i].device_name,tty) == 0)
            {
                break;
            }
        }
    }

    if(i == MAX_OPENED_TTY)
    {
        for(j = 0; j < MAX_OPENED_TTY ; j++)
        {
            if(opened_tty[j].device_name == NULL)
                break;
        }
        opened_tty[j].device_name = tty;
        fd = open(tty, O_RDWR);
        if(fd < 0){
            dbg_perror("open tty");
            }
        opened_tty[j].fd = fd;
        opened_tty[j].open_cnt++;
        return fd;
    }

    if(i != MAX_OPENED_TTY)
    {
        printf("tty opened!\n");
        opened_tty[i].open_cnt++;
        memcpy(opeded_tty_data,&opened_tty[i],sizeof(struct g_opened_tty));

        return fd;
    }

}
void tty_close(int fd)
{
    int i = 0;
    for(i = 0; i < MAX_OPENED_TTY;i++)
    {
        if((opened_tty[i].fd == fd) && (opened_tty[i].fd != 0))
        {

            printf("opened_tty[%d].open_cnt %d\n",i,opened_tty[i].open_cnt);
            if(opened_tty[i].open_cnt <= 0)
            {
                memset(&opened_tty[i], 0, sizeof(struct g_opened_tty));
                close(fd);
                thread_tty_fd = 0;
                current_birate = 0;
            }
        }
    }

}
/******************************************************************************
  Function:       tty_setting
  Description:    set the tty device's mode and bitrate
  Input:          fd       --  tty device fd
  				  bitrate --  tty baudrate
  				  mode	   --  tty mode, 1: rs485; 0: rs232
  				  flow	   --  cts/rts flow control
  				  par	   --  odd/even 
  				  stop	   --  number of stop bits
                  
  Output:          
  Return:         int 	   --  tty setting status 0:success 
  Others:         NONE
*******************************************************************************/
int 			tty_setting(int fd, int bitrate, int datasize, int mode, int flow, int par, int stop)
{
    int i = 0;
	struct termios newtio;
    dbg_printf("%d %d %d %d %d %d %d\n",fd,bitrate,datasize,mode,flow,par,stop);
    for(i = 0; i < MAX_OPENED_TTY; i++ )
    {
        if(opened_tty[i].fd == fd)
        {
            opened_tty[i].bitrate = bitrate;
            opened_tty[i].datasize = datasize;
            opened_tty[i].mode = mode;
            opened_tty[i].flow = flow;
            opened_tty[i].par = par;
            opened_tty[i].stop = stop;
        }

    }

    current_birate = bitrate;
	/* ignore modem control lines and enable receiver */
	memset(&newtio, 0, sizeof(newtio));
	newtio.c_cflag = newtio.c_cflag |= CLOCAL | CREAD;
	if(flow == 1)
	{
		newtio.c_cflag = newtio.c_cflag |= CLOCAL | CREAD | CRTSCTS;
	}
	
	newtio.c_cflag &= ~CSIZE;

	/* set character size */
	switch (datasize) {
	case 8:
		newtio.c_cflag |= CS8;
		break;
	case 7:
		newtio.c_cflag |= CS7;
		break;
	case 6:
		newtio.c_cflag |= CS6;
		break;
	case 5:
		newtio.c_cflag |= CS5;
		break;
	default:
		newtio.c_cflag |= CS8;
		break;
	}
	
	/* set the parity */
	switch (par) {
	case 'o':
	case 'O':
		newtio.c_cflag |= PARENB;
		newtio.c_cflag |= PARODD;
		newtio.c_iflag |= (INPCK | ISTRIP);
		break;
	case 'e':
	case 'E':
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
		newtio.c_iflag |= (INPCK | ISTRIP);
		break;
	case 'n':
	case 'N':
		newtio.c_cflag &= ~PARENB;
		break;
	default:
		newtio.c_cflag &= ~PARENB;
		break;
	}
	
	/* set the stop bits */
	switch (stop) {
	case 1:
		newtio.c_cflag &= ~CSTOPB;
		break;
	case 2:
		newtio.c_cflag |= CSTOPB;
		break;
	default:
		newtio.c_cflag &= ~CSTOPB;
		break;
	}
	
	/* set output and input baud rate */
	switch (bitrate) {
	case 0:
		cfsetospeed(&newtio, B0);
		cfsetispeed(&newtio, B0);
		break;
	case 50:
		cfsetospeed(&newtio, B50);
		cfsetispeed(&newtio, B50);
		break;
	case 75:
		cfsetospeed(&newtio, B75);
		cfsetispeed(&newtio, B75);
		break;
	case 110:
		cfsetospeed(&newtio, B110);
		cfsetispeed(&newtio, B110);
		break;
	case 134:
		cfsetospeed(&newtio, B134);
		cfsetispeed(&newtio, B134);
		break;
	case 150:
		cfsetospeed(&newtio, B150);
		cfsetispeed(&newtio, B150);
		break;
	case 200:
		cfsetospeed(&newtio, B200);
		cfsetispeed(&newtio, B200);
		break;
	case 300:
		cfsetospeed(&newtio, B300);
		cfsetispeed(&newtio, B300);
		break;
	case 600:
		cfsetospeed(&newtio, B600);
		cfsetispeed(&newtio, B600);
		break;
	case 1200:
		cfsetospeed(&newtio, B1200);
		cfsetispeed(&newtio, B1200);
		break;
	case 1800:
		cfsetospeed(&newtio, B1800);
		cfsetispeed(&newtio, B1800);
		break;
	case 2400:
		cfsetospeed(&newtio, B2400);
		cfsetispeed(&newtio, B2400);
		break;
	case 4800:
		cfsetospeed(&newtio, B4800);
		cfsetispeed(&newtio, B4800);
		break;
	case 9600:
		cfsetospeed(&newtio, B9600);
		cfsetispeed(&newtio, B9600);
		break;
	case 19200:
		cfsetospeed(&newtio, B19200);
		cfsetispeed(&newtio, B19200);
		break;
	case 38400:
		cfsetospeed(&newtio, B38400);
		cfsetispeed(&newtio, B38400);
		break;
	case 57600:
		cfsetospeed(&newtio, B57600);
		cfsetispeed(&newtio, B57600);
		break;
	case 115200:
		cfsetospeed(&newtio, B115200);
		cfsetispeed(&newtio, B115200);
		break;
	case 230400:
		cfsetospeed(&newtio, B230400);
		cfsetispeed(&newtio, B230400);
		break;
	default:
		cfsetospeed(&newtio, B9600);
		cfsetispeed(&newtio, B9600);
		break;
	}
	
	/* set timeout in deciseconds for non-canonical read */
	newtio.c_cc[VTIME] = 0;
	/* set minimum number of characters for non-canonical read */
	newtio.c_cc[VMIN] = 1;

	/* flushes data received but not read */
	tcflush(fd, TCIFLUSH);
	/* set the parameters associated with the terminal from
	   the termios structure and the change occurs immediately */
	if((tcsetattr(fd, TCSANOW, &newtio))!=0) {
		dbg_perror("set_tty/tcsetattr");
		return -1;
	}

	tty_mode(fd, mode);
	return 0;

}

int				tty_read(int fd, char *frame)
{
	struct timeval tv;
	fd_set fds;
	int ret;

	unsigned int timeout = TTY_READ_TIMEOUT_USEC;
	tv.tv_sec = 0;
	tv.tv_usec = timeout ;

	FD_ZERO(&fds);
	FD_SET(fd, &fds);

	if(timeout > 0){
		ret = select(fd+1, &fds, NULL, NULL, &tv);
		if(ret < 0){
			dbg_perror("select tty");
			}
		else if(ret == 0){
			// select timeout
			// dbg_printf("select %d timeout\r\n", fd);
			}
		}
	if(FD_ISSET(fd, &fds)){
		ret = read(fd, frame, TTY_READ_BUFFER_SIZE);
		}
	
	return ret;
	
}
void set_imx6ul_rs485(int fd,int len,int statu)
{
    int f_fd = 0;
    char buf[100] ={0};

    f_fd = open(DEVICE_TREE_CAMPATIABLE,O_RDONLY);
    if(f_fd < 0)
    {
        printf("open %s error!\n",DEVICE_TREE_CAMPATIABLE);
    }

    read(f_fd, buf ,sizeof(buf));
    if(strstr(buf, DEVICE_IMX6))
    {
        struct serial_rs485 rs485conf;
        int res,rs485_wait_time=0;

        usleep(100000);
        printf("imx6ull rs485\n");
        /* Get configure from device */
        res = ioctl(fd, TIOCGRS485, &rs485conf);
        if (res < 0) {
                perror("Ioctl error on getting 485 configure:");
                close(fd);
                return res;
        }

        if(statu == 1)
        {
            rs485conf.flags |= SER_RS485_RTS_AFTER_SEND;

        }
        else if(statu == 0)
        {
            rs485conf.flags &= ~SER_RS485_RTS_AFTER_SEND;
            rs485_wait_time = 10*len/current_birate*1000;
            rs485conf.delay_rts_after_send = rs485_wait_time;
        }
        else;
        /* Set configure to device */
        res = ioctl(fd, TIOCSRS485, &rs485conf);
        if (res < 0) {
                perror("Ioctl error on setting 485 configure:");
                close(fd);
        }

    }
    close(f_fd);

}
int				tty_write(int fd, char *frame, int len)
{
    int ret = -1;

    set_imx6ul_rs485(fd,len,0);

	ret = write(fd, frame, len);
	if(ret < 0){
		dbg_perror("tty write!");
    }

    set_imx6ul_rs485(fd,len,1);

	return ret;
}

int 			tty_mode(const int fd,  int mode)
{
	struct serial_rs485 rs485conf;
    int res;
	/* Get configure from device */

	res = ioctl(fd, TIOCGRS485, &rs485conf);
    if (res < 0)
    {
		perror("Ioctl error on getting 485 configure:");
		close(fd);
		return res;
	}

    /* Set enable/disable to configure */
    if(mode == TTY_RS485_MODE)
        // Enable rs485 mode
        rs485conf.flags |= SER_RS485_ENABLED;
    else
        // Disable rs485 mode
        rs485conf.flags &= ~(SER_RS485_ENABLED);


    rs485conf.delay_rts_before_send = 0x00000004;

	/* Set configure to device */
	res = ioctl(fd, TIOCSRS485, &rs485conf);
	if (res < 0) {
		perror("Ioctl error on setting 485 configure:");
		close(fd);
	}

	return res;
}

void get_rs485_list(char * result)
{

    get_cfg_list("rs485",result);

}

void get_serial_list(char * result)
{

    get_cfg_list("rs232",result);
}


void * tty_read_thread0(void *arg)
{
    struct timeval tv;
    DBusMessage* msg;
    DBusMessageIter args;
    fd_set fds;
    int ret=0,len=0;
    char buf[50] = {0};
    char *respone = NULL;
    int fd = (int )arg;
    printf("tty_read_thread0 fd:%d\n",fd);
    while(1)
    {
        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        tv.tv_sec = 0;
        tv.tv_usec = 100000 ;

        ret = select(fd+1, &fds, NULL, NULL, &tv);
        if(ret < 0){
                perror("select error\n");
        }
        else
        {

            if(ret && FD_ISSET(fd, &fds))
            {
                memset(buf, 0, sizeof(buf));
                len = read(fd, buf, sizeof(buf));
                if(len > 0)
                {
                    respone = &buf[0];
                    if(IsUtf8Format(buf))
                    {

                        msg = dbus_message_new_signal(DBUS_SERVER_PATH,
                                                      DBUS_SERVER_INTERFACE,
                                                      "sigSerialRecv");
                        dbus_message_iter_init_append(msg, &args);
                        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &fd)) {
                            fprintf(stderr, "Out Of Memory!\n");
                            return;
                        }
                        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &respone)) {
                            fprintf(stderr, "Out Of Memory!\n");
                            return;
                        }
                        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &len)) {
                            fprintf(stderr, "Out Of Memory!\n");
                            return;
                        }
                        if (!dbus_connection_send(dbus_server_conn, msg, NULL)) {
                            fprintf(stderr, "Out Of Memory!\n");
                            return;
                        }

                        dbus_connection_flush(dbus_server_conn);
                        dbus_message_unref(msg);
                    }
                    else
                    {
                        printf("str is not utf-8!\n");
                    }


                }
            }

        }
    }
    return NULL;
}
void * tty_read_thread1(void *arg)
{
    struct timeval tv;
    DBusMessage* msg;
    DBusMessageIter args;
    fd_set fds;
    int ret=0,len=0;
    char buf[50] = {0};
    char *respone = NULL;
    int fd = (int) arg;
    printf("tty_read_thread1 fd:%d\n",fd);
    while(1)
    {
        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        tv.tv_sec = 0;
        tv.tv_usec = 100000 ;

        ret = select(fd+1, &fds, NULL, NULL, &tv);

        if(ret < 0){
                perror("select error\n");
        }
        else
        {

            if(ret && FD_ISSET(fd, &fds))
            {
                memset(buf, 0, sizeof(buf));
                len = read(fd, buf, sizeof(buf));
                if(len > 0)
                {
                    respone = &buf[0];
                    if(IsUtf8Format(buf))
                    {

                        msg = dbus_message_new_signal(DBUS_SERVER_PATH,
                                                      DBUS_SERVER_INTERFACE,
                                                      "sigSerialRecv");
                        dbus_message_iter_init_append(msg, &args);
                        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &fd)) {
                            fprintf(stderr, "Out Of Memory!\n");
                            return;
                        }
                        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &respone)) {
                            fprintf(stderr, "Out Of Memory!\n");
                            return;
                        }
                        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &len)) {
                            fprintf(stderr, "Out Of Memory!\n");
                            return;
                        }
                        if (!dbus_connection_send(dbus_server_conn, msg, NULL)) {
                            fprintf(stderr, "Out Of Memory!\n");
                            return;
                        }

                        dbus_connection_flush(dbus_server_conn);
                        dbus_message_unref(msg);
                    }
                    else
                    {
                        printf("str is not utf-8!\n");
                    }


                }
            }

        }
    }
    return NULL;
}
void * tty_read_thread2(void *arg)
{
    struct timeval tv;
    DBusMessage* msg;
    DBusMessageIter args;
    fd_set fds;
    int ret=0,len=0;
    char buf[50] = {0};
    char *respone = NULL;
    int fd = (int )arg;
    printf("tty_read_thread2 fd:%d\n",fd);
    while(1)
    {
        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        tv.tv_sec = 0;
        tv.tv_usec = 100000 ;

        ret = select(fd+1, &fds, NULL, NULL, &tv);
        if(ret < 0){
                perror("select error\n");
        }
        else
        {

            if(ret && FD_ISSET(fd, &fds))
            {
                memset(buf, 0, sizeof(buf));
                len = read(fd, buf, sizeof(buf));
                if(len > 0)
                {
                    respone = &buf[0];
                    if(IsUtf8Format(buf))
                    {

                        msg = dbus_message_new_signal(DBUS_SERVER_PATH,
                                                      DBUS_SERVER_INTERFACE,
                                                      "sigSerialRecv");
                        dbus_message_iter_init_append(msg, &args);
                        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &fd)) {
                            fprintf(stderr, "Out Of Memory!\n");
                            return;
                        }
                        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &respone)) {
                            fprintf(stderr, "Out Of Memory!\n");
                            return;
                        }
                        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &len)) {
                            fprintf(stderr, "Out Of Memory!\n");
                            return;
                        }
                        if (!dbus_connection_send(dbus_server_conn, msg, NULL)) {
                            fprintf(stderr, "Out Of Memory!\n");
                            return;
                        }

                        dbus_connection_flush(dbus_server_conn);
                        dbus_message_unref(msg);
                    }
                    else
                    {
                        printf("str is not utf-8!\n");
                    }


                }
            }

        }
    }
    return NULL;
}
void * tty_read_thread3(void *arg)
{
    struct timeval tv;
    DBusMessage* msg;
    DBusMessageIter args;
    fd_set fds;
    int ret=0,len=0;
    char buf[50] = {0};
    char *respone = NULL;
    int fd = (int )arg;
    printf("tty_read_thread3 fd:%d\n",fd);
    while(1)
    {
        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        tv.tv_sec = 0;
        tv.tv_usec = 100000 ;

        ret = select(fd+1, &fds, NULL, NULL, &tv);
        if(ret < 0){
                perror("select error\n");
        }
        else
        {

            if(ret && FD_ISSET(fd, &fds))
            {
                memset(buf, 0, sizeof(buf));
                len = read(fd, buf, sizeof(buf));
                if(len > 0)
                {
                    respone = &buf[0];
                    if(IsUtf8Format(buf))
                    {

                        msg = dbus_message_new_signal(DBUS_SERVER_PATH,
                                                      DBUS_SERVER_INTERFACE,
                                                      "sigSerialRecv");
                        dbus_message_iter_init_append(msg, &args);
                        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &fd)) {
                            fprintf(stderr, "Out Of Memory!\n");
                            return;
                        }
                        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &respone)) {
                            fprintf(stderr, "Out Of Memory!\n");
                            return;
                        }
                        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &len)) {
                            fprintf(stderr, "Out Of Memory!\n");
                            return;
                        }
                        if (!dbus_connection_send(dbus_server_conn, msg, NULL)) {
                            fprintf(stderr, "Out Of Memory!\n");
                            return;
                        }

                        dbus_connection_flush(dbus_server_conn);
                        dbus_message_unref(msg);
                    }
                    else
                    {
                        printf("str is not utf-8!\n");
                    }


                }
            }

        }
    }
    return NULL;
}
void * tty_read_thread4(void *arg)
{
    struct timeval tv;
    DBusMessage* msg;
    DBusMessageIter args;
    fd_set fds;
    int ret=0,len=0;
    char buf[50] = {0};
    char *respone = NULL;
    int fd = (int )arg;
    printf("tty_read_thread4 fd:%d\n",fd);
    while(1)
    {
        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        tv.tv_sec = 0;
        tv.tv_usec = 100000 ;

        ret = select(fd+1, &fds, NULL, NULL, &tv);
        if(ret < 0){
                perror("select error\n");
        }
        else
        {

            if(ret && FD_ISSET(fd, &fds))
            {
                memset(buf, 0, sizeof(buf));
                len = read(fd, buf, sizeof(buf));
                if(len > 0)
                {
                    respone = &buf[0];
                    if(IsUtf8Format(buf))
                    {

                        msg = dbus_message_new_signal(DBUS_SERVER_PATH,
                                                      DBUS_SERVER_INTERFACE,
                                                      "sigSerialRecv");
                        dbus_message_iter_init_append(msg, &args);
                        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &fd)) {
                            fprintf(stderr, "Out Of Memory!\n");
                            return;
                        }
                        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &respone)) {
                            fprintf(stderr, "Out Of Memory!\n");
                            return;
                        }
                        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &len)) {
                            fprintf(stderr, "Out Of Memory!\n");
                            return;
                        }
                        if (!dbus_connection_send(dbus_server_conn, msg, NULL)) {
                            fprintf(stderr, "Out Of Memory!\n");
                            return;
                        }

                        dbus_connection_flush(dbus_server_conn);
                        dbus_message_unref(msg);
                    }
                    else
                    {
                        printf("str is not utf-8!\n");
                    }


                }
            }

        }
    }
    return NULL;
}
void parse_tty_param(char *tty_param)
{
    int i = 0, j = 0;
    char *param[10];
    char *temp = strtok(tty_param," ");


    while(temp)
    {
        param[i++] = temp;
        temp = strtok(NULL," ");
    }

    if(i == 7)
    {
        tty_setting(atoi(param[0]),atoi(param[1]),atoi(param[2]),atoi(param[3]),atoi(param[4]),param[5][0],atoi(param[6]));

        for(i = 0; i < MAX_OPENED_TTY;i++)
        {
            if(opened_tty[i].fd != 0)
            {

                j++;
            }
        }
        create_tty_read_thread(j,atoi(param[0]));
    }
}
void create_tty_read_thread(int thread_cnt,int fd)
{
   int i = 0;

   pthread_create(&tty_thread_id[thread_cnt-1],NULL,tty_thread[thread_cnt-1],(void *) fd);
   printf("pthread_create pthread_id:%lu fd:%d\n",tty_thread_id[thread_cnt-1],fd);
   for(i = 0; i < MAX_OPENED_TTY;i++)
   {
       if((opened_tty[i].fd == fd) && (opened_tty[i].fd != 0))
       {
            opened_tty[i].pthread_id = tty_thread_id[thread_cnt-1];

       }
   }

}
void delete_tty_read_thread(int fd)
{
    int i =0;
    for(i = 0; i < MAX_OPENED_TTY;i++)
    {
        if((opened_tty[i].fd == fd) && (opened_tty[i].fd != 0))
        {
            opened_tty[i].open_cnt--;
            if(opened_tty[i].open_cnt == 0)
            {
                printf("pthread_cancel pthread_id:%lu fd:%d\n",opened_tty[i].pthread_id,fd);
                pthread_cancel(opened_tty[i].pthread_id);
                pthread_join(opened_tty[i].pthread_id,NULL);
                opened_tty[i].pthread_id = 0;
            }
        }
    }

}
/******************************************************************************
  Function:       tty_init
  Description:    initialize tty device
  Input:          tty    	--  tty device name, such as '/dev/ttyO0', '/dev/ttyO5'

  Output:
  Return:         int		-- return the tty fd
  Others:         NONE
*******************************************************************************/

void tty_init()
{
    int i = 0;
    for(i = 0; i< MAX_OPENED_TTY;i++)
    {
        memset(&opened_tty[i], 0, sizeof(struct g_opened_tty));
    }
    tty_thread[0] = tty_read_thread0;
    tty_thread[1] = tty_read_thread1;
    tty_thread[2] = tty_read_thread2;
    tty_thread[3] = tty_read_thread3;
    tty_thread[4] = tty_read_thread4;
}
