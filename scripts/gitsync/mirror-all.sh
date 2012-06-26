#!/bin/bash

PATH=$HOME/bin:$PATH
SCRIPTDIR="$HOME/cron/svn-mirror-git/scripts"
export PATH SCRIPTDIR

echo " "
echo "============================================================================="
echo "Starting synchronization of external repositories @ `date`"

## 
## TL-Support
##
${SCRIPTDIR}/mirror.sh hp oem-hp tl-support/likewise-oem-hp
${SCRIPTDIR}/mirror.sh oem-platform-6.2 master tl-support/Platform-6.2
${SCRIPTDIR}/mirror.sh oem-platform-6.4 master tl-support/Platform-6.4


##
## EMC
##
#${SCRIPTDIR}/mirror.sh likewise-platform platform emc/Platform
#${SCRIPTDIR}/mirror.sh likewise-platform-6-2 master emc/Platform-6.2
#${SCRIPTDIR}/mirror.sh likewise-platform-6-4 master emc/Platform-6.4

#${SCRIPTDIR}/mirror.sh lwidentity-5.3-datadomain mirror/lw-oem-datadomain emc/lwidentity-5.3-datadomain

## Internal only
#${SCRIPTDIR}/mirror2.sh build-tools-main master emc/build-tools-git-migration
#${SCRIPTDIR}/mirror2.sh likewise-main master emc/Platform-git-migration
#${SCRIPTDIR}/mirror2.sh likewise-main-v6.2 master emc/Platform-git-migration-v6.2
#${SCRIPTDIR}/mirror2.sh likewise-main-v6.4 master emc/Platform-git-migration-v6.4

# ${SCRIPTDIR}/mirror2.sh linus master emc/Linus-git-migration

#(cd ${SCRIPTDIR}/../git/likewise-main-dev && \
#     git fetch main && \
#     git checkout master && \
#     git merge main/master && \
#     git push origin master)

#${SCRIPTDIR}/mirror2.sh emc-trunk master emc/emc-trunk
#${SCRIPTDIR}/mirror2.sh emc-v6.2 v6.2 emc/emc-v6.2
#${SCRIPTDIR}/mirror2.sh emc-v6.4 v6.4 emc/emc-v6.4


echo "Synchronization complete @ `date`"
