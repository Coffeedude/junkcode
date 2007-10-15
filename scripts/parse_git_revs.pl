#!/usr/bin/perl -w

my ($rev, $revlist, $badrevlist);

open(REVS, "< $ARGV[0]") || die @!;
while (<REVS>) {
    chomp($_);
    $revlist{$_} = 1;
}
close(REVS);

open(BADREVS, "< $ARGV[1]") || die @!;
while (<BADREVS>) {
    chomp($_);
    $badrevlist{$_} = "BADREV_";
}
close(BADREVS);


while (<STDIN>) {
    chomp($_);
    $_ =~ s/^\s+//;

    if ($_ =~ /^commit [a-f0-9]/) {
	$_ =~ s/^commit //;
	$hash = $_;
	undef $rev;
    } elsif ($_ =~ /^r[0-9]+:/) {
	$_ =~ s/:.*//;
	$_ =~ s/^r//;
	$rev = $_;

	if (defined($revlist{$rev}) > 0) {
	    print "$badrevlist{$rev}" if (defined($badrevlist{$rev}));
	    print "$hash\n";
	}
    }
}

