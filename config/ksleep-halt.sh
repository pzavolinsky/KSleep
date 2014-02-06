#!/bin/sh

IFS="
"

LASTINTERVAL=~/.ksleep/last_interval.conf
echo "0" > $LASTINTERVAL

VBOX=$(which VBoxManage)
if [ ! -z "$VBOX" ]; then
	for vm in $($VBOX list runningvms | sed -ne 's/.*\({.*}\)/\1/p')
	do
		$VBOX controlvm "$vm" savestate
	done
fi
#echo "halt" >> ~/ksleep.log
sudo shutdown -h +1
