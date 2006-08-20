#!/usr/bin/perl 

=usage:
    This script fetches the locally nonexistent latest check-ins of specified authors, and
will produce a html output file for reading easilly.
    If the GNU source-highlight program has been installed, this script will also produce
source highlighted HTML files under the same output directory.

command line switchs:
  -r branch-name :      specify the branch to process (default is branch-castineL,passing "none" to omit branch)
  -w user{,user} :      comma separated (no spaces or tabs allowed) user names to specify whose 
                        check-ins to fetch (default is gwei2,ahsieh)
  -o file :             name of the output html file (default is show.htm)
  -d dir :	        specify the directory to place the fetched files (default is ./fetched_files/)
=cut

use Getopt::Long;
use strict;

my @auths=();
my @ignores=();
my $branch="branch-castineL";
my $output_file="show.htm";
my $output_dir="./fetched_files";
my $source_highlight=`which source-highlight`;
$source_highlight="source-highlight" if $source_highlight;

my ($file,$rev,$org_rev,$date,$auth,$log,$cur_rev,$not_latest);

&get_opts;
my $cvslog="cvs log $branch -N";
print $cvslog."\n";
system("$cvslog > log.$$ 2>warn.$$");
open WARN,"<warn.$$";
while(<WARN>){
	if(/no revision/){
		split;
		$_[7] =~s/["'"`]|\/Attic//g;	# just make source-highlight happy ['`] is ok
		push ( @ignores,$_[7] ) if !&filt_file($_[7]);
	}
}
close WARN;
open INPUT,"<log.$$" || die;
open OUTPUT,">$output_file" || die;
print OUTPUT"<table cellSpacing=1 cellPadding=1 border=1 width=100%>\n";
print OUTPUT"<tr valign=top bgColor=turquoise> <td><strong>File(Original)</strong></td> <td><strong>Latest Revision</strong></td> <td><strong>Author</strong></td> <td><strong>Check-in time</strong></td> <td><strong>Log Message</strong></td></tr>\n";
while(<INPUT>){
	next if !/RCS file/;
	$_=<INPUT>;split;
	$rev="";
	$file=$_[2];
	next if &filt_file($file);
#	print "\nfile: $file\n";
	$not_latest=0;
	my $found=0;
	my $pdir=$file;
	my $tmp= `basename $file`;
	chomp $tmp;
	if ($file eq $tmp){
		$pdir="";
	}
	else{
		$pdir=~s/(.*)\/.*/$1/;
	}
	$_=<INPUT>;
	$_=<INPUT> until /--------/ || /========/;
	next if /========/;
	while(<INPUT>){
		split if /revision/;
		$cur_rev=$_[1];
		if($found){
			$_=<INPUT> until /--------/ || /========/;
			last if /========/;
			next;
		}
		if(!$rev){
			$rev=$cur_rev;
			print "latest rev: $rev\n";
		}
		$_=<INPUT>;split;
		chop $_[4];
		$auth=$_[4];
		chop $_[2];
		$date=$_[1]." ".$_[2];
		$log="";
		$_=<INPUT>;
		$log=$_ if !/branches/;
		$_=<INPUT>;
		until(/--------/ || /========/){
			$log=$log.$_;
			$_=<INPUT>;
		}
		$tmp=$_;
		if(&check_auth($auth)){
			$found=1;
			print "$file\'s latest revision ($cur_rev) checked in by author $auth found: $date , $log";
			if ($rev ne $cur_rev){
				print "Warning: not matching to the latest revision ($rev)\n";
				$rev=$cur_rev;
				$not_latest=1;
			}
			system("mkdir -p $output_dir/$pdir");
			&fetch_file($file,$cur_rev);
		}
		last if $tmp=~/========/;
	}
	$org_rev=$cur_rev;
#	print $org_rev."\n";
	if($found){
		&fetch_file($file,$org_rev);
		print "\n";
		&do_output;
	}
}
print OUTPUT"</table>";
close OUTPUT;
close INPUT;
unlink("log.$$");
unlink("warn.$$");

sub get_opts
{
	my $auths="gwei2,ahsieh";
	my $result=GetOptions("r=s" => \$branch,
			      "w=s" => \$auths,
			      "o=s" => \$output_file,
			      "d=s" => \$output_dir);
	if($branch eq "none"){
		$branch="-b";
		print "Start fetching revisions of MAIN trunk";
	}
	else{
		print "Start fetching revisions of $branch";
		$branch="-r".$branch;
	}
	split(",",$auths);
	push @auths,@_;
	print " checked-in by ".join(",",@auths)."\n";
	print "Output files will be in $output_dir,please see $output_file for details\n";
	print "Source highlighted files will be produced\n" if $source_highlight;
}

sub fetch_file
{
	my ($file_to_fetch,$rev)=@_;
	if (!-e "$output_dir/$file_to_fetch-$rev" || -z "$output_dir/$file_to_fetch-$rev"){ 
		print "Fetching revision $rev of $file_to_fetch ...\n";
		system("cvs -q  up -r$rev -p $file_to_fetch > $output_dir/$file_to_fetch-$rev");
	}
	if (!-e "$output_dir/$file_to_fetch-$rev.html" || -z "$output_dir/$file_to_fetch-$rev.html"){ 
		my $format="cpp";
		$_=$file_to_fetch;
		~m/.*\.(.*)/;
		$format="perl" if $1 eq "pl" || $1 eq "plx";
		$format="python" if $1 eq "py";
# add more supported format here
		system("$source_highlight --line-number-ref --doc -s $format -f html $output_dir/$file_to_fetch-$rev") if $source_highlight;
	}
}

sub filt_file
{
	my ($file_to_check)=@_;
	foreach my $val (@ignores){
		return 1  if $val =~/$file_to_check/;
	}
	0;
}

sub check_auth
{
	my ($auth_to_check)=@_;
	foreach my $val (@auths){
		return 1  if $val eq $auth_to_check;
	}
	0;
}
{
my $i=0;
sub do_output
{
	my $taged_file="<a href=$output_dir/$file-$org_rev>$file</a>";
	$taged_file.="&nbsp&nbsp<a href=$output_dir/$file-$org_rev.html>HTML</a>" if $source_highlight;
	my $taged_latest="<a href=$output_dir/$file-$rev>$rev</a>";
	$taged_latest.="&nbsp&nbsp<a href=$output_dir/$file-$rev.html>HTML</a>" if $source_highlight;
	$taged_latest.="&nbsp&nbsp<font color=red>*</font>" if $not_latest;
	my $taged_auth="<a href=mailto:$auth>$auth</a>";
	my $bgColor="lightgrey";
	$bgColor="palegoldenrod" if $i%2;
	$i++;
	print OUTPUT"<tr valign=top bgColor=$bgColor> <td>$taged_file</td> <td>$taged_latest</td> <td>$taged_auth</td> <td>$date</td> <td>$log</td></tr>\n";
	1;
}
}
