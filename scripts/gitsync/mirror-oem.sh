#!/bin/bash

SCRIPTDIR="$HOME/cron/svn-mirror-git/scripts"

${SCRIPTDIR}/mirror.sh datadomain-auth     mirror/lw-oem-datadomain       	LWISO-Oem-DataDomain
# ${SCRIPTDIR}/mirror.sh datadomain-cifs-6-1 v6-1-oem-dd-cifs   Oem-DD-CIFS-6-1
${SCRIPTDIR}/mirror.sh hp       oem-hp                 OEM-HP
## ${SCRIPTDIR}/mirror.sh isilon   mirror/lw-oem-isilon   LWISO-Oem-Isilon
## ${SCRIPTDIR}/mirror.sh filetek2 oem-filetek-6-1	Oem-Filetek
## ${SCRIPTDIR}/mirror.sh dell     oem-dell               OEM-Dell
## ${SCRIPTDIR}/mirror.sh riverbed oem-riverbed           OEM-Riverbed
## ${SCRIPTDIR}/mirror.sh riverbed-cifs-6-1 master likewise-oem-riverbed-cifs-6-1

