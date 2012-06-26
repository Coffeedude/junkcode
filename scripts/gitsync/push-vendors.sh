#!/bin/bash

PATH=$HOME/bin:$PATH
SCRIPTDIR="$HOME/cron/svn-mirror-git/scripts"

export PATH SCRIPTDIR

##
## EMC
##
(cd ${SCRIPTDIR}/../git/emc/likewise-platform-6-2 && git push isilon && git push --tags isilon)
(cd ${SCRIPTDIR}/../git/emc/likewise-platform-6-4 && git push isilon && git push --tags isilon)
(cd ${SCRIPTDIR}/../git/emc/likewise-platform-6-4 && git push datadomain && git push --tags datadomain)

##
## TL-SUPPORT
##
(cd ${SCRIPTDIR}/../git/tl-support/likewise-platform-6-2 && git push dell && git push --tags dell)
(cd ${SCRIPTDIR}/../git/tl-support/likewise-platform-6-2 && git push riverbed && git push --tags riverbed)
(cd ${SCRIPTDIR}/../git/tl-support/likewise-platform-6-2 && git push filetek && git push --tags filetek)

(cd ${SCRIPTDIR}/../git/tl-support/likewise-platform-6-4 && git push dell && git push --tags dell)
(cd ${SCRIPTDIR}/../git/tl-support/likewise-platform-6-4 && git push hp && git push --tags hp)
(cd ${SCRIPTDIR}/../git/tl-support/likewise-platform-6-4 && git push filetek && git push --tags filetek)

(cd ${SCRIPTDIR}/../git/tl-support/panasas-v6.4 && \
	git fetch v6-4-main && \
	git merge v6-4-main/master && \
	git push vendor && \
	git push --tags vendor)

