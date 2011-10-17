#!/bin/bash

git filter-branch --env-filter '
    author_email=`echo "$GIT_AUTHOR_NAME <$GIT_AUTHOR_EMAIL>" | $GIT_DIR/../../author.pl`;
    GIT_AUTHOR_NAME=`echo "$author_email" | cut -d: -f1`;
    GIT_AUTHOR_EMAIL=`echo "$author_email" | cut -d: -f2`;
    export GIT_AUTHOR_NAME GIT_AUTHOR_EMAIL
' HEAD

