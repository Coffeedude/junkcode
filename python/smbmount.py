#!/usr/bin/python

"""Command line utility for mounting SMB filesystems"""

import sys, os, getopt

def main():
    mount_options = 'uid=%d,gid=%d,file_mode=0600,dir_mode=0700' % (os.getuid(), os.getgid())

    if len(sys.argv) < 2:
        print "Usage: %s <unc path> [options]" % (sys.argv[0])
        return 1

    path = sys.argv[1].split('/')
    server = path[2]
    share = path[3]

    print 'Server = %s' % server
    print 'Share  = %s' % share

    optlist, args = getopt.getopt(sys.argv[2:], '',
                                  ['user=', 
                                   'domain=', 
                                   'password=',
                                   'mnt=',
                                   'signing='])
    d = {}
    # Set defaults
    d['--signing'] = 'yes'
    for key,value in optlist:
        d[key] = value

    if not d.has_key('--user'):
        if d['--signing'] == 'yes':
            mount_options = 'sec=krb5i,%s' % (mount_options)
        else:
            mount_options = 'sec=krb5,%s' % (mount_options)
    else:
        if d['--signing'] == 'yes':
            mount_options = 'sec=ntlmi,user=%s,domain=%s,%s' % (d['--user'], d['--domain'], mount_options)
        else:
            mount_options = 'sec=ntlm,user=%s,domain=%s,%s' % (d['--user'], d['--domain'], mount_options)

        if d.has_key('--password'):
            mount_options = '%s,password=%s' % (mount_options, d['--password'])

    if d.has_key('--mnt'):
        mount_target = d['--mnt']
    else:
        mount_target = '%s/smb/%s/%s' % (os.environ['HOME'], server, share)

    if not os.path.exists(mount_target):
        os.makedirs(mount_target)
    else:
        if not os.path.isdir(mount_target):
            print "%s is not a directory." % (mount_target)
            return 1

    cmd = 'sudo mount.cifs %s %s -o%s' % (sys.argv[1], mount_target, mount_options)
    print cmd
    os.system(cmd)

if __name__ == '__main__':
    main()
