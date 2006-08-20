#!/usr/bin/perl 

use strict;
my ($a,$found,$incom,$pdir);
$a=$ARGV[0];
if($a ne "."){
#	system("grep \"declspec(local_mem)\" $a >/dev/null 2>&1");
#=comment
	open INPUT,"<$a";
	$found=0;
	$incom=0;
	while(<INPUT>){
		s#//.*##g;
		s#/\*(.*?)\*/##g;
		$incom=1  if(/\/\*/);
		$incom=0  if($incom==1 && /\*\//);
		$found=1, last if (/declspec\(local_mem\)/ && 0==$incom);
	}
	close INPUT;
#=cut	
#	$found  = $? >> 8;
	if($found){
		print "$a FOUND\n";
		$pdir=$a;
		$pdir=~s/(.*\/)(.*)/$1/;
		if(-d "../ControlStore/$pdir")
		{
		}
		else{
			system("mkdir -p ../ControlStore/$pdir");
		}
		system("cp  $a ../ControlStore/$pdir 2>/dev/null");
	}
}

