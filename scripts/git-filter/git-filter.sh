#!/bin/bash

PRUNE=""

INDEXFILTER="git rm -r --cached --ignore-unmatch ${PRUNE}"
MSGFILTER="sed -e 's/^\s*git-svn-id:.*\@/revision: /g' -e 's/ 5513777b-1b0e-0410-a91d-ed1dec569d5e//g'"

#git filter-branch --index-filter "${INDEXFILTER}" --msg-filter "${MSGFILTER}" --prune-empty HEAD && \
git filter-branch --env-filter '
    author_email=`echo "$GIT_AUTHOR_NAME <$GIT_AUTHOR_EMAIL>" | $GIT_DIR/../../author.pl`;
    GIT_AUTHOR_NAME=`echo "$author_email" | cut -d: -f1`;
    GIT_AUTHOR_EMAIL=`echo "$author_email" | cut -d: -f2`;
    export GIT_AUTHOR_NAME GIT_AUTHOR_EMAIL
' --msg-filter "${MSGFILTER}" --prune-empty HEAD && \
   git for-each-ref --format="%(refname)" refs/original/ | xargs -n 1 git update-ref -d && \
   git reflog expire --expire=now --all && \
   git reset --hard

