#!/bin/bash

BASE_DIR="$HOME/cron/svn-mirror-git"

if [ -n "$1" ]; then
    DIRS="$1"
else
    DIRS=`/bin/ls | sort`
fi

PATCHDIR="./patches"

if [ ! -d "${PATCHDIR}" ]; then
    mkdir -p ${PATCHDIR} || exit 1
fi

for dir in ${DIRS}; do
    _revhash=""

    ## Skip any non-git directories
    if [ ! -d ${dir}/.git ]; then
	continue
    fi

    echo "Updating ${dir}...."

    ## Clean up any old stuff
    (cd ${dir} && git clean -d -x -f)

    _revhash=`(cd ${dir} && git log --pretty=one | head -1 | awk '{print $1}')`
    if [ -z "${_revhash}" ]; then
	echo "Unable to determine the last commit in \"${dir}\"!"
	exit 2
    fi
    
    ## update the svn mirror now
    cd ${dir} || exit 1
    git svn fetch || exit 1
    git svn rebase || exit 1
    git format-patch ${_revhash}
    cd ..

    if [ -d "${PATCHDIR}/${dir}" ]; then
        /bin/rm -rf "${PATCHDIR}/${dir}"
    fi
    ## do we have any patches?
    _files=`ls ${dir}/0*.patch 2> /dev/null`
    if [ -n "${_files}" ]; then
	mkdir -p "${PATCHDIR}/${dir}" || exit 2

	for p in ${_files}; do
	    ${BASE_DIR}/scripts/sanitize2.pl $dir $p > ${PATCHDIR}/${dir}/`basename $p`
	done
    fi

    echo " "
done
