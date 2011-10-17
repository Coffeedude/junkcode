#!/usr/bin/perl 

$UUID="5513777b-1b0e-0410-a91d-ed1dec569d5e";

%authors = (
	"CORP\\aberry <CORP\\aberry\@$UUID>"               => "Arlene Berry <aberry\@likewise.com> ",
	"bdunstan <bdunstan\@$UUID>"                       => "Brian Dunstan <bdunstan\@likewise.com>",
        "bkoropoff <bkoropoff\@$UUID>"                     => "Brian Koropoff <bkoropoff\@likewise.com>",
        "CORP\\metze <CORP\\metze\@$UUID>"                 => "Contract <contractor\@likewise.com>",
	"CORP\\v-amit_k <CORP\\v-amit_k\@$UUID>"           => "Contract <contractor\@likewise.com>",
	"CORP\\v-ramprasad_b <CORP\\v-ramprasad_b\@$UUID>" => "Contract <contractor\@likewise.com>",
	"CORP\\v-sudheer_b <CORP\\v-sudheer_b\@$UUID>"     => "Contract <contractor\@likewise.com>",
	"CORP\\v-suneetha_g <CORP\\v-suneetha_g\@$UUID>"   => "Contract <contractor\@likewise.com>",
	"CORP\\v-shivaprakash_dm <CORP\\v-shivaprakash_dm\@$UUID>"   => "Contract <contractor\@likewise.com>",
	"v-suneetha_g <CORP\\v-suneetha_g\@$UUID>"         => "Contract <contractor\@likewise.com>",
	"v-palani_m <v-palani_m\@$UUID>"                   => "Contract <contractor\@likewise.com>",
	"v-todds <v-todds\@$UUID>"                         => "Contract <contractor\@likewise.com>",
	"dalmeida <dalmeida\@$UUID>"                       => "Danilo Almeida <dalmeida\@likewise.com>",
	"CORP\\gcarter <CORP\\gcarter\@$UUID>"             => "Gerald W. Carter <gcarter\@likewise.com>",
	"jerry <jerry\@$UUID>"                             => "Gerald W. Carter <gcarter\@likewise.com>",
	"glennc <glennc\@$UUID>"                           => "Glenn Curtis <gcurtis\@likewise.com>",
	"CORP\\glennc <CORP\\glennc\@$UUID>"               => "Glenn Curtis <gcurtis\@likewise.com>",
	"CORP\\kaya <CORP\\kaya\@$UUID>"                   => "Kaya Bekiroðlu <kaya\@likewise.com>",
	"krishnag <krishnag\@$UUID>"                       => "Krishna Ganugapati <krishnag\@likewise.com>",
	"kstemen <kstemen\@$UUID>"                         => "Kyle Stemen <kstemen\@likewise.com>",
	"CORP\\kstemen <CORP\\kstemen\@$UUID>"             => "Kyle Stemen <kstemen\@likewise.com>",
	"mvellon <mvellon\@$UUID>"                         => "Manny Vellon <mvellon\@likewise.com>",
	"CORP\\pchai <CORP\\pchai\@$UUID>"                 => "Peter Chai <pchai\@likewise.com>",
	"CORP\\rszczesniak <CORP\\rszczesniak\@$UUID>"     => "Rafal Szczesniak <rafal\@likewise.com>",
	"rafal <rafal\@$UUID>"                             => "Rafal Szczesniak <rafal\@likewise.com>",
	"snambakam <snambakam\@$UUID>"                     => "Sriram Nambakam <snambakam\@likewise.com>",
	"CORP\\wfu <CORP\\wfu\@$UUID>"                     => "Wei Fu <wfu\@likewise.com>",
	"wfu <wfu\@$UUID>"                                 => "Wei Fu <wfu\@likewise.com>",
	"CORP\\mlampi <CORP\\mlampi\@$UUID>"               => "Michael Lampi <mlampi\@likewise.com>",
	"CORP\\abernstein <CORP\\abernstein\@$UUID>"       => "Adam Bernstein <abernstein\@likewise.com>",
	"CORP\\mguy <CORP\\mguy\@$UUID>"                   => "Marc Guy <mguy\@likewise.com>",
	"CORP\\keithr <CORP\\keithr\@$UUID>"               => "Keith Reynolds <kreynolds\@likewise.com>",
	"CORP\\ssalley <CORP\\ssalley\@$UUID>"             => "Scott Salley <ssalley\@likewise.com>",
	"CORP\\yvandoorn <CORP\\yvandoorn\@$UUID>"         => "Yvo Van Doorn <yvandoorn\@likewise.com>",
	"CORP\\epopovich <CORP\\epopovich\@$UUID>"         => "Evgeny Popovich <epopovich\@likewise.com>",
	"CORP\\slavam <CORP\\slavam\@$UUID>"               => "Slava Minyailov <sminyailov\@likewise.com>",
	"CORP\\dleimbach <CORP\\dleimbach\@$UUID>"         => "David Leimbach <dleimbach\@likewise.com>",
	"CORP\\kanderson <CORP\\kanderson\@$UUID>"         => "Ki Anderson <kanderson\@likewise.com>",
	"CORP\\dsmith <CORP\\kanderson\@$UUID>"            => "Donald Smith <dsmith\@likewise.com>",
	"asrinivasan <asrinivasan\@$UUID>"                 => "Aravind Srinivasan <aravind.srinivasan\@isilon.com>",
	"sdanneman <sdanneman\@$UUID>"                     => "Steven Danneman <steven.danneman\@isilon.com>",
	"tprouty <tprouty\@$UUID>"                         => "Tim Prouty <tim.prouty\@isilon.com>",
	"zloafman <zloafman\@$UUID>"                       => "Zach Loafman <zachary.loafman\@isilon.com>"
);

%compnames = (
	"lmc"	=> "Quartz"
	# "lwreg" => "lwreg2"
	# "build" => "oem-local"
);

$component=$ARGV[0];

open(PATCHFILE, "< $ARGV[1]") || die @!;
while(<PATCHFILE>) {
    chomp($_);

    if ($_ =~ /^From: /) {
	($tag, $id) = split(/:/, $_);
	die "Bad author!\n" if (!defined($id));

	$id =~ s/^\s+//;

	$new_id = $authors{$id};
	if (!defined($authors{$id})) {
	    $new_id = "Unknown <unknown\@likewise.com>";
	}

	print "From: $new_id\n";
	next;
    }

    ## Replace the git-svn metadata
    if ($_ =~ /^git-svn-id:/) {
	$compname_svn = "$component";
	if (defined($compnames{$component})) {
	    $compname_svn = "$compnames{$component}";
	}
	$_ =~ s/$compname_svn@/~/;
	($path, $rev) = split(/~/, $_);
	($rev) = split(/\s+/, $rev);
	$_ = "($component: r$rev)";
    }

    ## Need to reformat for the consolidated repo one level up
    $_ =~ s/\-\-\- a\//\-\-\- a\/$component\//;
    $_ =~ s/\+\+\+ b\//\+\+\+ b\/$component\//;

    if ($_ =~ /^diff --git /) {
	$_ =~ s/ a\// a\/$component\//;
	$_ =~ s/ b\// b\/$component\//;
    }

    print "$_\n";
}
