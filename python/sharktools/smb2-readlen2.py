#!/usr/bin/env python

import sys
import pyshark


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print >> sys.stderr, "usage: %s <pcap filename>" % (sys.argv[0])
        sys.exit(1)

    print "Opening file: %s" % (sys.argv[1])

    f = pyshark.read(
            sys.argv[1],
            ['smb2.read_length',
             'smb2.write_length',
             'smb2.file_offet', ],
            'smb2 && ((smb2.cmd == 8) || (smb2.cmd == 9)) && smb2.flags.response == 1')

    b = list(f)
    rlengths = {}
    wlengths = {}
    for i in range(len(b)):
        if 'smb2.read_length' in b[i]:
            if list == type(b[i]['smb2.read_length']):
                for j in range(len(b[i]['smb2.read_length'])):
                    if str(b[i]['smb2.read_length'][j]) in rlengths:
                        rlengths[str(b[i]['smb2.read_length'][j])] += 1
                    else:
                        rlengths[str(b[i]['smb2.read_length'][j])] = 1
            else:
                if str(b[i]['smb2.read_length']) in rlengths:
                    rlengths[str(b[i]['smb2.read_length'])] += 1
                else:
                    rlengths[str(b[i]['smb2.read_length'])] = 1

        elif 'smb2.write_length' in b[i]:
            if list == type(b[i]['smb2.write_length']):
                for j in range(len(b[i]['smb2.write_length'])):
                    if str(b[i]['smb2.write_length'][j]) in wlengths:
                        wlengths[str(b[i]['smb2.write_length'][j])] += 1
                    else:
                        wlengths[str(b[i]['smb2.write_length'][j])] = 1
            else:
                if str(b[i]['smb2.write_length']) in wlengths:
                    wlengths[str(b[i]['smb2.write_length'])] += 1
                else:
                    wlengths[str(b[i]['smb2.write_length'])] = 1

    key_list = [int(x) for x in rlengths.keys()]
    print >> sys.stdout, "SMB2_COM_READ:"
    for k in sorted(key_list):
       print "%s, %s" % (k, rlengths[str(k)])

    print >> sys.stdout, "SMB2_COM_WRITE:"
    key_list = [int(x) for x in wlengths.keys()]
    for k in sorted(key_list):
       print "%s, %s" % (k, wlengths[str(k)])

