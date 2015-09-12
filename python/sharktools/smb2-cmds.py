#!/usr/bin/env python

import pyshark

smb2_cmds = [ 'Negotiate',
              'SessionSetup',
              'Logoff',
              'TreeConnect',
              'TreeDisconnect',
              'Create',
              'Close',
              'Flush',
              'Read',
              'Write',
              'Lock',
              'IoCtl',
              'Cancel',
              'Echo',
              'QueryDirectory',
              'ChangeNotify',
              'QueryInfo',
              'SetInfo',
              'OplockBreak' ]

f = pyshark.read('/home/coffeedude/Desktop/Ubuntu-Boot-1.pcap', ['smb2.cmd'], 'smb2 && smb2.flags.response == 0')
b = list(f)
lengths = {}
for i in range(len(b)):
    if list == type(b[i]['smb2.cmd']):
        for j in range(len(b[i]['smb2.cmd'])):
            if str(b[i]['smb2.cmd'][j]) in lengths:
                lengths[str(b[i]['smb2.cmd'][j])] += 1
            else:
                lengths[str(b[i]['smb2.cmd'][j])] = 1
    else:
        if str(b[i]['smb2.cmd']) in lengths:
            lengths[str(b[i]['smb2.cmd'])] += 1
        else:
            lengths[str(b[i]['smb2.cmd'])] = 1

key_list = [int(x) for x in lengths.keys()]
for k in sorted(key_list):
    print "{0:>15} => {1}".format(smb2_cmds[k], lengths[str(k)])

