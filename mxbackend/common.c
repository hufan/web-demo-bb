#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cJSON.h>
#include "common.h"
char *board_cfg;
unsigned char utf8_look_for_table[] =
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1};

#define UTFLEN(x) utf8_look_for_table[(x)]

//根据首字节,获取utf8字符所占字节数
inline int GetUtf8charByteNum(unsigned char ch)
{
    int byteNum = 0;

    if (ch >= 0xFC && ch < 0xFE)
        byteNum = 6;
    else if (ch >= 0xF8)
        byteNum = 5;
    else if (ch >= 0xF0)
        byteNum = 4;
    else if (ch >= 0xE0)
        byteNum = 3;
    else if (ch >= 0xC0)
        byteNum = 2;
    else if (0 == (ch & 0x80))
        byteNum = 1;

    return byteNum;
}

//判断字符串是否是utf8格式
int IsUtf8Format(const char *str)
{
    int byteNum = 0;
    unsigned char ch;
    const char *ptr = str;

    if (NULL == str)
        return 0;

    while (*ptr != '\0')
    {
        ch = (unsigned char)*ptr;
        if (byteNum == 0) //根据首字节特性判断该字符的字节数
        {
            if (0 == (byteNum = GetUtf8charByteNum(ch)))
                return 0;
        }
        else //多字节字符,非首字节格式:10xxxxxx
        {
            if ((ch & 0xC0) != 0x80)
                return 0;
        }
        byteNum--;
        ptr++;
    }

    if (byteNum > 0)
        return 0;

    return 1;
}
void board_cfg_init()
{
    FILE *f;
    long len;

    f=fopen(MXDE_BOARD_CFG_PATH,"rb");
    fseek(f,0,SEEK_END);
    len=ftell(f);
    fseek(f,0,SEEK_SET);
    board_cfg=(char*)malloc(len+1);
    fread(board_cfg,1,len,f);
    fclose(f);
}
void get_cfg_list(char *item_name,char * result)
{

    cJSON *json;
    cJSON *node;
    cJSON *item;
    cJSON *tnode;
    int i=0,n=0;

    json=cJSON_Parse(board_cfg);
    if (!json) {
        printf("Error before: [%s]\n",cJSON_GetErrorPtr());
        return;
    }

    item = cJSON_GetObjectItem(json,"board_info");
    if(item == NULL)
    {
        printf("Error before: [%s]\n",cJSON_GetErrorPtr());
        return ;
    }
    node = cJSON_GetObjectItem(item,item_name);
    if(node->type == cJSON_Array)
    {
        int size1 = cJSON_GetArraySize(node);
        for(i=0;i<size1;i++)
        {
            tnode = cJSON_GetArrayItem(node,i);
            if(tnode->type == cJSON_String)
            {
                n +=sprintf(result+n,"%s\n",tnode->valuestring);
            }
        }

    }
}
