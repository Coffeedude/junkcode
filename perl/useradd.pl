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
my ( $username );
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
my ( $home_dir, $shell, $default_group, $password );
my ( $new_user );
my ( $nfs_server, $nfs_path, $maildomain );


##############################################################################
## Script defaults

$server   	= "ldap.home.plainjoe.org";
$base           = "dc=home,dc=plainjoe,dc=org";
$people   	= "ou=people,$base";
$group          = "ou=group,$base";
$binddn   	= "cn=diradmin,ou=people,$base";
$bindpw   	= "";
$home_dir 	= "/home/nfs2/";
$shell    	= "/bin/bash";
$default_group 	= "guest";
$password       = "*";
$nfs_server     = "nas1.home.plainjoe.org";
# $maildomain	= "plainjoe.org";

## End of script defaults
##############################################################################


##############################################################################
## Print the option usage
sub usage {

	print "useradd.pl [options] login_name\n";
	print "Options:\n";
	print "  --base     base_suffix\n";
	print "  -c         comment\n";
	print "  -d         home_dir\n";
	print "  --binddn   binddn\n";
	print "  -g         numeric_group\n";
	print "  --server   ldap_server\n";
	print "  -p         password\n";
	print "  -s         shell\n";
	print "  -u         numeric_uid\n";
	print "  --bindpw   bindpw\n";

}


##############################################################################
## Check whether or not the user already existed in the directory
sub account_exists {
	my ( $ldap, $name ) = @_;
	my ( $msg );

	$msg = $ldap->search ( base => $people, filter => "(uid=$name)" );
	$msg->code && warn $msg->error;

	return ($msg->count);
}

##############################################################################
## Check whether or not the user already existed in the directory
sub uid_exists {
	my ( $ldap, $uid) = @_;
	my ( $msg );

	$msg = $ldap->search ( base => $people, filter => "(uidNumber=$uid)" );
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
## get the next available uid from the idPool
sub get_next_uid {
	my ( $ldap ) = @_;
	my ( $uid );
	my ( $msg, $entry );
	my ( @Add, @Delete, @Changes );
	
	do {
		$msg = $ldap->search ( base => $people, scope => "base", 
				filter => "(objectclass=uidPool)" );
		if ($msg->code ) {
			warn $msg->error;
			return -1;
		}

		if ( ! $msg->count ) {
			warn "Unable to locate uidPool entry!";
			return -1;
		}

		$entry = $msg->entry(0);
		$uid = $entry->get_value( 'uidNumber' );


	
		push ( @Delete, 'uidNumber', $uid );
		push ( @Add, 'uidNumber', $uid+1 );
		push ( @Changes, 'delete', \@Delete );
		push ( @Changes, 'add', \@Add );

		$result = $ldap->modify ( $entry->dn(), 
				'changes' => [ @Changes ] );

		if ( $result->code ) { print $result->error(); $uid = -1 }

	} while ( $uid == -1 );

	return $uid;

}

##############################################################################
##                               MAIN DRIVER                                ##
##############################################################################

$result = GetOptions(\%options, 
			"c=s", 
			"d=s", 
			"g=i",
			"p=s",
			"s=s",
			"u=i",
			"server=s",
			"binddn=s",
			"bindpw=s",
			"base=s");

if (!$result  || $#ARGV) {
	usage();
	exit 1;
}

$username = shift @ARGV;

print "Creating account for [$username]\n";

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

#$result = $handle->start_tls();
#$result->code && die $result->error;

$result = $handle->bind($binddn, password => $bindpw);
$result->code && die $result->error;

##
## Check for existing entries
## 
if (account_exists($handle, $username)) {
	print "Account for $username already exists!\n";
	exit 2;
}

if ($options{'u'}) {
	if (uid_exists($handle, $options{'u'})) {
		print "uid [", $options{'u'}, "] already exists!\n";
		exit 3;
	}
}
else {
	$options{'u'} = get_next_uid($handle);
	if ($options{'u'} == -1) {
		print "Unable to generate new uid!\n";
		exit 1;
	}
}

if (!defined($options{'d'})) { 
	$options{'d'} = "$home_dir$username"; 
}
elsif ($options{'d'} =~ /:/) {
	($nfs_server, $nfs_path) = split(/:/, $options{'d'});
	$options{'d'} = $nfs_path;
}
 
if (!defined($options{'s'})) { $options{'s'} = $shell; } 
if (!defined($options{'c'})) { $options{'c'} = $username; } 
if (!defined($options{'g'})) { 
	$options{'g'} = get_gid($handle, $default_group);
	if ($options{'g'} == -1) {
		warn "Unable to lookup default group [$default_group] in directory!\n";
		exit 1;	
	}
} 

##
## Check on the password
##
if ( $options{'p'} ) {
	$password = md5_base64($options{'p'}) . "==";
}


## Create the LDAP entry
$new_user = Net::LDAP::Entry->new();
$new_user->add(	'objectclass'   => [ "posixAccount", "inetOrgPerson" ],
		'uid'           => $username,
		'uidNumber'     => $options{'u'},
		'gidNumber'     => $options{'g'},
		'homeDirectory' => $options{'d'},
		'loginShell'    => $options{'s'},
		'userPassword'  => "{MD5}$password",
		'cn'            => $options{'c'},
		'sn'            => $options{'c'} );
		# 'mail'		=> "$username\@$maildomain" );

$new_user->dn( "uid=$username,$people" );
$new_user->changetype('add');

$new_user->dump();
$result = $new_user->update($handle);
$result->code && warn $result->error;

##
## Now that we've created the account, call mkdir if necessary
##
#if ( ! defined($options{'M'}) ) {
#	`ssh root\@$nfs_server "mkdir $options{'d'}" && chown $username $options{'d'}`;
#}


$handle->unbind();


