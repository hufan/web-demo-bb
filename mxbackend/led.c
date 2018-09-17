#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "led.h"

#define MAX_LED_NO 5
char leds[MAX_LED_NO][30] = {0};
int led_num = 0;
int set_led_brightness(const char * led, int on)
{
	int ret = 0;
	int i = 0;
	char cmdline[128];
	
	for(i = 0; i < led_num; i++)
	{
		
		if(strcmp(led, leds[i]) == 0)
			break;          
	}
	if( i == led_num)
	{
		printf("argument: led name error\n");
		return -1;
	}
	sprintf(cmdline, "echo %d >  /sys/class/leds/%s/brightness", on, led);
	ret =  system(cmdline);
	return  WEXITSTATUS(ret);
}

int run_system(const char * cmd, char *pRetMsg, int msg_len)  
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

void parse_led_list(char *input,char *led_list)
{
	int i=0,j=0,n=0;
	char *led_name[5];
	char cmd[100];
	char led_brightness[10];
	char respone[200];
	
    char *temp = strtok(input,"\n");
    while(temp)
    {
        led_name[i++] = temp;
        temp = strtok(NULL,"\n");
    } 
	for(j = 0; j < i; j++)
	{
		memset(cmd,0,sizeof(cmd));
		memset(led_brightness,0,sizeof(led_brightness));
		
		sprintf(cmd,"cat /sys/class/leds/%s/brightness",led_name[j]);
		run_system(cmd,led_brightness,sizeof(led_brightness));
		
		n +=sprintf(respone+n,"%s %s\n",led_name[j],led_brightness);
	}
	dbg_printf("respone:%s\n",respone);
	strcpy(led_list,respone);
}
void  get_led_list(char * led_list)
{
	char result[120] = {0};

	run_system("ls /sys/class/leds/",result,sizeof(result));
	parse_led_list(result,led_list);
}

void led_init()
{
	char result[120] = {0};
	int i = 0;
	

	run_system("ls /sys/class/leds/",result,sizeof(result));

	char *temp = strtok(result,"\n");

	while(temp)
    {
        //leds[i++] = temp;
		strcpy(leds[i++],temp);
        temp = strtok(NULL,"\n");
    }
	led_num = i;

	

}