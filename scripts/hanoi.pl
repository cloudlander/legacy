sub hanoi
{
	my ($A,$B,$C,$n)=@_;
	if($n==1)
	{
		print "$A -> $C\n";
		return;
	}
	&hanoi($A,$C,$B,$n-1);
	print "$A -> $C\n";
	&hanoi($B,$A,$C,$n-1);
}
&hanoi("A","B","C",$ARGV[0]);
