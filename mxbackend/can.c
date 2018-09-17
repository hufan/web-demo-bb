/******************************************************************************
* Copyright (C), 2016-2017, Sunny.Guo
* FileName: can.c 
* Author: Sunny.Guo
* Version: 1.0
* Date: 2017年 01月 19日 星期四 10:12:50 CST
* Description: can operation functions
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

#include "common.h"
#include "can.h"


static int can_baudrate = CAN_DEFAULT_BAUDRATE;

int thread_can_fd = 0;


#define MAX_CAN_OPENED 5
struct opened_can_t opened_can[MAX_CAN_OPENED];

pthread_t can_thread_id[MAX_CAN_OPENED] = {0};
void *(*can_thread[MAX_CAN_OPENED])(void *args);

static unsigned char asc2nibble(char c) {

	if ((c >= '0') && (c <= '9'))
		return c - '0';

	if ((c >= 'A') && (c <= 'F'))
		return c - 'A' + 10;

	if ((c >= 'a') && (c <= 'f'))
		return c - 'a' + 10;

	return 16; /* error */
}


/******************************************************************************
  Function:       can_init
  Description:    initialize can device
  Input:          can    	--  can device name, such as 'can0', 'can1'
                   
  Output:          
  Return:         int		-- return the can fd
  Others:         NONE
*******************************************************************************/
int				can_init(const char * can)
{
	int canfd;
	struct ifreq ifr;
	struct sockaddr_can can_addr;
	int loopback = 0;
    int flags,i;

	memset(&ifr, 0, sizeof(ifr));
	
	canfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if (canfd < 0) {
		dbg_perror("socket");
		return canfd;
	}
	
	strcpy(ifr.ifr_name, can);

	/* Determine the interface index */
	ioctl(canfd, SIOCGIFINDEX, &ifr);
	can_addr.can_family = AF_CAN;
	can_addr.can_ifindex = ifr.ifr_ifindex;
	
  	flags = fcntl(canfd,F_GETFL,0);
	fcntl(canfd,F_SETFL,O_NONBLOCK|flags);

	/* bin the socket to a CAN interface */
	bind(canfd, (struct sockaddr *)&can_addr, sizeof(can_addr));
	
	/* Set the lookback rules */
    /*CAN_RAW_FILTER CAN_RAW_LOOPBACK*/
    setsockopt(canfd, SOL_CAN_RAW, CAN_RAW_FILTER,
			   &loopback, sizeof(loopback));


    for(i = 0 ;i < MAX_CAN_OPENED;i++ )
    {
        if(opened_can[i].device_name != NULL)
        {
            if(strcmp(opened_can[i].device_name,can) == 0)
            {
                opened_can[i].fd = canfd;
            }
        }
    }
	return canfd;
}
void close_can_port(char *can_name,int can_fd)
{
    int i;
    struct opened_can_t can_ret;
    for(i = 0 ;i < MAX_CAN_OPENED;i++ )
    {
        if((opened_can[i].fd == can_fd) && (opened_can[i].fd != 0))
        {

            printf("opened_can[%d].open_cnt %d\n",i,opened_can[i].open_cnt);
            if(opened_can[i].open_cnt <= 0)
            {
                memset(&opened_can[i], 0, sizeof(struct opened_can_t));
                close(can_fd);

                can_setting(can_name,can_baudrate,CAN_DISABLE,"off",&can_ret);
            }
        }
    }

}

/******************************************************************************
  Function:       can_setting
  Description:    set the can device's bitrate and status
  Input:          can      --  can name string
  				  bitrate  --  can bitrate
  				  enable   --  can status, 1: enable; 0: disable
                  
  Output:          
  Return:         int 	   --  can setting status 0:success 
  Others:         NONE
*******************************************************************************/
int 			can_setting(const char* can, const int bitrate, int enable,const char* loop,struct opened_can_t *can_configure)
{
	int ret = 0;
	char cmdline[128] = { '\0' };
    can_baudrate = bitrate;

    int i = 0,j= 0;


	if(enable == 1){
        for(i = 0 ;i < MAX_CAN_OPENED;i++ )
        {
            if(opened_can[i].device_name != NULL)
            {
                if(strcmp(opened_can[i].device_name,can) == 0)
                {
                    break;
                }
            }
        }

        if(i == MAX_CAN_OPENED)
        {
            for(j = 0; j < MAX_CAN_OPENED ; j++)
            {
                if(opened_can[j].device_name == NULL)
                    break;
            }
            opened_can[j].device_name = can;
            opened_can[j].bitrate = bitrate;
            opened_can[j].loop = loop;
            opened_can[j].open_cnt++;

        }

        if(i != MAX_CAN_OPENED)
        {
            printf("can opened!\n");
            opened_can[i].open_cnt++;
            memcpy(can_configure,&opened_can[i],sizeof(struct opened_can_t));

            return 100;
        }
        if(strcmp(loop,"ON") == 0)
        {
            sprintf(cmdline,
                    "ip link set %s down; ip link set %s type can bitrate %ld triple-sampling on  loopback on; ip link set %s up",
                    can, can, bitrate, can);
        }
        if(strcmp(loop,"OFF") == 0)
        {
            sprintf(cmdline,
                    "ip link set %s down; ip link set %s type can bitrate %ld triple-sampling on  loopback off; ip link set %s up",
                    can, can, bitrate, can);
        }
    }
	else if(enable == 0){

		sprintf(cmdline,
				"ip link set %s down",
				can);
		}

	ret = system(cmdline);
	return WEXITSTATUS(ret);
}

int				can_read(int fd, struct can_frame *frame)
{
	struct timeval tv;
	fd_set readfds;
	
	int ret = 0;
	unsigned int timeout = CAN_READ_TIMEOUT_USEC;
	
	tv.tv_sec = 0;
	tv.tv_usec = timeout;

	FD_ZERO(&readfds);
	FD_SET(fd, &readfds);

	if(timeout > 0){
		ret = select(fd+1, &readfds, NULL, NULL, &tv);

		if(ret < 0 ){
			dbg_perror("select can fd\n");
		}
		else if(ret == 0){
			//dbg_printf("select can timeout\n");
			
}
		}
	
	if(FD_ISSET(fd, &readfds)){
		ret = read(fd, frame, sizeof(struct can_frame));
		}

	return ret;
}

int				can_write(int fd, struct can_frame *frame, int len)
{
	int ret = 0;

	ret = write(fd, frame, len);

	return ret;
}

int				parse_canframe(char* strFrame, struct can_frame *frame)
	{
		int i, idx, dlen, len;
		int maxdlen = CAN_MAX_DLEN;
		int ret = CAN_MTU;
		unsigned char tmp;
	
		len = strlen(strFrame);

		memset(frame, 0, sizeof(*frame)); /* init CAN FD frame, e.g. LEN = 0 */
	
		if (len < 4)
			return 0;
	
		if (strFrame[3] == CANID_DELIM) { /* 3 digits */
	
			idx = 4;
			for (i=0; i<3; i++){
				if ((tmp = asc2nibble(strFrame[i])) > 0x0F)
					return 0;
				frame->can_id |= (tmp << (2-i)*4);
			}
		} else
			return 0;
	
		if((strFrame[idx] == 'R') || (strFrame[idx] == 'r')){ /* RTR frame */
			frame->can_id |= CAN_RTR_FLAG;
	
			/* check for optional DLC value for CAN 2.0B frames */
			if(strFrame[++idx] && (tmp = asc2nibble(strFrame[idx])) <= CAN_MAX_DLC)
				frame->can_dlc = tmp;
	
			return ret;
		}
	
		for (i=0, dlen=0; i < maxdlen; i++){
	
			if(strFrame[idx] == DATA_SEPERATOR) /* skip (optional) separator */
				idx++;
	
			if(idx >= len) /* end of string => end of data */
				break;
	
			if ((tmp = asc2nibble(strFrame[idx++])) > 0x0F)
				return 0;
			frame->data[i] = (tmp << 4);
			if ((tmp = asc2nibble(strFrame[idx++])) > 0x0F)
				return 0;
			frame->data[i] |= tmp;
			dlen++;
		}
		frame->can_dlc = dlen;
		
// for debug only
//
//		dbg_printf("=== parsed frame===== \n");
//		dbg_printf(" frame_id = 0x%x \n", frame->can_id);
//		dbg_printf(" frame_len = %d \n", frame->can_dlc);
//		
//		for(i=0; i <frame->can_dlc; i++){
//			dbg_printf("%#x ", frame->data[i]);
//			}
//		dbg_printf("\n");
//		dbg_printf("================= \n");
//	
		return ret;
	}

void get_can_list(char * result)
{

    get_cfg_list("can",result);

}



void * can_read_thread0(void *arg)
{
    struct timeval tv;
    DBusMessage* msg;
    DBusMessageIter args;
    fd_set fds;
    int ret=0,len=0,can_id,can_dlc,j;
    struct can_frame can_read_frame;
    char *respone = NULL;
    char temp[100] = {0};
    int fd = (int )arg;
    printf("can_read_thread0 fd:%d\n",fd);
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
                memset(&can_read_frame, 0, sizeof( struct can_frame));
                len = read(fd, &can_read_frame, sizeof(struct can_frame));
                if(len > 0)
                {
                    memset(temp, 0, sizeof(temp));
                    j = 0;

                    for(int i = 0; i < can_read_frame.can_dlc; i++){
                                j += sprintf(temp+j,"%#x ", can_read_frame.data[i]);
                       }

                    can_id = can_read_frame.can_id;
                    can_dlc = can_read_frame.can_dlc;

                    respone = &temp[0];
                    msg = dbus_message_new_signal(DBUS_SERVER_PATH,
                                                  DBUS_SERVER_INTERFACE,
                                                  "sigCanRecv");
                    dbus_message_iter_init_append(msg, &args);
                    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &fd)) {
                        fprintf(stderr, "Out Of Memory!\n");
                        return;
                    }
                    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &can_id)) {
                        fprintf(stderr, "Out Of Memory!\n");
                        return;
                    }
                    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &can_dlc)) {
                        fprintf(stderr, "Out Of Memory!\n");
                        return;
                    }
                    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &respone)) {
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
            }

        }
    }
    return NULL;
}
void * can_read_thread1(void *arg)
{
    struct timeval tv;
    DBusMessage* msg;
    DBusMessageIter args;
    fd_set fds;
    int ret=0,len=0,can_id,can_dlc,j;
    struct can_frame can_read_frame;
    char *respone = NULL;
    char temp[100] = {0};
    int fd = (int )arg;
    printf("can_read_thread1 fd:%d\n",fd);
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
                memset(&can_read_frame, 0, sizeof( struct can_frame));
                len = read(fd, &can_read_frame, sizeof(struct can_frame));
                if(len > 0)
                {
                    memset(temp, 0, sizeof(temp));
                    j = 0;

                    for(int i = 0; i < can_read_frame.can_dlc; i++){
                                j += sprintf(temp+j,"%#x ", can_read_frame.data[i]);
                       }

                    can_id = can_read_frame.can_id;
                    can_dlc = can_read_frame.can_dlc;

                    respone = &temp[0];
                    msg = dbus_message_new_signal(DBUS_SERVER_PATH,
                                                  DBUS_SERVER_INTERFACE,
                                                  "sigCanRecv");
                    dbus_message_iter_init_append(msg, &args);
                    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &fd)) {
                        fprintf(stderr, "Out Of Memory!\n");
                        return;
                    }
                    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &can_id)) {
                        fprintf(stderr, "Out Of Memory!\n");
                        return;
                    }
                    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &can_dlc)) {
                        fprintf(stderr, "Out Of Memory!\n");
                        return;
                    }
                    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &respone)) {
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
            }

        }
    }
    return NULL;
}
void * can_read_thread2(void *arg)
{
    struct timeval tv;
    DBusMessage* msg;
    DBusMessageIter args;
    fd_set fds;
    int ret=0,len=0,can_id,can_dlc,j;
    struct can_frame can_read_frame;
    char *respone = NULL;
    char temp[100] = {0};
    int fd = (int )arg;
    printf("can_read_thread2 fd:%d\n",fd);
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
                memset(&can_read_frame, 0, sizeof( struct can_frame));
                len = read(fd, &can_read_frame, sizeof(struct can_frame));
                if(len > 0)
                {
                    memset(temp, 0, sizeof(temp));
                    j = 0;

                    for(int i = 0; i < can_read_frame.can_dlc; i++){
                                j += sprintf(temp+j,"%#x ", can_read_frame.data[i]);
                       }

                    can_id = can_read_frame.can_id;
                    can_dlc = can_read_frame.can_dlc;

                    respone = &temp[0];
                    msg = dbus_message_new_signal(DBUS_SERVER_PATH,
                                                  DBUS_SERVER_INTERFACE,
                                                  "sigCanRecv");
                    dbus_message_iter_init_append(msg, &args);
                    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &fd)) {
                        fprintf(stderr, "Out Of Memory!\n");
                        return;
                    }
                    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &can_id)) {
                        fprintf(stderr, "Out Of Memory!\n");
                        return;
                    }
                    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &can_dlc)) {
                        fprintf(stderr, "Out Of Memory!\n");
                        return;
                    }
                    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &respone)) {
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
            }

        }
    }
    return NULL;
}
void * can_read_thread3(void *arg)
{
    struct timeval tv;
    DBusMessage* msg;
    DBusMessageIter args;
    fd_set fds;
    int ret=0,len=0,can_id,can_dlc,j;
    struct can_frame can_read_frame;
    char *respone = NULL;
    char temp[100] = {0};
    int fd = (int )arg;
    printf("can_read_thread3 fd:%d\n",fd);
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
                memset(&can_read_frame, 0, sizeof( struct can_frame));
                len = read(fd, &can_read_frame, sizeof(struct can_frame));
                if(len > 0)
                {
                    memset(temp, 0, sizeof(temp));
                    j = 0;

                    for(int i = 0; i < can_read_frame.can_dlc; i++){
                                j += sprintf(temp+j,"%#x ", can_read_frame.data[i]);
                       }

                    can_id = can_read_frame.can_id;
                    can_dlc = can_read_frame.can_dlc;

                    respone = &temp[0];
                    msg = dbus_message_new_signal(DBUS_SERVER_PATH,
                                                  DBUS_SERVER_INTERFACE,
                                                  "sigCanRecv");
                    dbus_message_iter_init_append(msg, &args);
                    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &fd)) {
                        fprintf(stderr, "Out Of Memory!\n");
                        return;
                    }
                    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &can_id)) {
                        fprintf(stderr, "Out Of Memory!\n");
                        return;
                    }
                    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &can_dlc)) {
                        fprintf(stderr, "Out Of Memory!\n");
                        return;
                    }
                    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &respone)) {
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
            }

        }
    }
    return NULL;
}
void * can_read_thread4(void *arg)
{
    struct timeval tv;
    DBusMessage* msg;
    DBusMessageIter args;
    fd_set fds;
    int ret=0,len=0,can_id,can_dlc,j;
    struct can_frame can_read_frame;
    char *respone = NULL;
    char temp[100] = {0};
    int fd = (int )arg;
    printf("can_read_thread4 fd:%d\n",fd);
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
                memset(&can_read_frame, 0, sizeof( struct can_frame));
                len = read(fd, &can_read_frame, sizeof(struct can_frame));
                if(len > 0)
                {
                    memset(temp, 0, sizeof(temp));
                    j = 0;

                    for(int i = 0; i < can_read_frame.can_dlc; i++){
                                j += sprintf(temp+j,"%#x ", can_read_frame.data[i]);
                       }

                    can_id = can_read_frame.can_id;
                    can_dlc = can_read_frame.can_dlc;

                    respone = &temp[0];
                    msg = dbus_message_new_signal(DBUS_SERVER_PATH,
                                                  DBUS_SERVER_INTERFACE,
                                                  "sigCanRecv");
                    dbus_message_iter_init_append(msg, &args);
                    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &fd)) {
                        fprintf(stderr, "Out Of Memory!\n");
                        return;
                    }
                    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &can_id)) {
                        fprintf(stderr, "Out Of Memory!\n");
                        return;
                    }
                    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &can_dlc)) {
                        fprintf(stderr, "Out Of Memory!\n");
                        return;
                    }
                    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &respone)) {
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
            }

        }
    }
    return NULL;
}
void create_can_read_thread(int fd)
{
    int i=0,j=0;
    for(i = 0; i < MAX_CAN_OPENED;i++)
    {
        if(opened_can[i].fd != 0)
        {

            j++;
        }
    }
    //create_tty_read_thread(j,atoi(param[0]));
    pthread_create(&can_thread_id[j-1],NULL,can_thread[j-1],(void *) fd);
    printf("pthread_create pthread_id:%lu fd:%d\n",can_thread_id[j-1],fd);
    for(i = 0; i < MAX_CAN_OPENED;i++)
    {
        if((opened_can[i].fd == fd) && (opened_can[i].fd != 0))
        {
             opened_can[i].pthread_id = can_thread_id[j-1];

        }
    }
}
void delete_can_read_thread(int fd)
{

    int i =0;
    for(i = 0; i < MAX_CAN_OPENED;i++)
    {
        if((opened_can[i].fd == fd) && (opened_can[i].fd != 0))
        {
            opened_can[i].open_cnt--;
            if(opened_can[i].open_cnt == 0)
            {
                printf("pthread_cancel pthread_id:%lu fd:%d\n",opened_can[i].pthread_id,fd);
                pthread_cancel(opened_can[i].pthread_id);
                pthread_join(opened_can[i].pthread_id,NULL);
                opened_can[i].pthread_id = 0;
            }
        }
    }
}
void can_data_write(int fd,char *data)
{
    struct can_frame  can_write_frame;
    int ret = 0;

    memset(&can_write_frame, 0, sizeof(struct can_frame));
    ret = parse_canframe(data, &can_write_frame);
    if( ret <= 0){
        dbg_printf("Parse can frame  %s failed!\r\n", data);
        fprintf(stderr, "\nWrong CAN-frame format! Try:\r\n");
        fprintf(stderr, "  <can_id>#{R|data} for CAN 2.0 frames\n");
        fprintf(stderr, "  <can_id> can have 3 (SFF)  hex chars\n");
        fprintf(stderr, "  {data} has 0..8  ASCII hex-values (optionally");
        fprintf(stderr, "  separated by '.')\n");
        fprintf(stderr, "  e.g. 5A1#11.2233.44556677.88 / 123#DEADBEEF / 5AA# / \r\n ");
        return;
     }
    ret = can_write(fd , &can_write_frame, sizeof(can_write_frame));
    if(ret <= 0){

        dbg_printf("Write can device failed!\r\n");
        return;
    }


}
void can_data_init()
{
    int i = 0;
    for(i = 0; i< MAX_CAN_OPENED;i++)
    {
        memset(&opened_can[i], 0, sizeof(struct opened_can_t));
    }
    can_thread[0] = can_read_thread0;
    can_thread[1] = can_read_thread1;
    can_thread[2] = can_read_thread2;
    can_thread[3] = can_read_thread3;
    can_thread[4] = can_read_thread4;
}
