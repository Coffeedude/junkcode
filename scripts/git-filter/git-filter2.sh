#!/bin/bash

PRUNE=""

INDEXFILTER="git rm -r --cached --ignore-unmatch ${PRUNE}"
MSGFILTER="sed -e 's/likewise-oem-isilon/likewise-oem/g'"

git filter-branch --msg-filter "${MSGFILTER}" --prune-empty HEAD && \
   git for-each-ref --format="%(refname)" refs/original/ | xargs -n 1 git update-ref -d && \
   git reflog expire --expire=now --all && \
   git reset --hard

