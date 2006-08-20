#/usr/bin/perl -wT

use strict;
use File::Path;
use File::Copy;


sub copy_dir
{
    my @sub_dirs;
    my @files;

    my ($src,$dest)=@_;
    die "fatal error" if ! -d "$src";


    opendir DH,$src;        
    while (my $symbol = readdir DH) 
    {                
        next if ($symbol =~ /^\.{1,2}$/ || $symbol =~ /^\.#/ || $symbol =~ /CVS/);
        my $locate = $src."/".$symbol; 
        push @sub_dirs,$symbol if (-d $locate);
        push @files,$symbol if !(-d $locate);
#        push @sub_dirs,$locate if (-d $locate);
#        push @files,$locate if !(-d $locate);
    }        
    close DH;

    foreach my $item(@files)
    {
		&File::Copy::copy("$src/$item","$dest");
    }

    foreach my $item(@sub_dirs)
    {
    	&File::Path::mkpath("$dest/$item") if ! -e "$dest/$item";
        &copy_dir("$src/$item","$dest/$item");
    }
}

my $dest_dir="d:/SSS/";

my $proj_file=$ARGV[0];
my $proj_option_file=$proj_file;
$proj_option_file=~s/\.dwp$/\.dwo/og;

open INPUT,"$ARGV[0]" || die "Input file error\n$!";

my $proj_path;
my @files;
my @dirs;


while(<INPUT>)
{
	$proj_path=$1 if /^PROJECT_PATH = (.*)$/;
	if( /^PATH = (.*)$/ )
	{
		my $f=$1;
		$f=~s/\\/\//og;
#		print "$f\n";
		push @files,$f;
	}
	if( /^COMPILER_INCLUDE = (.*)$/ )
	{
		my $d=$1;
		$d=~s/\\/\//og;
#		print "$d\n";
		push @dirs,$d;
	}
}

$proj_path=~s/\\/\//og;

my $dest_proj_path=$proj_path;
$dest_proj_path=~s/(.*)(applications.*)/${dest_dir}\/src\/$2/og;

print "PROJECT_PATH is $proj_path\nDestination is $dest_proj_path\n";
chdir($proj_path);

&File::Path::mkpath("$dest_proj_path/list") if ! -e "$dest_proj_path/list";
&File::Path::mkpath("$dest_proj_path/log") if ! -e "$dest_proj_path/log";
&File::Path::mkpath("$dest_proj_path/streams") if ! -e "$dest_proj_path/streams";
&copy_dir("streams","$dest_proj_path/streams");
&File::Copy::copy("$proj_file","$dest_proj_path/$proj_file");
&File::Copy::copy("$proj_option_file","$dest_proj_path/$proj_option_file");


foreach my $file(@files){
	$file=~/(.*)\/[^\/]*$/;
	my $dir=$1;
    &File::Path::mkpath("$dest_proj_path/$dir") if ! -e "$dest_proj_path/$dir";
    &File::Copy::copy("$file","$dest_proj_path/$dir");
	if($file=~/\.c$/io)
	{
		open INPUTC,"$file";
		while(<INPUTC>)
		{
			if(/^#include \"(.*)\"/)
			{
				my $h_file=$1;
				&File::Copy::copy("$dir/$h_file","$dest_proj_path/$dir") if -e "$dir/$h_file";
			}
		}
		close INPUTC;
	}
}
foreach my $dir(@dirs){
	$dir=~/(.*)\/[^\/]*$/;
	my $dir1=$1;
    &File::Path::mkpath("$dest_proj_path/$dir1") if ! -e "$dest_proj_path/$dir1";
	&copy_dir($dir,"$dest_proj_path/$dir1");
}
