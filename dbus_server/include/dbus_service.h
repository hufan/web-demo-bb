#ifndef DBUS_SERVICE_H
#define DBUS_SERVICE_H

#define DBUS_SERVER_NAME "com.myirtech.mxde"
#define DBUS_SERVER_PATH "/com/myirtech/mxde"
#define DBUS_SERVER_INTERFACE "com.myirtech.mxde.MxdeInterface"
#define DBUS_INTROSSPECTABLE_INTERFACE "org.freedesktop.DBus.Introspectable"
#define DBUS_SIGNAL_MATCH "type='signal',interface='com.myirtech.mxde.MxdeInterface'"


#define DEBUG 1

#ifdef DEBUG
#define dbg_printf(fmt, args...) printf(fmt, ##args)
#define dbg_perror(msg) (perror(msg))
#else
#define dbg_printf(fmt, args...)
#define dbg_perror(msg)
#endif
#endif