@arr=(0,1,2,3,4,5,6);

sub permute()
{
	my @arr=@_;

	if($#arr==0)
	{
		return (\@arr);
	}
	else
	{
		my @perming;
		my $head=$arr[0];
		shift @arr;
		my @permed=&permute(@arr);
		foreach my $refArr(@permed)
		{
			for(my $i=0;$i<=$#$refArr+1;$i++)
			{
				my @buf=@$refArr;
				splice(@buf,$i,0,$head);
				push(@perming,\@buf);
			}
		}
		return @perming;
	}
}

@permed=&permute(@arr);
foreach my $refArr(@permed)
{
	foreach my $i(@$refArr)
	{
		print "$i\t";
	}
	print "\n";
}
