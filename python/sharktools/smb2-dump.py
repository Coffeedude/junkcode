#!/usr/bin/env python

import sys
import pyshark
import smb2


def parse_smb2_frame(Frame):
    result = []

    if list == type(Frame['smb2.cmd']):
        for i in range(len(Frame['smb2.cmd'])):
            result.append(
                smb2.Header(
                    Frame['frame.number'],
                    Frame['smb2.cmd'][i],
                    Frame['smb2.credit.charge'][i],
                    Frame['smb2.tid'][i],
                    Frame['smb2.sesid'][i],
                    Frame['smb2.seq_num'][i],
                    0,
                    Frame['smb2.flags.response'][i]))

        return result

    if Frame['smb2.cmd'] == smb2.Cmd.SMB2_CANCEL:
        return result

    if Frame['smb2.flags.response']:
        status = Frame['smb2.nt_status']

        if Frame['smb2.flags.async']:
            tid = -1
        else:
            tid = Frame['smb2.tid']
    else:
        status = 0
        tid = Frame['smb2.tid']

    result.append(
        smb2.Header(
            Frame['frame.number'],
            Frame['smb2.cmd'],
            Frame['smb2.credit.charge'],
            tid,
            Frame['smb2.sesid'],
            Frame['smb2.seq_num'],
            status,
            Frame['smb2.flags.response']))

    return result


fields = [
    'frame.number',
    'frame.time',
    'smb2.cmd',
    'smb2.credit.charge',
    'smb2.seq_num',
    'smb2.aid',
    'smb2.tid',
    'smb2.sesid',
    'smb2.nt_status',
    'smb2.flags.response',
    'smb2.flags.async',
    'smb2.response_in',
    'smb2.response_to'
    ]


if len(sys.argv) < 2:
    print >> sys.stderr, "usage: %s <pcap filename>" % (sys.argv[0])
    sys.exit(1)

pcap_file = pyshark.read(
            sys.argv[1],
            fields,
            'smb2')

smb2_ops = []
for frame in pcap_file:
    print frame
