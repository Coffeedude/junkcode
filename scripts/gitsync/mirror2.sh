#!/bin/sh

umask 022

PATH=/usr/local/bin:/bin:/usr/bin:/sbin:/usr/sbin
export PATH

if [ -z "$1" -o -z "$2" -o -z "$3" ]; then
    echo "Usage: $0 <gitdir> <branch name> <svndir>"
    exit 1
fi

GITDIR="$1"
BRANCHNAME="$2"
SVNDIR="$3"

TOP_DIR="$HOME/cron/svn-mirror-git"
LOCKFILE="${TOP_DIR}/${GITDIR}.lock"

# number of seconds since last mod
age_modification() {
    fname="$1"
    tnow=`date +%s`
    tmod=`stat -c %Y $fname`
    echo `expr $tnow - $tmod`
}

############################
# grab a lock file. Not atomic, but close :)
# tries to cope with NFS
lock_file() {
        lck="$1"
        machine=`cat "$lck" 2> /dev/null | cut -d: -f1`
        pid=`cat "$lck" 2> /dev/null | cut -d: -f2`

        if [ -f "$lck" ] && 
            ( [ $machine != $host ] || kill -0 $pid ) 2> /dev/null; then
                age=`age_modification $lck`
                [ $age -lt 1800 ] && {
                       # Don't bother admins with this
                       exit 0
                }
                echo "lock file $lck is valid for $machine:$pid - age of lock file is $age seconds"
                return 1
        fi
        /bin/rm -f "$lck"
        echo "$host:$$" > "$lck"  || exit 1
        return 0
}

############################
# unlock a lock file
unlock_file() {
        lck="$1"
        /bin/rm -f "$lck"
}

update_git() {
    _gitdir="$1"
    _gitbranch="$2"
    _svndir="$3"

    if [ -z "${_gitdir}" -o -z "${_gitbranch}" -o -z "${_svndir}" ]; then
	echo "Invalid git or svn directory values!"
	return 1
    fi

    if [ ! -d "${_gitdir}" -o ! -d "${_svndir}" ]; then
	echo "Invalid directories!"
	return 1
    fi

    echo "Updating ${_gitdir} (${_gitbranch}) from ${_svndir}"

    (cd ${_gitdir} && \
	git checkout "${_gitbranch}" && \
	git clean -dxf) || exit 1
    (cd ${_svndir} && \
	${TOP_DIR}/scripts/update2.sh && \
	${TOP_DIR}/scripts/patch-main-repo2.sh `basename ${_gitdir}`) || exit 1
}

#############################
## Main()
##
if [ -f "${LOCKFILE}" ];then
	echo "Previous lock file found.  Exit..."
	exit 1
fi

lock_file "${LOCKFILE}"

update_git "${TOP_DIR}/git/${GITDIR}" "${BRANCHNAME}" "${TOP_DIR}/svn/${SVNDIR}" || exit 1
(cd "${TOP_DIR}/git/${GITDIR}" && [ -n "`git remote | grep internal`" ] && git push internal)
(cd "${TOP_DIR}/git/${GITDIR}" && [ -n "`git remote | grep vendor`" ] && git push vendor)

unlock_file "${LOCKFILE}"
