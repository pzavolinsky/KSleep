#!/bin/sh
APP=`qdbus org.kde.ksleep-* | head -1`
DBUS_PATH=/MainApplication
DBUS_ADAPTOR=com.ksleep.DBus.MainApplication

if [ "1$APP" = "1" ]; then
	echo "$0 error: KSleep is not running!"
	exit 1
fi

function setTimeout() # $1 = minutes
{
	qdbus $APP $DBUS_PATH $DBUS_ADAPTOR.setTimeout $1
}

function stop()
{
	qdbus $APP $DBUS_PATH $DBUS_ADAPTOR.stop
}

function halt()
{
	qdbus $APP $DBUS_PATH $DBUS_ADAPTOR.halt
}

function usage()
{
	echo "usage: $0 setTimeout minutes"
	echo "or:    $0 stop"
	exit 1
}

if [ $# -eq 0 ]; then usage; fi

case $1 in
	"stop")       if [ $# -ne 1 ]; then usage; fi; stop;;
	"halt")       if [ $# -ne 1 ]; then usage; fi; halt;;
	"setTimeout") if [ $# -ne 2 ]; then usage; fi; setTimeout $2;;
	*) usage;;
esac
