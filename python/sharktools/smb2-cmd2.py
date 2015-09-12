#!/usr/bin/env python

import sys
import pyshark
import smb2

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
    'smb2.filename',
    'smb2.read_length',
    'smb2.write_length',
    'smb2.file_offset',
    'smb2.fid',
    'smb2.tag'
    ]

def get_file_id(OpList):
    if None == OpList.request:
        return 0

    cmd = OpList.request.command

    if cmd == smb2.SMB2_CREATE and OpList.response.nt_status == 0x0:
        OpList.filename = OpList.request.filename
        return OpList.response.fid
    elif cmd == smb2.SMB2_READ:
        return OpList.request.fid
    elif cmd == smb2.SMB2_WRITE:
        return OpList.request.fid
    elif cmd == smb2.SMB2_CLOSE:
        return OpList.request.fid
    else:
        return 0

def calculate_lifetime(OpList):
    if len(OpList) == 0:
        return 0

    if OpList[0].request.command != smb2.SMB2_CREATE:
        return -1

    if OpList[-1].request.command != smb2.SMB2_CLOSE:
        return '(active)'

    return OpList[-1].request.time - OpList[0].request.time


##
## MAIN
##
if len(sys.argv) < 2:
    sys.argv.append('/home/coffeedude/Desktop/sample_smb.pcap')
    #print >> sys.stderr, "usage: %s <pcap filename>" % (sys.argv[0])
    #sys.exit(1)

pcap_file = pyshark.read(
            sys.argv[1],
            fields,
            'smb2')

smb2_ops = {}

for frame in pcap_file:
    commands = frame.pop('smb2.cmd')
    if list != type(commands):
        commands = [commands]

    for i in range(len(commands)):

        try:
            op = smb2.factory[commands[i]](commands[i], frame)
        except KeyError, e:
            print smb2.CommandName[commands[i]], e, frame

        seq_key = str(op.sequence)

        if not seq_key in smb2_ops:
            smb2_ops[seq_key] = smb2.Exchange()

        if op.is_request():
            smb2_ops[seq_key].request = op
        else:
            # Async response with STATUS_PENDING
            if op.is_response() and op.is_async() and op.nt_status == 0x00000103:
                smb2_ops[seq_key].async = op
            else:
                smb2_ops[seq_key].response = op

fid_stats = {}
cmd_stats = [0 for x in range(19)]

for i in sorted([int(x) for x in smb2_ops]):
    ## Command distribution
    smb2_exg = smb2_ops[str(i)]
    if smb2_exg.request == None:
        continue
    else:
        command = smb2_exg.request.command
        cmd_stats[command] += 1

    f = get_file_id(smb2_ops[str(i)])

    if f == 0:
        continue

    if not f in fid_stats:
        fid_stats[f] = []
    fid_stats[f].append(smb2_ops[str(i)])


##
## Print stats
##
print "\nCommand, Occurrences"
for k in range(len(cmd_stats)):
    print "{0}, {1}".format(
        smb2.CommandName[k],
        cmd_stats[k])

print "\nFile Handles\n"
for k in fid_stats:
    reads = {}
    read_count = 0
    writes = {}
    write_count = 0

    for op_exch in fid_stats[k]:
        if op_exch.response.nt_status != 0x0:
            continue

        if op_exch.request.command == smb2.SMB2_READ:
            if not op_exch.request.read_length in reads:
                reads[op_exch.request.read_length] = []

            reads[op_exch.request.read_length].append(op_exch.request.offset)
            read_count += 1

        elif op_exch.request.command == smb2.SMB2_WRITE:
            if not op_exch.request.write_length in reads:
                writes[op_exch.request.write_length]= []

            writes[op_exch.request.write_length].append(op_exch.request.offset)
            write_count += 1

    print '{0} ({1}: {2}) {3} sec (R/W: {4}/{5})'.format(
        fid_stats[k][0].request.filename,
        k,
        fid_stats[k][0].request.context,
        calculate_lifetime(fid_stats[k]),
        read_count,
        write_count)

    print "\nRead Size, File Offset\n"
    read_string = ''
    for rsize in sorted([int(x) for x in reads]):
        read_string = str(rsize) + ', '
        for roffset in reads[rsize]:
            read_string += str(roffset) + ', '
        read_string = read_string[:-2]
        print read_string

    print "\nWrite Size, File Offset\n"
    write_string = ''
    for wsize in sorted([int(x) for x in writes]):
        write_string = str(wsize) + ', '
        for woffset in writes[wsize]:
            write_string += str(woffset) + ', '
        write_string = write_string[:-2]
        print write_string
