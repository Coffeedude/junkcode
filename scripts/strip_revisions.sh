#!/bin/sh

cat $1 | grep ^r[1-9] | grep -v CVSIN | awk '{print $1}' | sed 's/^r//' > $1.revisions

