#!/bin/bash
if [[ $UID -ne 0 ]]; then
	echo "$0 must be run as root"
   	exit 1
fi

exec ./configure.sh
make install
( crontab -l 2>/dev/null | grep -Fv ntpdate ; printf -- "*/10 * * * * /opt/bin/monitor" ) | crontab

