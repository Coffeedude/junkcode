#!/bin/sh

for d in *; do 
	if [ -d $d/.git ]; then
		echo "Publishing $d..."
		(cd $d && git-push ssh://jerry@srv3.corp.centeris.com/home/srv3/jerry/public_html/git/lwi-trunk/$d master)
	fi
done
