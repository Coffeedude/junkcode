#!/bin/bash

USERNAME=`echo "$1" | sed -e 's/^.*\\\\//'`

/usr/bin/osascript -e "
tell application \"Finder\"
	set simple_name to \"$USERNAME\"
	open location \"cifs://elm.ad.plainjoe.org/\" & simple_name
end tell"

