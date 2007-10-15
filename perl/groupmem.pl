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
my ( $user, $group );
my ( $action );
my ( $result, $retvalue);
my ( %options );
my ( $handle );
my ( $msg );
my ( $entry );
my ( $result );
my ( $server, $base, $binddn, $bindpw );
my ( $peopledn, $groupdn );
my ( $filter );
my ( @attributes );


##############################################################################
## Script defaults

$server   	= "ldap";
$base           = "dc=plainjoe,dc=org";
$peopledn   	= "ou=people,$base";
$groupdn        = "ou=group,$base";
$binddn   	= "cn=diradmin,$peopledn";
$bindpw   	= "";

## End of script defaults
##############################################################################


##############################################################################
## Print the option usage
sub usage {

	print "groupmem.pl [options]\n";
	print "Options:\n";
	print "  --add      add group to user as a secondary group\n";
	print "  --base     base_suffix\n";
	print "  --binddn   binddn\n";
	print "  --delete   remove user from group\n";
	print "  --group    group namen";
	print "  --primary  set group as primary group for user\n";
	print "  --server   ldap_server\n";
	print "  --user     user name\n";
	print "  --bindpw   bindpw\n";

}


##############################################################################
## Check whether or not the user already existed in the directory
sub account_exists {
	my ( $ldap, $name ) = @_;
	my ( $msg );

	$msg = $ldap->search ( base => $peopledn, filter => "(&(uid=$name)(objectclass=posixAccount))" );
	$msg->code && warn $msg->error;

	return ($msg->count);
}

##############################################################################
## Check whether or not the group already existed in the directory
sub group_exists {
	my ( $ldap, $name ) = @_;
	my ( $msg );

	$msg = $ldap->search ( base => $groupdn, filter => "(&(cn=$name)(objectclass=posixGroup))" );
	$msg->code && warn $msg->error;

	return ($msg->count);
}

##############################################################################
## Check whether or not the user already existed in the directory
sub add_user_to_group {
	my ( $ldap, $group, $user ) = @_;

	my ( $msg, $entry );

	$msg = $ldap->search ( base => $groupdn, filter => "(&(cn=$group)(objectclass=posixGroup))" );
	if ($msg->code) {
		warn $msg->error;
		return -1;
	}

	if (!$msg->count) { return -1; }

	if ($msg->count > 1) {
		warn "More than one entry returned for \"(&(cn=$group)(objectclass=posixGroup))\" ";
		warn "when searching base $groupdn";
	}

	$entry = $msg->entry(0);

	$entry->changetype( 'modify' );
	$entry->add( memberUid => $user );

	$msg = $entry->update( $ldap );

	if ($msg->code) {
		warn $msg->error;
		return $msg->code;
	}

	return 0;
}

##############################################################################
## Check whether or not the user already existed in the directory
sub delete_user_from_group {
	my ( $ldap, $group, $user ) = @_;

	my ( $msg, $entry );

	$msg = $ldap->search ( base => $groupdn, filter => "(&(cn=$group)(objectclass=posixGroup))" );
	if ($msg->code) {
		warn $msg->error;
		return -1;
	}

	if (!$msg->count) { return -1; }

	if ($msg->count > 1) {
		warn "More than one entry returned for \"(&(cn=$group)(objectclass=posixGroup))\" ";
		warn "when searching base $groupdn";
	}

	$entry = $msg->entry(0);

	$entry->changetype( 'modify' );
	$entry->delete( memberUid => [ $user ] );

	$msg = $entry->update( $ldap );

	if ($msg->code) {
		warn $msg->error;
		return $msg->code;
	}

	return 0;
}

##############################################################################
## Check whether or not the user already existed in the directory
sub set_primary_group {
	my ( $ldap, $group, $user ) = @_;

	my ( $msg, $entry, $gid );

	$msg = $ldap->search ( base => $peopledn, filter => "(&(uid=$user)(objectclass=posixAccount))" );
	if ($msg->code) {
		warn $msg->error;
		return -1;
	}

	if (!$msg->count) { return -1; }

	if ($msg->count > 1) {
		warn "More than one entry returned for (&(uid=$user)(objectclass=posixAccount))\" ";
		warn " when searching base $peopledn";
	}

	$gid = get_gid( $ldap, $group );
	if ( $gid == -1 ) {
		print "Cannot get gid for $group!\n";
		return -1;
	}
	
	$entry = $msg->entry(0);

	$entry->changetype( 'modify' );
	$entry->replace( gidNumber => [ $gid ] );

	$msg = $entry->update( $ldap );

	if ($msg->code) {
		warn $msg->error;
		return $msg->code;
	}

	return 0;
}


##############################################################################
## lookup the gidNumber for a given group name
sub get_gid {
	my ( $ldap, $name ) = @_;
	my ( $msg, $entry );

	$msg = $ldap->search ( base => $groupdn, filter => "(&(cn=$name)(objectclass=posixGroup))" );
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
##                               MAIN DRIVER                                ##
##############################################################################

$result = GetOptions(\%options,
			"user=s",
			"group=s",
			"primary",
			"add",
			"delete",
			"server=s",
			"binddn=s",
			"bindpw=s",
			"base=s");

if ( !$result ) {
	usage();
	exit 1;
}

$user = $options{'user'};
$group = $options{'group'};

if ( !defined($user) || !defined($group) ) {
	print "You must define a user and group!\n";
	usage();
	exit 1;
}

if ( defined($options{'add'}) ) {
	$action = 1;
	print "Adding $group as secondary group for user $user\n";
} elsif ( defined($options{'delete'}) ) {
	$action = 2;
	print "Removing user $user from $group\n";
} elsif ( defined($options{'primary'}) ) {
	$action = 3;
	print "Setting $group as primary group for user $user\n";
}

if ( !defined( $action ) ) {
	print "You didn't tell me what to do!\n";
	usage();
	exit 1;
}

## do we use the defaults?

if ( "$options{'server'}" ) { $server = $options{'server'}; }
if ( "$options{'binddn'}" ) { $binddn = $options{'binddn'}; }
if ( "$options{'bindpw'}" ) { $bindpw = $options{'bindpw'}; }
if ( "$options{'base'}"   ) {
	$base     = $options{'base'};
	$peopledn = "ou=people,$base";
	$groupdn  = "ou=group,$base";
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

#$result = $handle->start_tls();
#$result->code && die $result->error;

$result = $handle->bind($binddn, password => $bindpw);
$result->code && die $result->error;

##
## Check for existing entries
##
if (!account_exists($handle, $user)) {
	print "$user does not exist on ldap://$server/$peopledn!\n";
	exit 2;
}

if (!group_exists($handle, $group)) {
	print "$group does not exist on ldap://$server/$group!\n";
	exit 2;
}

## add the user to the group
if ( $action == 1 ) {
	$retvalue = add_user_to_group( $handle, $group, $user );
} 
## delete the user from the group
elsif ( $action == 2) {
	$retvalue = delete_user_from_group( $handle, $group, $user );
} 
## set the primary group membership
else {
	$retvalue = set_primary_group( $handle, $group, $user );
}

$handle->unbind();

exit $retvalue;
