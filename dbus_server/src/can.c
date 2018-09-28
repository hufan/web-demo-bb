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

#define CAN_INFO_PATH "/usr/share/myir/board_can_info"
static int can_baudrate = CAN_DEFAULT_BAUDRATE;
pthread_t can_thread_id = 0;
int thread_can_fd = 0;
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
	int flags;

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
	setsockopt(canfd, SOL_CAN_RAW, CAN_RAW_LOOPBACK,
			   &loopback, sizeof(loopback));
    thread_can_fd = canfd;
	return canfd;
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
int 			can_setting(const char* can, const int bitrate, int enable,const char* loop)
{
	int ret = 0;
	char cmdline[128] = { '\0' };
    can_baudrate = bitrate;
	if(enable == 1){

        if(strcmp(loop,"ON") == 0)
        {
        }
        if(strcmp(loop,"OFF") == 0)
        {
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
    char tmp[30]={0};
    int n = 0;
    FILE *fp;

    fp = fopen(CAN_INFO_PATH,"r");

    if (NULL == fp)
      {
          printf("fopen error!\n");
          return;
      }
    while(fgets(tmp,20,fp) != NULL)
    {
        n +=sprintf(result+n,"%s",tmp);
        memset(tmp, 0, sizeof(tmp));
    }
}


void close_can_port(char *can_name,int can_fd)
{
    //delete can recv thread
    close(can_fd);
    thread_can_fd = 0;
    can_setting(can_name,can_baudrate,CAN_DISABLE,"off");
}

int * can_read_thread(void *arg)
{
    struct timeval tv;
    DBusMessage* msg;
    DBusMessageIter args;
    fd_set fds;
    int ret=0,len=0,can_id,can_dlc,j;
    struct can_frame can_read_frame;
    char *respone = NULL;
    char temp[100] = {0};

    while(1)
    {
        FD_ZERO(&fds);
        FD_SET(thread_can_fd, &fds);

        tv.tv_sec = 0;
        tv.tv_usec = 100000 ;

        ret = select(thread_can_fd+1, &fds, NULL, NULL, &tv);
        if(ret < 0){
                perror("select error\n");
        }
        else
        {

            if(ret && FD_ISSET(thread_can_fd, &fds))
            {
                memset(&can_read_frame, 0, sizeof( struct can_frame));
                len = read(thread_can_fd, &can_read_frame, sizeof(struct can_frame));
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
                    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &thread_can_fd)) {
                        fprintf(stderr, "Out Of Memory!\n");
                        return 0;
                    }
                    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &can_id)) {
                        fprintf(stderr, "Out Of Memory!\n");
                        return 0;
                    }
                    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &can_dlc)) {
                        fprintf(stderr, "Out Of Memory!\n");
                        return 0;
                    }
                    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &respone)) {
                        fprintf(stderr, "Out Of Memory!\n");
                        return 0;
                    }
                    if (!dbus_connection_send(dbus_server_conn, msg, NULL)) {
                        fprintf(stderr, "Out Of Memory!\n");
                        return 0;
                    }

                    dbus_connection_flush(dbus_server_conn);
                    dbus_message_unref(msg);
                }
            }

        }
    }
    return NULL;
}
void create_can_read_thread()
{
     pthread_create(&can_thread_id,NULL,(void *)can_read_thread,NULL);
}
void delete_can_read_thread()
{
    if(can_thread_id != 0)
    {

        pthread_cancel(can_thread_id);
        pthread_join(can_thread_id,NULL);
        can_thread_id = 0;

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
