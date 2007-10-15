#!/bin/bash

for rev in `seq 1 25600`; do
	svn log -r$rev --incremental file:///data/src/mirror/svn/samba | ./fix-svn-log.pl > $rev
	if [ ! -s $rev ]; then
		/bin/rm $rev
	else
		echo "r$rev"
		svnadmin setlog --bypass-hooks /data/src/mirror/svn/samba -r$rev $rev
	fi
done
