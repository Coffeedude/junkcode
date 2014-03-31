import sys, os, time, hashlib

if len(sys.argv) < 3:
	print "Usage: {0} <source directory> <target directory>".format(sys.argv[0])
	sys.exit(2)

walk_dir = sys.argv[1]
if not os.path.isdir(walk_dir):
	print "{0} is not a directory!".format(walk_dir)
	sys.exit(1)

top_dir = sys.argv[2]

if not os.path.isdir(top_dir):
	print "{0} is not a directory!".format(top_dir)
	sys.exit(1)

for root, dirs, files in os.walk(walk_dir):
	for f in files:
		fname = os.path.join(root, f)
		s = os.stat(fname)

		# Determine destination directory
		subdir = None
		target_dir = os.path.join(top_dir, root)

		if not os.path.isdir(target_dir):
			os.mkdir(target_dir)

		target_file = os.path.join(target_dir, f)
		if os.path.isfile(target_file):
			src_hash = hashlib.md5(open(fname, 'rb').read()).digest()
			dest_hash = hashlib.md5(open(target_file, 'rb').read()).digest()
			stat_target = os.stat(target_file)
			# filename, size, and MD5 are identical
			if stat_target.st_size == s.st_size and src_hash == dest_hash:
				print "Removing duplicate file ({0})".format(fname)
				os.remove(fname)
			else:
				print 'Skipping file with name conflict! ({0})'.format(fname)
		else:
			os.rename(fname, target_file)

