#!/usr/bin/perl

open INPUT,"$ARGV[0]";
my @y;
my %y;
while(<INPUT>)
{
	if(/Call Stack/)
	{
		my $t;
		$f=0;
		while(<INPUT>)
		{
			if(/IXP_/)
			{
				$t=$_;
				$f=1;
				last;
			}
			if(/Call Stack/)
			{
				next;
			}
		}
		if($f)
		{
			$f=0;
			foreach $n(@y)
			{
				if($n eq $t)
				{
					$f=1;
					last;
				}
			}
			if(!$f)
			{
				push @y,$t;
				$t=~/(.*) \((\d+)\):/;
				$x2=$2;
				$x1=$1;
				$x1=~s/^\s*//;
#				print "$t\n";
				if($x2>0)
				{
					$x2--;
					$com="perl -n -e \"print ".'$_ if (++$i)=='."$x2"."\" $x1";
#					print "$com\n";
					$y{$t}=`$com`;
					$x2++;
				}
				$com="perl -n -e \"print ".'$_ if (++$i)=='."$x2"."\" $x1";
#					print "$com\n";
				$y{$t}.=`$com`;
				if($x2 < `wc -l $x1`)
				{
					$x2++;
					$com="perl -n -e \"print ".'$_ if (++$i)=='."$x2"."\" $x1";
#					print "$com\n";
					$y{$t}.=`$com`;
				}
			}
		}
		$_=<INPUT>;
	}
}
foreach $y(@y)
{
	print "/*****/ $y";
	print "$y{$y}\n";
}
