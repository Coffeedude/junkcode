#!/usr/bin/perl -w

my %authors;

open(AUTHORS, "< $ARGV[0]") || die @!;
while (<AUTHORS>) {
    chomp($_);
    $_ =~ s/\s+=\s+/=/;
    ($name, $userinfo) = split(/=/, $_);
    $authors{$name} = $userinfo;
}
close(AUTHORS);

open(INDATA, "< $ARGV[1]") || die @!;
while (<INDATA>) {
    chomp($_);
    if ($_ =~ /^committer /) {
	@lineinfo = split(/ /, $_);
	if (!defined($authors{$lineinfo[1]})) {
	    die "No entry for \"$lineinfo[1]\" in $ARGV[0]!\n";
	}
	($realname, $emailaddr) = split(/</, $authors{$lineinfo[1]});
	$realname =~ s/\s+$//g;
	$emailaddr =~ s/^\s+//;
	print "committer $realname <$emailaddr  $lineinfo[3] $lineinfo[4]\n";
    } else {
	print "$_\n";
    }
}
close(INDATA);
