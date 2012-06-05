#!/usr/bin/perl

#$UUID="5513777b-1b0e-0410-a91d-ed1dec569d5e";
$UUID="f1991e19-cb36-4043-9cec-1ea08b1aba44";

%authors = (
	"builder <builder\@$UUID>"                         => "Builder Account:<builder\@likewise.com> ",
	"aberry <aberry\@$UUID>"                           => "Arlene Berry:<aberry\@likewise.com> ",
	"bdunstan <bdunstan\@$UUID>"                       => "Brian Dunstan:<bdunstan\@likewise.com>",
        "bkoropoff <bkoropoff\@$UUID>"                     => "Brian Koropoff:<bkoropoff\@likewise.com>",
	"dalmeida <dalmeida\@$UUID>"                       => "Danilo Almeida:<dalmeida\@likewise.com>",
	"jerry <jerry\@$UUID>"                             => "Gerald W. Carter:<gcarter\@likewise.com>",
	"glennc <glennc\@$UUID>"                           => "Glenn Curtis:<gcurtis\@likewise.com>",
	"krishnag <krishnag\@$UUID>"                       => "Krishna Ganugapati:<krishnag\@likewise.com>",
	"kstemen <kstemen\@$UUID>"                         => "Kyle Stemen:<kstemen\@likewise.com>",
	"mvellon <mvellon\@$UUID>"                         => "Manny Vellon:<mvellon\@likewise.com>",
	"rafal <rafal\@$UUID>"                             => "Rafal Szczesniak:<rszczesniak\@likewise.com>",
	"snambakam <snambakam\@$UUID>"                     => "Sriram Nambakam:<snambakam\@likewise.com>",
	"Sriram <Nambakam>Sriram <Nambakam\@$UUID>"        => "Sriram Nambakam:<snambakam\@likewise.com>",
	"wfu <wfu\@$UUID>"                                 => "Wei Fu:<wfu\@likewise.com>",
	"Wei <Fu>Wei <Fu\@$UUID>"                          => "Wei Fu:<wfu\@likewise.com>",
	"kmacgugan <kmacgugan\@$UUID>"                     => "Kyleen MacGugan:<kmacgugan\@likewise.com>",
	"shoenisch <shoenisch\@$UUID>"                     => "Steve Hoenisch:<shoenisch\@likewise.com>",
	"ramenn <ramenn\@$UUID>"                           => "Robert Amenn:<ramenn\@likewise.com>",
	"jgeer <jgeer\@$UUID>"                             => "John Geer:<jgeer\@likewise.com>",
	"marney <marney\@$UUID>"                           => "Michelle Arney:<marney\@likewise.com>",
	"dloudon <dloudon\@$UUID>"                         => "David Loudon:<dloudon\@likewise.com>",
	"aglabek <aglabek\@$UUID>"                         => "Arek Glabek:<aglabek\@likewise.com>",
	"mporwit <mporwit\@$UUID>"                         => "Marcin Porwit:<mporwit\@likewise.com>",
	"asrinivasan <asrinivasan\@$UUID>"                 => "Aravind Srinivasan:<aravind.srinivasan\@isilon.com>",
	"sdanneman <sdanneman\@$UUID>"                     => "Steven Danneman:<steven.danneman\@isilon.com>",
	"tprouty <tprouty\@$UUID>"                         => "Tim Prouty:<tim.prouty\@isilon.com>",
	"zloafman <zloafman\@$UUID>"                       => "Zach Loafman:<zachary.loafman\@isilon.com>",
	"v-suneetha_g <v-suneetha_g\@$UUID>"               => "v-suneetha_g:<contractor\@likewise.com>",
	"v-palani_m <v-palani_m\@$UUID>"                   => "v-palani_m:<contractor\@likewise.com>",
	"v-todds <v-todds\@$UUID>"                         => "Todd Stecher:<contractor\@likewise.com>",
        "todds <todds\@$UUID>"                             => "Todd Stecher:<contractor\@likewise.com>",
	"rrodseth <rrodseth\@$UUID>"                       => "Richard Rodseth:<rrodseth\@likewise.com>",

	"CORP\\aberry <CORP\\aberry\@$UUID>"               => "Arlene Berry:<aberry\@likewise.com> ",
	"CORP\\bdunstan <CORP\\bdunstan\@$UUID>"           => "Brian Dunstan:<bdunstan\@likewise.com>",
        "CORP\\bkoropoff <CORP\\bkoropoff\@$UUID>"         => "Brian Koropoff:<bkoropoff\@likewise.com>",
        "CORP\\metze <CORP\\metze\@$UUID>"                 => "metze:<contractor\@likewise.com>",
	"CORP\\dalmeida <CORP\\dalmeida\@$UUID>"           => "Danilo Almeida:<dalmeida\@likewise.com>",
	"CORP\\gcarter <CORP\\gcarter\@$UUID>"             => "Gerald W. Carter:<gcarter\@likewise.com>",
	"CORP\\glennc <CORP\\glennc\@$UUID>"               => "Glenn Curtis:<gcurtis\@likewise.com>",
	"CORP\\kaya <CORP\\kaya\@$UUID>"                   => "Kaya Bekiroðlu:<kaya\@likewise.com>",
	"CORP\\kstemen <CORP\\kstemen\@$UUID>"             => "Kyle Stemen:<kstemen\@likewise.com>",
	"CORP\\pchai <CORP\\pchai\@$UUID>"                 => "Peter Chai:<pchai\@likewise.com>",
	"CORP\\rszczesniak <CORP\\rszczesniak\@$UUID>"     => "Rafal Szczesniak:<rszczesniak\@likewise.com>",
	"CORP\\wfu <CORP\\wfu\@$UUID>"                     => "Wei Fu:<wfu\@likewise.com>",
	"CORP\\mlampi <CORP\\mlampi\@$UUID>"               => "Michael Lampi:<mlampi\@likewise.com>",
	"CORP\\abernstein <CORP\\abernstein\@$UUID>"       => "Adam Bernstein:<abernstein\@likewise.com>",
	"CORP\\mguy <CORP\\mguy\@$UUID>"                   => "Marc Guy:<mguy\@likewise.com>",
	"CORP\\keithr <CORP\\keithr\@$UUID>"               => "Keith Reynolds:<kreynolds\@likewise.com>",
	"CORP\\ssalley <CORP\\ssalley\@$UUID>"             => "Scott Salley:<ssalley\@likewise.com>",
	"CORP\\yvandoorn <CORP\\yvandoorn\@$UUID>"         => "Yvo Van Doorn:<yvandoorn\@likewise.com>",
	"CORP\\epopovich <CORP\\epopovich\@$UUID>"         => "Evgeny Popovich:<epopovich\@likewise.com>",
	"CORP\\slavam <CORP\\slavam\@$UUID>"               => "Slava Minyailov:<sminyailov\@likewise.com>",
	"CORP\\dleimbach <CORP\\dleimbach\@$UUID>"         => "David Leimbach:<dleimbach\@likewise.com>",
	"CORP\\hwilkinson <CORP\\hwilkinson\@$UUID>"       => "Hal Wilkinson:<hwilkinson\@likewise.com>",
	"CORP\\rbudd <CORP\\rbudd\@$UUID>"                 => "Robin Budd:<rbudd\@likewise.com>",
	"CORP\\bcombs <CORP\\bcombs\@$UUID>"               => "Bob Combs:<bcombs\@likewise.com>",
	"CORP\\kanderson <CORP\\kanderson\@$UUID>"         => "Ki Anderson:<kanderson\@likewise.com>",
	"CORP\\dsmith <CORP\\dsmith\@$UUID>"               => "Donald Smith:<dsmith\@likewise.com>",
	"CORP\\kmacgugan <CORP\\kmacgugan\@$UUID>"         => "Kyleen MacGugan:<kmacgugan\@likewise.com>",
	"CORP\\sdanneman <CORP\\sdanneman\@$UUID>"         => "Steven Danneman:<steven.danneman\@isilon.com>",
	"CORP\\glewison <CORP\\glewison\@$UUID>"           => "Gabriel Lewison:<glewison\@likewise.com>",
	"CORP\\shaskin <CORP\\shaskin\@$UUID>"             => "Scott Haskin:<shaskin\@likewise.com>",
	"CORP\\pmartin <CORP\\pmartin\@$UUID>"             => "Paul Martin:<pmartin\@likewise.com>",
	"CORP\\rhaddon <CORP\\rhaddon\@$UUID>"             => "Rob Haddon:<rhaddon\@likewise.com>",
	"CORP\\kfoley <CORP\\kfoley\@$UUID>"               => "Kelsey Foley:<kfoley\@likewise.com>",
	"CORP\\cking <CORP\\cking\@$UUID>"                 => "Cosmo King:<cking\@likewise.com>",
	"CORP\\shoenisch <CORP\\shoenisch\@$UUID>"         => "Steve Hoenisch:<shoenisch\@likewise.com>",
	"CORP\\shoenisch <CORP\\shoenisch\@$UUID>"         => "Steve Hoenisch:<shoenisch\@likewise.com>",
	"CORP\\rrodseth <CORP\\rrodseth\@$UUID>"           => "Richard Rodseth:<rrodseth\@likewise.com>",
	"CORP\\johrt <CORP\\johrt\@$UUID>"                 => "John Ort:<contractor\@likewise.com>",

	"CORPQA\\jerry <CORPQA\\jerry\@$UUID>"             => "Gerald W. Carter:<gcarter\@likewise.com>",

	"CORP\\v-fshaik <CORP\\v-fshaik\@$UUID>"           => "v-fshaik:<contractor\@likewise.com>",
	"CORP\\v-krangarajan <CORP\\v-krangarajan\@$UUID>" => "v-krangarajan:<contractor\@likewise.com>",
	"CORP\\v-kandasamy_m <CORP\\v-kandasamy_m\@$UUID>" => "v-kandasamy_m:<contractor\@likewise.com>",
	"CORP\\v-kusumakara_c <CORP\\v-kusumakara_c\@$UUID>"=> "v-kusumakara_c:<contractor\@likewise.com>",
	"CORP\\v-rajendraprasad_k <CORP\\v-rajendraprasad_k\@$UUID>"=> "v-rajendraprasad_k:<contractor\@likewise.com>",
	"CORP\\v-ravindra_d <CORP\\v-ravindra_d\@$UUID>"    => "v-ravindra_d:<contractor\@likewise.com>",
	"CORP\\v-satya <CORP\\v-satya\@$UUID>"              => "v-satya:<contractor\@likewise.com>",
	"CORP\\v-sivabalan_k <CORP\\v-sivabalan_k\@$UUID>"  => "v-sivabalan_k:<contractor\@likewise.com>",
	"CORP\\v-amit_k <CORP\\v-amit_k\@$UUID>"           => "v-amit_k:<contractor\@likewise.com>",
	"CORP\\v-ibharati <CORP\\v-ibharati\@$UUID>"       => "v-ibharati:<contractor\@likewise.com>",
	"CORP\\v-ramprasad_b <CORP\\v-ramprasad_b\@$UUID>" => "v-ramprasad_b:<contractor\@likewise.com>",
	"CORP\\v-sudheer_b <CORP\\v-sudheer_b\@$UUID>"     => "v-sudheer_b:<contractor\@likewise.com>",
	"CORP\\v-suneetha_g <CORP\\v-suneetha_g\@$UUID>"   => "v-suneetha_g:<contractor\@likewise.com>",
	"CORP\\v-shivaprakash_dm <CORP\\v-shivaprakash_dm\@$UUID>"   => "v-shivaprakash_dm:<contractor\@likewise.com>",
	"CORP\\v-ajith_g <CORP\\v-ajith_g\@$UUID>"         => "v-ajith_g:<contractor\@likewise.com>",
);

while(<STDIN>) {
    chomp($_);

	$id = $_;

	$id =~ s/^\s+//;
	$id =~ s/\//\\/g;
        $id =~ s/corp/CORP/g;

    $new_id = $authors{$id};
    die "Unknown author \"$id\"!\n" if !defined($authors{$id});
    # print "$id" if !defined($authors{$id});

    print "$new_id";
}
