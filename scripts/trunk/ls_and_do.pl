$|=1;
open INPUT, "$ARGV[0]";
open OUTPUT,">../errata_ren/cs_$ARGV[0]";

$patt="cs_$ARGV[0]";
$name=$ARGV[0];
print "name=$name,patt=$patt\n";
$data=0;
while(<INPUT>){
	if( /DATA_START/){
 		$data=1;
	   	print OUTPUT"$_";
		next;
	}	
	if( /DATA_END/){
		$data=0;
	   	print OUTPUT"$_";
		next;
	}	
	if($data){
		print "<<<  $_";
		~s/$name/$patt/;
		print ">>>  $_";
		print OUTPUT"$_";
		next;
	}	
	print OUTPUT"$_";
}
die "fatal error" if $data;
close INPUT;
close OUTPUT;

=cut
$a=`grep -n 364 $ARGV[0]`;
if($a eq "")
{
	system("cat $ARGV[0] >> all");
	exit(0);
}
@a=split(/\n/,$a);
$n=$a[$#a];
$n=$n-1;
system('sed -e "1,'."$n".' d"'." $ARGV[0]".' >> all');
=cut
