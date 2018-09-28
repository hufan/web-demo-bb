#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dbus/dbus.h>
#include <unistd.h>
#include <libxml/parser.h>  
#include <libxml/xmlmemory.h>  
#include "led.h"
#include "dbus_service.h"


void Introspect_method_call(DBusMessage * msg, DBusConnection * conn)
{
	DBusMessage * reply;
	DBusMessageIter arg;
    xmlChar *xmlbuf;
	xmlDocPtr doc; 
	int buffersize;
	char *respone;
	
	doc = xmlReadFile("/usr/share/myir/mxde.xml","UTF-8",XML_PARSE_RECOVER);  
    xmlDocDumpFormatMemory(doc, &xmlbuf, &buffersize, 1);  
	respone = (char *)xmlbuf;
	
	reply = dbus_message_new_method_return(msg);
	dbus_message_iter_init_append(reply,&arg); 
	if(!dbus_message_iter_append_basic (&arg,DBUS_TYPE_STRING,&respone)){
		printf("Out of Memory!/n");
		return;
	}
	if( !dbus_connection_send (conn, reply, NULL)){
		printf("Out of Memory/n");
		return;
	}

	dbus_connection_flush (conn);
	dbus_message_unref (reply);		
}
#if 1
void send_led_set_signal(char *name, int brightness,DBusConnection * conn)
{
	dbus_uint32_t serial = 0; 
	DBusMessage* msg;
	DBusMessageIter args;
	char data[50] = {0}; 
	char *respone ;
	
	sprintf(data,"%s %d",name,brightness);
	respone = &data[0];
	
	msg = dbus_message_new_signal(DBUS_SERVER_PATH, // object name of the signal
								  DBUS_SERVER_INTERFACE, // interface name of the signal
								  "sigLedBrightnessChanged"); // name of the signalprintf("1\n");
	dbus_message_iter_init_append(msg, &args);
	if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &respone)) {
		fprintf(stderr, "Out Of Memory!\n");
		return;
	}
	
	if (!dbus_connection_send(conn, msg, NULL)) {
		fprintf(stderr, "Out Of Memory!\n");
		return;
	}
	
	dbus_connection_flush(conn);
	dbus_message_unref(msg);	
}
void send_led_set_signal13(char *name, int brightness,DBusConnection * conn)
{
	dbus_uint32_t serial = 0;
	DBusMessage* msg;
	DBusMessageIter args;
	char data[50] = {0};
	char *respone ;

	sprintf(data,"%s %d",name,brightness);
	respone = &data[0];

	msg = dbus_message_new_signal(DBUS_SERVER_PATH, // object name of the signal
								  DBUS_SERVER_INTERFACE, // interface name of the signal
								  "sigLedBrightnessChanged13"); // name of the signalprintf("1\n");
	dbus_message_iter_init_append(msg, &args);
	if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &respone)) {
		fprintf(stderr, "Out Of Memory!\n");
		return;
	}

	if (!dbus_connection_send(conn, msg, NULL)) {
		fprintf(stderr, "Out Of Memory!\n");
		return;
	}

	dbus_connection_flush(conn);
	dbus_message_unref(msg);
}
#endif
void setLedBrightress_method_call(DBusMessage * msg, DBusConnection * conn)
{
	DBusMessage * reply;
	DBusMessageIter arg;
	char *respone;
	char led_list[200] = {0};
	dbus_uint32_t  serial =0; 
	int ret=0;

	char *led_name = NULL;
	int led_brightness = 0;
	
	get_led_list(led_list);
	respone = &led_list[0];
	reply = dbus_message_new_method_return(msg);
	dbus_message_iter_init_append(reply,&arg); 
	if(!dbus_message_iter_append_basic (&arg,DBUS_TYPE_STRING,&respone)){
		printf("Out of Memory!/n");
		return;
	}
	if( !dbus_connection_send (conn, reply, &serial)){
		printf("Out of Memory/n");
		return;
	}


/*signal*/
	dbus_message_iter_init(msg, &arg); 
	dbus_message_iter_get_basic (&arg, &led_name);
	dbus_message_iter_next(&arg);
	dbus_message_iter_get_basic (&arg, &led_brightness);

	dbg_printf("led_name:%s,led_brightness:%d\n",led_name,led_brightness);
	ret = set_led_brightness(led_name,led_brightness);
	if(ret == 0)
	{
		send_led_set_signal(led_name,led_brightness,conn);
	}


	dbus_connection_flush (conn);
	dbus_message_unref (reply);	
}

void setLedBrightress_method_call13(DBusMessage * msg, DBusConnection * conn)
{
	DBusMessage * reply;
	DBusMessageIter arg;
	char *respone;
	char led_list[200] = {0};
	dbus_uint32_t  serial =0;
	int ret=0;

	char *led_name = NULL;
	int led_brightness = 0;

	get_led_list(led_list);
	respone = &led_list[0];
	reply = dbus_message_new_method_return(msg);
	dbus_message_iter_init_append(reply,&arg);
	if(!dbus_message_iter_append_basic (&arg,DBUS_TYPE_STRING,&respone)){
		printf("Out of Memory!/n");
		return;
	}
	if( !dbus_connection_send (conn, reply, &serial)){
		printf("Out of Memory/n");
		return;
	}


/*signal*/
	dbus_message_iter_init(msg, &arg);
	dbus_message_iter_get_basic (&arg, &led_name);
	dbus_message_iter_next(&arg);
	dbus_message_iter_get_basic (&arg, &led_brightness);

	dbg_printf("led_name:%s,led_brightness:%d\n",led_name,led_brightness);
	ret = set_led_brightness(led_name,led_brightness);
	if(ret == 0)
	{
		send_led_set_signal13(led_name,led_brightness,conn);
	}


	dbus_connection_flush (conn);
	dbus_message_unref (reply);
}

void getLedList_method_call(DBusMessage * msg, DBusConnection * conn)
{
	DBusMessage * reply;
	DBusMessageIter arg;
	char *respone;
	char led_list[200] = {0};
	dbus_uint32_t  serial =0; 
	
	get_led_list(led_list);
	
	respone = &led_list[0];
	reply = dbus_message_new_method_return(msg);
	dbus_message_iter_init_append(reply,&arg); 
	if(!dbus_message_iter_append_basic (&arg,DBUS_TYPE_STRING,&respone)){
		printf("Out of Memory!/n");
		return;
	}
	if( !dbus_connection_send (conn, reply, &serial)){
		printf("Out of Memory/n");
		return;
	}

	dbus_connection_flush (conn);
	dbus_message_unref (reply);	
}

void listen_dbus()
{
    DBusMessage * msg;
    DBusMessageIter arg;
    DBusConnection * connection;
    DBusError err;
    int ret;
    char * sigvalue;

    dbus_error_init(&err);
    //careate session D-Bus
    connection = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if(dbus_error_is_set(&err)){
        fprintf(stderr,"Connection Error %s/n",err.message);
        dbus_error_free(&err);
    }
    if(connection == NULL)
        return;
    //set dbus service name 
    ret = dbus_bus_request_name(connection,DBUS_SERVER_NAME,DBUS_NAME_FLAG_REPLACE_EXISTING,&err);
    if(dbus_error_is_set(&err)){
        fprintf(stderr,"Name Error %s/n",err.message);
        dbus_error_free(&err);
    }
    if(ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER)
        return;

    //要求监听某个singal：来自接口test.signal.Type的信号 
    dbus_bus_add_match(connection,DBUS_SIGNAL_MATCH,&err);
    dbus_connection_flush(connection);
    if(dbus_error_is_set(&err)){
        fprintf(stderr,"Match Error %s/n",err.message);
        dbus_error_free(&err);
    } 

    while(1){ 
        dbus_connection_read_write (connection,0);
        msg = dbus_connection_pop_message (connection);

        if(msg == NULL){
            usleep(100000);
            continue;
        } 
		if(strcmp(dbus_message_get_path (msg),DBUS_SERVER_PATH) == 0)
        {
			
			if(dbus_message_is_signal(msg,DBUS_SERVER_INTERFACE,"Test")){ 
					
			}else if(dbus_message_is_method_call(msg,DBUS_SERVER_INTERFACE,"getLedList")){ 
		
				getLedList_method_call(msg,connection);
					
			}else if(dbus_message_is_method_call(msg,DBUS_SERVER_INTERFACE,"setLedBrightress")){ 
				
				setLedBrightress_method_call(msg,connection);
			}else if(dbus_message_is_method_call(msg,DBUS_SERVER_INTERFACE,"setLedBrightress13")){

				setLedBrightress_method_call13(msg,connection);

			
			}else if(dbus_message_is_method_call(msg,DBUS_SERVER_INTERFACE,"setLedTrigger")){ 
				
			}else if(dbus_message_is_method_call(msg,DBUS_SERVER_INTERFACE,"setLedDuty")){ 
			
			}else if(dbus_message_is_method_call(msg,DBUS_INTROSSPECTABLE_INTERFACE,"Introspect")){
				Introspect_method_call(msg,connection);
			}
		}
        dbus_message_unref(msg); 
    }
    
    
}
int main( int argc , char ** argv){
	
	led_init();
    listen_dbus(); 
    return 0;
}
