#!/bin/bash

#BASE_URL="svn+ssh://srv3.corp.centeris.com/home/srv3/svn/repo/active/dev/branches/Platform-6.4/src/linux"
BASE_URL="svn+ssh://srv3.corp.centeris.com/home/srv3/svn/repo/active/dev/trunk/Platform/src/linux"

# [ -z "$1" ] && exit

for dir in *; do
	URL="${BASE_URL}/${dir}"
	echo "Initializing ${dir} mirror from ${URL}"
	cd ${dir} || exit 1
	REVNUM="71754"
	REV=`svn log ${URL}@${REVNUM} 2> /dev/null | head -1000 | grep ^r[1-9] | head -1 | awk '{print $1}' | sed 's/^r//'`
	echo "Fetching SVN r${REV}"
	git svn init ${URL} && \
            git config svn-remote.svn.followparent false && \
	    git config whitespace.apply strip && \
	    git svn fetch -r${REV} 
	cd ..
done

