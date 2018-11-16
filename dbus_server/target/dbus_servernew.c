#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dbus/dbus.h>
#include <unistd.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>

#include "led.h"
#include "dbus_server.h"
#include "tty.h"
#include "can.h"

void tt_add(DBusMessageIter arg,DBusMessage *msg, DBusConnection *conn)
{       
        dbus_uint32_t  serial =0;
        DBusMessage * reply; 
        char led_list[200] = {0};
        char *respone;
        //get_led_list(led_list);
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

}

//serial start
void getSerialList_method_call(DBusMessage * msg, DBusConnection * conn)
{
    DBusMessage * reply;
    DBusMessageIter arg;
    char *respone;
    char data[60];

    get_serial_list(data);
    respone = &data[0];

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
void setSerialPort_method_call(DBusMessage * msg, DBusConnection * conn)
{
    DBusMessageIter arg;
    char *tty_param;

    dbus_server_conn = conn;
    dbus_message_iter_init(msg, &arg);
    dbus_message_iter_get_basic (&arg, &tty_param);

	printf("tttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt\n");
    
    parse_tty_param(tty_param);

	tt_add(arg,msg,conn);
/*
	 dbus_uint32_t  serial =0;
        DBusMessage * reply;
        char led_list[200] = {0};
        char *respone;
        //get_led_list(led_list);
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
*/
    dbus_connection_flush (conn);
    dbus_message_unref (msg);

}

void closeSerialPort_method_call(DBusMessage * msg, DBusConnection * conn)
{
    DBusMessageIter arg;

    int  fd;

    dbus_message_iter_init(msg, &arg);
    dbus_message_iter_get_basic (&arg, &fd);


    delete_tty_read_thread();
    tty_close(fd);

    tt_add(arg,msg,conn);

    dbus_connection_flush (conn);
    dbus_message_unref (msg);
}


void openSerialPort_method_call(DBusMessage * msg, DBusConnection * conn)
{
    DBusMessageIter arg;
    DBusMessage * reply;
    char *dev_name = NULL;
    int fd;

    dbus_message_iter_init(msg, &arg);
    dbus_message_iter_get_basic (&arg, &dev_name);

    dbg_printf("serial_name:%s\n",dev_name);
    fd = tty_open(dev_name);

    reply = dbus_message_new_method_return(msg);
    dbus_message_iter_init_append(reply,&arg);
    if(!dbus_message_iter_append_basic (&arg,DBUS_TYPE_INT32,&fd)){
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
void SerialWrite_method_call(DBusMessage * msg, DBusConnection * conn)
{
    DBusMessageIter arg;
    int fd,len;
    char *data;

    dbus_message_iter_init(msg, &arg);
    dbus_message_iter_get_basic (&arg, &fd);

    dbus_message_iter_next(&arg);
    dbus_message_iter_get_basic (&arg, &data);

    dbus_message_iter_next(&arg);
    dbus_message_iter_get_basic (&arg, &len);

    tty_write(fd ,data , len);

    tt_add(arg,msg,conn);
    dbus_connection_flush(conn);
    dbus_message_unref(msg);

}
//serial end
//rs485 start
void getRs485List_method_call(DBusMessage * msg, DBusConnection * conn)
{
    DBusMessage * reply;
    DBusMessageIter arg;
    char *respone;
    char data[60];

    get_rs485_list(data);
    respone = &data[0];

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
//rs485 end
//can start
void getCanList_method_call(DBusMessage * msg, DBusConnection * conn)
{
    DBusMessage * reply;
    DBusMessageIter arg;
    char *respone;
    char data[60];

    get_can_list(data);
    respone = &data[0];

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
void openCanPort_method_call(DBusMessage * msg, DBusConnection * conn)
{
    DBusMessage * reply;
    DBusMessageIter arg;
    char *can_name;
    int can_fd = 0;

    dbus_message_iter_init(msg, &arg);
    dbus_message_iter_get_basic (&arg, &can_name);

    can_fd = can_init(can_name);

    dbus_server_conn = conn;

    create_can_read_thread();

    reply = dbus_message_new_method_return(msg);
    dbus_message_iter_init_append(reply,&arg);
    if(!dbus_message_iter_append_basic (&arg,DBUS_TYPE_INT32,&can_fd)){
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
void closeCanPort_method_call(DBusMessage * msg, DBusConnection * conn)
{
    DBusMessage * reply;
    DBusMessageIter arg;
    char *can_name;
    int can_fd;

    dbus_message_iter_init(msg, &arg);
    dbus_message_iter_get_basic (&arg, &can_name);

    dbus_message_iter_next(&arg);
    dbus_message_iter_get_basic (&arg, &can_fd);

    delete_can_read_thread();
    dbg_printf("close:%s,%d\n",can_name,can_fd);
    close_can_port(can_name,can_fd);


    tt_add(arg,msg,conn);
    dbus_connection_flush (conn);
    dbus_message_unref (msg);
}
void setCanPort_method_call(DBusMessage * msg, DBusConnection * conn)
{
    DBusMessageIter arg;
    DBusMessage * reply;
    char *can_name,*loop;
    int bitrate, status,ret ;


    dbus_message_iter_init(msg, &arg);
    dbus_message_iter_get_basic (&arg, &can_name);

    dbus_message_iter_next(&arg);
    dbus_message_iter_get_basic (&arg, &bitrate);

    dbus_message_iter_next(&arg);
    dbus_message_iter_get_basic (&arg, &status);

    dbus_message_iter_next(&arg);
    dbus_message_iter_get_basic (&arg, &loop);

    dbg_printf("setting:%s,%d,%d\n",can_name,bitrate,status);
    ret  = can_setting(can_name , bitrate, status,loop);

    reply = dbus_message_new_method_return(msg);
    dbus_message_iter_init_append(reply,&arg);
    if(!dbus_message_iter_append_basic (&arg,DBUS_TYPE_INT32,&ret)){
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

void CanWrite_method_call(DBusMessage * msg, DBusConnection * conn)
{
    DBusMessageIter arg;
    int can_fd,len;
    char *data;
    dbus_message_iter_init(msg, &arg);
    dbus_message_iter_get_basic (&arg, &can_fd);

    dbus_message_iter_next(&arg);
    dbus_message_iter_get_basic (&arg, &data);

    dbus_message_iter_next(&arg);
    dbus_message_iter_get_basic (&arg, &len);

    can_data_write(can_fd, data);

    tt_add(arg,msg,conn);
    dbus_connection_flush (conn);
    dbus_message_unref (msg);
}
//can end
void Introspect_method_call(DBusMessage * msg, DBusConnection * conn)
{
#if 1
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
#endif
}

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

void setLedBrightress_method_call(DBusMessage * msg, DBusConnection * conn)
{
	DBusMessageIter arg;
	char *led_name = NULL;
	int led_brightness = 0;
	int ret = 0;

	dbus_message_iter_init(msg, &arg); 
	dbus_message_iter_get_basic (&arg, &led_name);
	
	dbus_message_iter_next(&arg);
	dbus_message_iter_get_basic (&arg, &led_brightness);
/*
	dbus_uint32_t  serial =0;
	DBusMessage * reply;
        //get_led_list(led_list);
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
*/	
	tt_add(arg,msg,conn);	

	
	dbg_printf("led_name:%s,led_brightness:%d\n",led_name,led_brightness);
	ret = set_led_brightness(led_name,led_brightness);
	if(ret == 0)
	{
		send_led_set_signal(led_name,led_brightness,conn);
	}
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

    //listen signal
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
				
            }else if(dbus_message_is_method_call(msg,DBUS_SERVER_INTERFACE,"openSerialPort")){

                openSerialPort_method_call(msg,connection);

            }else if(dbus_message_is_method_call(msg,DBUS_SERVER_INTERFACE,"setSerialPort")){

                setSerialPort_method_call(msg,connection);

            }else if(dbus_message_is_method_call(msg,DBUS_SERVER_INTERFACE,"getSerialList")){

                getSerialList_method_call(msg,connection);

            }else if(dbus_message_is_method_call(msg,DBUS_SERVER_INTERFACE,"closeSerialPort")){

                closeSerialPort_method_call(msg,connection);

            }else if(dbus_message_is_method_call(msg,DBUS_SERVER_INTERFACE,"SerialWrite")){

                SerialWrite_method_call(msg,connection);

            }else if(dbus_message_is_method_call(msg,DBUS_SERVER_INTERFACE,"getRs485List")){

                getRs485List_method_call(msg,connection);

            }else if(dbus_message_is_method_call(msg,DBUS_SERVER_INTERFACE,"getCanList")){

                getCanList_method_call(msg,connection);

            }else if(dbus_message_is_method_call(msg,DBUS_SERVER_INTERFACE,"openCanPort")){

                openCanPort_method_call(msg,connection);

            }else if(dbus_message_is_method_call(msg,DBUS_SERVER_INTERFACE,"closeCanPort")){

                closeCanPort_method_call(msg,connection);

            }else if(dbus_message_is_method_call(msg,DBUS_SERVER_INTERFACE,"setCanPort")){

                setCanPort_method_call(msg,connection);

            }else if(dbus_message_is_method_call(msg,DBUS_SERVER_INTERFACE,"CanWrite")){

                CanWrite_method_call(msg,connection);

            }else if(dbus_message_is_method_call(msg,DBUS_SERVER_INTERFACE,"xxxxx")){

                //closeSerialPort_method_call(msg,connection);

            }else if(dbus_message_is_method_call(msg,DBUS_SERVER_INTERFACE,"xxxxx")){

                //closeSerialPort_method_call(msg,connection);

            }else if(dbus_message_is_method_call(msg,DBUS_SERVER_INTERFACE,"xxxxx")){

                //closeSerialPort_method_call(msg,connection);

            }else if(dbus_message_is_method_call(msg,DBUS_SERVER_INTERFACE,"xxxxx")){

                //closeSerialPort_method_call(msg,connection);

            }else if(dbus_message_is_method_call(msg,DBUS_INTROSSPECTABLE_INTERFACE,"Introspect")){
				Introspect_method_call(msg,connection);
			}
		}
        //dbus_message_unref(msg);
    }
    
    
}
int main( int argc , char ** argv){
	
	led_init();

    listen_dbus(); 
    return 0;
}


