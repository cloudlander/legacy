#!/usr/bin/perl

use Config;
defined $Config{sig_name} || die "no sigs";
foreach $name (split(' ',$Config{sig_name})){
	$signame[$i++]=$name;
}
	
$|=1;

$timeout=shift @ARGV;
$pid=fork();
if(0==$pid)
{
	exec @ARGV;
}
else
{
	local $SIG{ALRM}=sub {kill 9,$pid ; waitpid $pid,0; alarm 0; print "execution timeout\n"; exit 254};
	alarm $timeout;
	waitpid $pid,0;
	if($? & 127){
		print "signal ",$signame[$?&127]," caught\n";
		exit 254;
	}
	exit $?>>8;
}
