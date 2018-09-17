#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "info.h"

#define SYSTEM_INFO_CMD "uname -a"
#define CPU_INFO_CMD "cat /proc/cpuinfo"
#define MEM_INFO_PATH "/proc/meminfo"
#define MEM_INFO_LINE 28
#define STORAGE_MMC_INFO_CMD "fdisk -l | grep mmcblk*"
#define STORAGE_NAND_INFO_CMD " fdisk -l | grep /dev/mtd*"

#define DEVICE_TREE_CAMPATIABLE "cat /sys/firmware/devicetree/base/compatible | grep am43"
#define AM437X "am43"
static int run_system(const char * cmd, char *pRetMsg, int msg_len)
{
    FILE * fp;
    char * p = NULL;
    int res = -1;
    if (cmd == NULL || pRetMsg == NULL || msg_len < 0)
    {
        printf("Param Error!\n");
        return -1;
    }
    if ((fp = popen(cmd, "r") ) == NULL)
    {
        printf("Popen Error!\n");
        return -2;
    }
    else
    {
        memset(pRetMsg, 0, msg_len);
        //get lastest result
        while(fread(pRetMsg, msg_len,1, fp) != 0)
        {
            printf("Msg:%s",pRetMsg); //print all info
        }

        if ( (res = pclose(fp)) == -1)
        {
            printf("close popenerror!\n");
            return -3;
        }
        pRetMsg[strlen(pRetMsg)-1] = '\0';
        return 0;
    }
}

void get_system_info(char * res)
{
    char tmp[100] = {0};

    run_system(SYSTEM_INFO_CMD, tmp , sizeof(tmp));

    memcpy(res, tmp ,strlen(tmp));
}

void get_cpu_info(char * res)
{
    char tmp[500] = {0};

    run_system(CPU_INFO_CMD,tmp,sizeof(tmp));

    memcpy(res, tmp, strlen(tmp));
}
 void get_mem_info(char * res)
{

    int fd = 0;
    char buf[3*MEM_INFO_LINE] = {0};

    fd = open(MEM_INFO_PATH,O_RDONLY);
    if(fd < 0)
    {
        printf("can not load file!");
        return 1;
    }

    read(fd , buf, sizeof(buf));
    close(fd);

    memcpy(res, buf, sizeof(buf));
}
void get_storage_info(char *res)
{
    int fd = 0;
    char buf[1500] = {0};
    char device_tree_buf[100] = {0};

    run_system(DEVICE_TREE_CAMPATIABLE,device_tree_buf,sizeof(device_tree_buf));
    if(strstr(device_tree_buf, AM437X))
    {

        run_system(STORAGE_MMC_INFO_CMD, buf , sizeof(buf));
    }
    else
    {

        run_system(STORAGE_NAND_INFO_CMD, buf , sizeof(buf));
    }

    memcpy(res, buf, strlen(buf));
   // close(fd);
}
