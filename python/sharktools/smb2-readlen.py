#!/usr/bin/env python

import pyshark

if __name__ == '__main__':
    f = pyshark.read('/home/coffeedude/Desktop/Ubuntu-Boot-1.pcap', ['smb2.read_length'], 'smb2 && smb2.cmd == 8 && smb2.flags.response == 1')
    b = list(f)
    print "Number of SMB2_READ requests == %s" % (len(b))
    lengths = {}
    for i in range(len(b)):
        if list == type(b[i]['smb2.read_length']):
            for j in range(len(b[i]['smb2.read_length'])):
                if str(b[i]['smb2.read_length'][j]) in lengths:
                    lengths[str(b[i]['smb2.read_length'][j])] += 1
                else:
                    lengths[str(b[i]['smb2.read_length'][j])] = 1
        else:
            if str(b[i]['smb2.read_length']) in lengths:
                lengths[str(b[i]['smb2.read_length'])] += 1
            else:
                lengths[str(b[i]['smb2.read_length'])] = 1

    key_list = [int(x) for x in lengths.keys()]
    for k in sorted(key_list):
       print "%s, %s" % (k, lengths[str(k)])

