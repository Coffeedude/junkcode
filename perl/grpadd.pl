#!/usr/bin/perl

##
## Pragmas
##
use Net::LDAP;
use Digest::MD5 qw(md5_base64);
use Term::ReadKey;
use Getopt::Long;
use strict;

##
## scoped variables
##
my ( $grpname );
my ( $result );
my ( %options );
my ( $handle );
my ( $msg );
my ( $entry );
my ( $result );
my ( $server, $base, $binddn, $bindpw );
my ( $people, $group );
my ( $filter );
my ( @attributes );
my ( $new_group );


##############################################################################
## Script defaults

$server         = "ldap";
$base           = "dc=plainjoe,dc=org";
$people         = "ou=people,$base";
$group          = "ou=group,$base";
$binddn         = "cn=diradmin,$people";
$bindpw         = "";

## End of script defaults
##############################################################################


##############################################################################
## Print the option usage
sub usage {

	print "useradd.pl [options] login_name\n";
	print "Options:\n";
	print "  --base     base_suffix\n";
	print "  -d         description\n";
	print "  --binddn   binddn\n";
	print "  -g         numeric_group\n";
	print "  --server   ldap_server\n";
	print "  --bindpw   bindpw\n";
	print "  -m         comma separated list of members\n";

}


##############################################################################
## Check whether or not the user already existed in the directory
sub group_exists {
	my ( $ldap, $name ) = @_;
	my ( $msg );

	$msg = $ldap->search ( base => $group, filter => "(&(objectclass=posixGroup)(cn=$name))" );
	$msg->code && warn $msg->error;

	return ($msg->count);
}

##############################################################################
## Check whether or not the user already existed in the directory
sub gid_exists {
	my ( $ldap, $gid) = @_;
	my ( $msg );

	$msg = $ldap->search ( base => $group, filter => "(gidNumber=$gid)" );
	$msg->code && warn $msg->error;

	return ($msg->count);
}

##############################################################################
## lookup the gidNumber for a given group name
sub get_gid {
	my ( $ldap, $name ) = @_;
	my ( $msg, $entry );

	$msg = $ldap->search ( base => $group, filter => "(cn=$name)" );
	if ($msg->code) {
		warn $msg->error;
		return -1;
	}

	if (!$msg->count) { return -1; }

	if ($msg->count > 1) {
		warn "More than one entry returned for (cn=$name) when searching base";
		warn "$group";
	}

	$entry = $msg->entry(0);
	
	if (!defined($entry->get_value('gidNumber'))) {
		return -1;
	}
	else {
		return $entry->get_value('gidNumber');
	}
}


##############################################################################
## get the next available gid from the idPool
sub get_next_gid {
	my ( $ldap ) = @_;
	my ( $gid );
	my ( $msg, $entry );
	my ( @Add, @Delete, @Changes );
	
	do {
		$msg = $ldap->search ( base => $group, scope => "base", 
				filter => "(objectclass=gidPool)" );
		if ($msg->code ) {
			warn $msg->error;
			return -1;
		}

		if ( ! $msg->count ) {
			warn "Unable to locate gidPool entry!";
			return -1;
		}

		$entry = $msg->entry(0);
		$gid = $entry->get_value( 'gidNumber' );


	
		push ( @Delete, 'gidNumber', $gid );
		push ( @Add, 'gidNUmber', $gid+1 );
		push ( @Changes, 'delete', \@Delete );
		push ( @Changes, 'add', \@Add );

		$result = $ldap->modify ( $entry->dn(), 
				'changes' => [ @Changes ] );

		if ( $result->code ) { $gid = -1 }

	} while ( $gid == -1 );

	return $gid;

}

##############################################################################
##                               MAIN DRIVER                                ##
##############################################################################

$result = GetOptions(\%options, 
			"d=s", 
			"g=i",
			"m=s",
			"server=s",
			"binddn=s",
			"bindpw=s",
			"base=s");

if (!$result  || $#ARGV) {
	usage();
	exit 1;
}

$grpname = shift @ARGV;

print "Creating account for [$grpname]\n";

## do we use the defaults?

if ( "$options{'server'}" ) { $server = $options{'server'}; }
if ( "$options{'binddn'}" ) { $binddn = $options{'binddn'}; }
if ( "$options{'bindpw'}" ) { $bindpw = $options{'bindpw'}; }
if ( "$options{'base'}"   ) { 
	$base   = $options{'base'};   
	$people = "ou=people,$base";
	$group  = "ou=group,$base";
}

if ( ! "$bindpw" ) {
	ReadMode( 'noecho' );
	print "Enter Bind Password: ";
	$bindpw = ReadLine(0);
	ReadMode( 'restore' );
	chomp($bindpw);
}

##
## connect and bind to the ldap server
##
$handle = Net::LDAP->new ($server, port => 389, version => 3 )
or die "$@";

$result = $handle->start_tls();
$result->code && die $result->error;

$result = $handle->bind($binddn, password => $bindpw);
$result->code && die $result->error;

##
## Check for existing entries
## 
if (group_exists($handle, $grpname)) {
	print "Account for $grpname already exists!\n";
	exit 2;
}

if (defined($options{'g'})) { 
	if (gid_exists($handle, $options{'g'})) {
		print "gid [", $options{'g'}, "] already exists!\n";
		exit 3;
	}
}
else {
	$options{'g'} = get_next_gid($handle);
	if ($options{'g'} == -1) {
		warn "Unable to enerate new gid!\n";
		exit 1;	
	}
} 

## Create the LDAP entry
$new_group = Net::LDAP::Entry->new();
$new_group->add(	'objectclass'   => "posixGroup",
		'cn'            => $grpname,
		'gidNumber'     => $options{'g'} );

if ( defined($options{'d'}) ) {
	$new_group->add( 'description' => $options{'d'} );
}

if ( defined($options{'m'}) ) {
	my ( @members, $member );

	## get the list of members
	@members = split( /,/, $options{'m'} );
	foreach $member ( @members ) {
		$new_group->add( 'memberUid' => $member );
	}
}


$new_group->dn( "cn=$grpname,$group" );
$new_group->changetype('add');

$new_group->dump();
$result = $new_group->update($handle);
$result->code && warn $result->error;

$handle->unbind();


