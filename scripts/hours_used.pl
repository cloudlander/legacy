#!/usr/bin/perl

use strict;
use LWP::UserAgent;
use HTML::TokeParser;
use Time::HiRes qw/gettimeofday/;
use Time::Local;
use POSIX qw/strftime/;

my $ua;
$ua=LWP::UserAgent->new;
$ua->agent("BillChecker/0.1");
$ua->timeout(1300);

my ($time,$ms)=gettimeofday;
my @ltime=localtime($time);
my $ltimebuf=sprintf("%d-%d-%d",$ltime[5]+1900,$ltime[4]+1,$ltime[3]);
print "Today is $ltimebuf\n";
my $total=0;
my $end_t=time;
my $start_t=timelocal(0,0,0,1,$ltime[4],$ltime[5]);
print "Start Time: $start_t\nEnd Time: $end_t\n";
my $proxy=$ENV{http_proxy};
$proxy=$ENV{HTTP_PROXY} if ! defined ($proxy);
print <<EOF
1.	Use Proxy: $proxy
2.	Connect Directly (Default)
Select your option:
EOF
;

my $use_proxy=<STDIN>;
$ua->proxy(["http"],$proxy) if($use_proxy == 1);
&run("http://202.109.10.106:8080/adslquery.php4?login=ad54175533&jqlogin=a&ram1=1080203660&ram2=2206490&acct_id0=46554&bill_id0=0000&start_t=$start_t&end_t=$end_t&yhlx=M10001&serv_lx=0");
print "Total: $total hours\n";

sub run($)
{
	my $webroot=$_[0];
	while(1)
	{
		my $res = &getnext($webroot);
		if($res =~/所有数据都已经为你显示完毕/)
		{
			return;	
		}	
		if($res =~/本页使用费用累计:(.*)小时/)
		{
#			print $1,"\n";
			$total += $1;
			$res =~/ACTION="(.*)" METHOD/;
#			print $1,"\n";
			$webroot=$1;
			next;
		}
	}
}

sub getnext($)
{
    my $webroot=$_[0];
    my ($req,$res,$stream,$token);
    print "fetching $webroot\n";
    $req=HTTP::Request->new(GET=>$webroot);
    $res=$ua->request($req);
#    print $res->content;
	return $res->content;
}
print "Press any key to exit\n";
<STDIN>
