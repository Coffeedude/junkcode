#!/bin/bash

PRUNE=""

INDEXFILTER="git rm -r --cached --ignore-unmatch ${PRUNE}"
MSGFILTER="sed -e 's/^\s*git-svn-id:.*\@/revision: /g' -e 's/ f1991e19-cb36-4043-9cec-1ea08b1aba44//g'"

#git filter-branch --index-filter "${INDEXFILTER}" --msg-filter "${MSGFILTER}" --prune-empty HEAD && \
git filter-branch --env-filter '
    printf "%s\n" $GIT_AUTHOR_NAME;
    author_email="`printf \"%s <%s>\" $GIT_AUTHOR_NAME $GIT_AUTHOR_EMAIL | $GIT_DIR/../../author2.pl`";
    GIT_AUTHOR_NAME=`echo "$author_email" | cut -d: -f1`;
    GIT_AUTHOR_EMAIL=`echo "$author_email" | cut -d: -f2`;
    GIT_COMMITTER_NAME=${GIT_AUTHOR_NAME};
    GIT_COMMITTER_EMAIL=${GIT_AUTHOR_EMAIL};
    export GIT_AUTHOR_NAME GIT_AUTHOR_EMAIL GIT_COMMITTER_NAME GIT_COMMITTER_EMAIL
' --msg-filter "${MSGFILTER}" --prune-empty HEAD && \
   git for-each-ref --format="%(refname)" refs/original/ | xargs -n 1 git update-ref -d && \
   git reflog expire --expire=now --all && \
   git reset --hard

