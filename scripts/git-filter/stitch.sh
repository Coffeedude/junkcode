#!/bin/bash

git filter-branch -d /run/tmp/git-filter-$$  -- 360fbf57680d4830c371d3cd9bcfd3a5493e7f84~1..HEAD && \
git for-each-ref --format="%(refname)" refs/original/ | xargs -n 1 git update-ref -d && git reflog expire --expire=now --all && git reset --hard

