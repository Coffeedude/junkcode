#!/bin/bash

SCRIPTDIR="$HOME/cron/svn-mirror-git/scripts"

## Active mirrors
${SCRIPTDIR}/mirror.sh  likewise-platform platform 		Platform
${SCRIPTDIR}/mirror.sh  likewise-oem-6-0  platform-oem-v6-0 	Platform-CIFS-6-0
${SCRIPTDIR}/mirror.sh  likewise-oem-6-1  platform-oem-6-1 	Platform-OEM-6.1
${SCRIPTDIR}/mirror.sh  likewise-platform-6-2 master Platform-6.2
${SCRIPTDIR}/mirror.sh  likewise-platform-6-3 master Platform-6.3
${SCRIPTDIR}/mirror.sh  likewise-platform-6-4 master Platform-6.4
