#!/bin/sh

LASTINTERVAL=~/.ksleep/last_interval.conf
KSLEEP_SH=~/.ksleep/ksleep.sh

sudo shutdown -c  2> /dev/null > /dev/null

function setTimer()
{
	echo "$1" > $LASTINTERVAL
	$KSLEEP_SH setTimeout $1
	exit
}

function stopTimer()
{
	echo "0" > $LASTINTERVAL
	$KSLEEP_SH stop
	exit
}

if [ -r $LASTINTERVAL ]; then 
	TIMEOUT=`cat $LASTINTERVAL`
else
	TIMEOUT="0"
fi

if [ "$TIMEOUT" ==  "15" ]; then setTimer  "30"; fi
if [ "$TIMEOUT" ==  "30" ]; then setTimer  "45"; fi
if [ "$TIMEOUT" ==  "45" ]; then setTimer  "60"; fi
if [ "$TIMEOUT" ==  "60" ]; then setTimer  "90"; fi
if [ "$TIMEOUT" ==  "90" ]; then setTimer "120"; fi
if [ "$TIMEOUT" == "120" ]; then setTimer "180"; fi
if [ "$TIMEOUT" == "180" ]; then setTimer "240"; fi
if [ "$TIMEOUT" == "240" ]; then stopTimer     ; fi

setTimer  "15"
