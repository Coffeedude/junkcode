#!/usr/bin/perl -w

my ($i, $body, $modified);

for ($i=0; $i<3; $i++) {
    <STDIN>;
}

while (<STDIN>) {
    last if (!($_ =~ /^\s+$/));
    $modified = 1;
}

$body = "$_";
while (<STDIN>) {
    $body = "$body$_";
}

print "$body" if (defined($modified));
