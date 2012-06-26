#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 <git repo>"
    exit 1
fi

BASE_DIR="$HOME/cron/svn-mirror-git"
LWISO_HOME="${BASE_DIR}/git/$1"
PATCHDIR="${PWD}/patches"

if [ ! -d "${LWISO_HOME}" ]; then
    echo "${LWISO_HOME} is not a directory!  Exiting..."
    exit 1
fi

echo "Using BASE_DIR = ${BASE_DIR}"
for dir in ${PATCHDIR}/*; do
    _files=`ls ${dir}/*patch 2> /dev/null | sort`
    if [ -n "${_files}" ]; then
	echo "Applying patches from `basename ${dir}`..."
	for p in ${_files}; do
	    (cd ${LWISO_HOME} && git clean -dxf && (git am -3 < $p || exit 1)) || exit 1
	done
	echo " "
    fi
done
