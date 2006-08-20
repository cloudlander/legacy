#!c:/perl/bin/perl
use strict;
die "MECAREA not found!" if ! defined $ENV{MECAREA};
print "Your MECAREA is $ENV{MECAREA}\n";
my $SRC="$ENV{MECAREA}/dev/ixp/ixp_cg";
my $CODECOVDIR="$ENV{MECAREA}/CodeCovResult/CodeCoverage";

my @all_uncvrd=`ls $CODECOVDIR/*UNCVRDFNCS_2*.HTML`;
my %uncvrd_func;
my @content;
my %fake_uncvrd_func;	# those filted

my $count=0;
#&dump_all;die;
foreach my $uncvrd_file(@all_uncvrd){
	chomp $uncvrd_file;
	
	my $uncvrd_file0=$uncvrd_file;
	$uncvrd_file0=~s/2.HTML/0.HTML/;
	my $uncvrd_file4=$uncvrd_file;
	$uncvrd_file4=~s/2.HTML/4.HTML/;
	my $uncvrd_file6=$uncvrd_file;
	$uncvrd_file6=~s/2.HTML/6.HTML/;
	
	my $srcfile=`grep "<title>.*</title>" $uncvrd_file`;	# get src file name
	$srcfile=~m/<title>(.*)<\/title>/;
	$srcfile=$1;
	$srcfile=~s/(.*\\|.*\/)(.*)/$2/;
	$srcfile=~tr/[a-z]/[A-Z]/;
	print $srcfile."\n";

	# get all uncovered functions
	my @blocks;
	@content=`grep "Line" $uncvrd_file`;
	@blocks=`grep -P "[0-9]+<\/td" $uncvrd_file`;
#=cut
	`cp $uncvrd_file $uncvrd_file.bak`;					# use sed to replace unreferenced function
	`cp $uncvrd_file0 $uncvrd_file0.bak`;				# use sed to replace unreferenced function
	`cp $uncvrd_file4 $uncvrd_file4.bak`;				# use sed to replace unreferenced function
	`cp $uncvrd_file6 $uncvrd_file6.bak`;				# use sed to replace unreferenced function
#=cut
	my $index=0;
	foreach my $c(@content){					
		$c=~m/.*>([_0-9a-zA-Z]*)<\/a><\/td>/;		# get uncovered function
		my $uncvrd_func=$1;
		$c=~m/.*Line ([0-9]+).*/;					
		my $uncvrd_line=$1;						
		# scan to find the ending declaration line
		my $uncvrd_line_end=&getEndLine("$SRC/$srcfile",$uncvrd_line);
#		print "$uncvrd_func: $uncvrd_line,$uncvrd_line_end\n";
	
		# get uncorved blocks of this function
		my $buf=$blocks[$index];
		$buf=~m/.*>([0-9]+)<.*/;
		my $uncvrd_blocks=$1;
		$index++;
		
		my @content_tmp=`grep -n -H "$uncvrd_func(" $SRC/*.c $SRC/*.h`;
		my @refFiles=&getAllRefs(@content_tmp);
		
		if(0==scalar(@refFiles)){
			push(@{$fake_uncvrd_func{$srcfile}},{"func"=>$uncvrd_func,"start_line"=>$uncvrd_line,"end_line"=>$uncvrd_line_end,"blocks"=>$uncvrd_blocks});
			print "$srcfile:$uncvrd_func filted!\n";
#=cut
			system("sed -e \"s/$uncvrd_func/<font color=red>*<\\/font>$uncvrd_func/\" $uncvrd_file > $uncvrd_file.bak");
			system("sed -e \"s/$uncvrd_func/<font color=red>*<\\/font>$uncvrd_func/\" $uncvrd_file0 > $uncvrd_file0.bak");
			system("sed -e \"s/$uncvrd_func/<font color=red>*<\\/font>$uncvrd_func/\" $uncvrd_file4 > $uncvrd_file4.bak");
			system("sed -e \"s/$uncvrd_func/<font color=red>*<\\/font>$uncvrd_func/\" $uncvrd_file6 > $uncvrd_file6.bak");
			`cp $uncvrd_file.bak $uncvrd_file`;					# use sed to replace unreferenced function
			`cp $uncvrd_file0.bak $uncvrd_file0`;				# use sed to replace unreferenced function
			`cp $uncvrd_file4.bak $uncvrd_file4`;				# use sed to replace unreferenced function
			`cp $uncvrd_file6.bak $uncvrd_file6`;				# use sed to replace unreferenced function
#=cut
			next;
		}
		push(@{$uncvrd_func{$srcfile}},{"func"=>$uncvrd_func,"start_line"=>$uncvrd_line,"end_line"=>$uncvrd_line_end,"blocks"=>$uncvrd_blocks,"refs"=>\@refFiles});
	}
	
	`rm $uncvrd_file.bak $uncvrd_file0.bak $uncvrd_file4.bak $uncvrd_file6.bak`;

	$count++;
#	last if $count==6;
}

&cleanResult;

&dump_all;

sub cleanResult
{
	my $changed;
	print "Cleaning\n";
	while(1){
		$changed=0;
		foreach my $file (keys %uncvrd_func){
#			print "$file:\n";
			my $index=0;
			my @indexToRemove;
			foreach my $item (@{$uncvrd_func{$file}}){
#				print "checking func: $item->{func}\n";
=cut
				print "start: $item->{start_line}\n";
				print "end: $item->{end_line}\n";
				print "blocks: $item->{blocks}\n";
				print "refs: \n";
=cut
				my $isFakeRef=1;
				foreach my $ref(@{$item->{refs}}){
=cut
					print "\treffile:$ref->{file}\n";
					print "\tline: $ref->{line}\n";
					print "\tcontent: $ref->{ref}\n";
=cut
					if(!&isFakeRef($ref->{file},$ref->{line})){
						$isFakeRef=0;
						last;
					}
					else{
#						print "$ref->{file}:$ref->{line} is fake\n";
					}
				}
				if($isFakeRef){
						print("new fake uncvrd func found:$item->{func}\n");
						push(@{$fake_uncvrd_func{$file}},$item);
						push(@indexToRemove,$index);
				}
				$index++;
			}
			foreach my $i(reverse @indexToRemove){
				splice(@{$uncvrd_func{$file}},$i,1);
				$changed=1;
			}
#			print "\n";
		}
		foreach my $file(keys %uncvrd_func){
			if(exists($uncvrd_func{$file}) && !scalar(@{$uncvrd_func{$file}})){
				delete $uncvrd_func{$file};
			}
		}
		last if ! $changed;
	}
}

sub isFakeRef
{
	my ($file,$line)=@_;
	foreach my $key (keys %fake_uncvrd_func ){
		my $buf=$key;
		$buf=~tr/A-Z/a-z/;
		if($file eq "$SRC/$buf"){
			foreach my $item (@{$fake_uncvrd_func{$key}}){
				if($item->{start_line}<$line && $line<$item->{end_line}){
#					print "$item->{start_line} < $line && $line<$item->{end_line}\n";
					return 1;
				}
			}
		}
	}
	return 0;
}

sub getAllRefs
{
	my @content=@_;
	my $isStatic=0;
	my @refFiles;
	foreach my $tmp(@content){
		# filt comments
		$tmp=~s#//.*##g;
		$tmp=~s#/\*(.*?)\*/##g;
#		print $tmp."\n";
#	
		my @all=split(':',$tmp);
		my $file=$all[0].":".$all[1];
		my $line=$all[2];
		my $ref=$all[3];
		for(my $i=4;$i<= $#all;$i++){
			$ref.=":$all[$i]";
		}

		# sanity test
		if( "$file:$line:$ref" ne $tmp)
		{
			print "$file,$line,$ref"."\n";
			print $tmp;
			die "sanity test failed";
		}
		next if $ref=~/^[ \t\n\r]*$/;
		#filt declaration statements
		next if $ref =~/extern/;
		if( $ref =~/static/ ){
			$isStatic=1;
			next;
		} 
		next if( $ref =~/[_a-zA-Z0-9]+[\* ]+[_a-zA-Z0-9]+/ && $ref !~/=|\"|\[|\]|return|#define/);
		push(@refFiles,{"file"=>$file,"line"=>$line,"ref"=>$ref});
	}
	
	return @refFiles;
}

sub dump_all
{
	foreach my $key (keys %uncvrd_func ){
		print "$key:\n";
		foreach my $item (@{$uncvrd_func{$key}}){
			print "func: $item->{func}\n";
			print "start: $item->{start_line}\n";
			print "end: $item->{end_line}\n";
			print "blocks: $item->{blocks}\n";
			print "refs: \n";
			foreach my $ref(@{$item->{refs}}){
				print "\treffile:$ref->{file}\n";
				print "\tline: $ref->{line}\n";
				print "\tcontent: $ref->{ref}\n";
			}
		}
		print "\n";
	}
	
	my $unref_funcs=0;
	my $unref_blocks=0;

	print "These functions filted!\n";
	foreach my $key (keys %fake_uncvrd_func ){
#=cut
		`cp $CODECOVDIR/__COVERED_FILES_COVERAGE_0.HTML $CODECOVDIR/__COVERED_FILES_COVERAGE_0.HTML.$$`;
		`cp $CODECOVDIR/__COVERED_FILES_COVERAGE_1.HTML $CODECOVDIR/__COVERED_FILES_COVERAGE_1.HTML.$$`;
		`cp $CODECOVDIR/__COVERED_FILES_COVERAGE_2.HTML $CODECOVDIR/__COVERED_FILES_COVERAGE_2.HTML.$$`;
		`cp $CODECOVDIR/__COVERED_FILES_COVERAGE_4.HTML $CODECOVDIR/__COVERED_FILES_COVERAGE_4.HTML.$$`;
		`cp $CODECOVDIR/__COVERED_FILES_COVERAGE_5.HTML $CODECOVDIR/__COVERED_FILES_COVERAGE_5.HTML.$$`;
		`cp $CODECOVDIR/__COVERED_FILES_COVERAGE_6.HTML $CODECOVDIR/__COVERED_FILES_COVERAGE_6.HTML.$$`;
#=cut
		print "$key:\n";
		foreach my $item (@{$fake_uncvrd_func{$key}}){
			print "func: $item->{func}\n";
=cut
			print "start: $item->{start_line}\n";
			print "end: $item->{end_line}\n";
			print "blocks: $item->{blocks}\n";
			print "refs: \n";
			foreach my $ref(@{$item->{refs}}){
				print "\treffile:$ref->{file}\n";
				print "\tline: $ref->{line}\n";
				print "\tcontent: $ref->{ref}\n";
			}
=cut
			$unref_funcs++;
			$unref_blocks+=$item->{blocks};
			system("sed -e \"s/>$key</><font color=red>*<\\\/font>$key</i\" $CODECOVDIR/__COVERED_FILES_COVERAGE_0.HTML > $CODECOVDIR/__COVERED_FILES_COVERAGE_0.HTML.$$");
			system("sed -e \"s/>$key</><font color=red>*<\\\/font>$key</i\" $CODECOVDIR/__COVERED_FILES_COVERAGE_1.HTML > $CODECOVDIR/__COVERED_FILES_COVERAGE_1.HTML.$$");
			system("sed -e \"s/>$key</><font color=red>*<\\\/font>$key</i\" $CODECOVDIR/__COVERED_FILES_COVERAGE_2.HTML > $CODECOVDIR/__COVERED_FILES_COVERAGE_2.HTML.$$");
			system("sed -e \"s/>$key</><font color=red>*<\\\/font>$key</i\" $CODECOVDIR/__COVERED_FILES_COVERAGE_4.HTML > $CODECOVDIR/__COVERED_FILES_COVERAGE_4.HTML.$$");
			system("sed -e \"s/>$key</><font color=red>*<\\\/font>$key</i\" $CODECOVDIR/__COVERED_FILES_COVERAGE_5.HTML > $CODECOVDIR/__COVERED_FILES_COVERAGE_5.HTML.$$");
			system("sed -e \"s/>$key</><font color=red>*<\\\/font>$key</i\" $CODECOVDIR/__COVERED_FILES_COVERAGE_6.HTML > $CODECOVDIR/__COVERED_FILES_COVERAGE_6.HTML.$$");
		}
#=cut
		`mv $CODECOVDIR/__COVERED_FILES_COVERAGE_0.HTML.$$ $CODECOVDIR/__COVERED_FILES_COVERAGE_0.HTML`;
		`mv $CODECOVDIR/__COVERED_FILES_COVERAGE_1.HTML.$$ $CODECOVDIR/__COVERED_FILES_COVERAGE_1.HTML`;
		`mv $CODECOVDIR/__COVERED_FILES_COVERAGE_2.HTML.$$ $CODECOVDIR/__COVERED_FILES_COVERAGE_2.HTML`;
		`mv $CODECOVDIR/__COVERED_FILES_COVERAGE_4.HTML.$$ $CODECOVDIR/__COVERED_FILES_COVERAGE_4.HTML`;
		`mv $CODECOVDIR/__COVERED_FILES_COVERAGE_5.HTML.$$ $CODECOVDIR/__COVERED_FILES_COVERAGE_5.HTML`;
		`mv $CODECOVDIR/__COVERED_FILES_COVERAGE_6.HTML.$$ $CODECOVDIR/__COVERED_FILES_COVERAGE_6.HTML`;
#=cut
		print "\n";
	}
#=cut
	if($unref_funcs==0){
		die if $unref_blocks;
		return;
	}
	if($unref_blocks==0){
		die if $unref_funcs;
		return;
	}
	`cp $CODECOVDIR/__CODE_COVERAGE.HTML.bak $CODECOVDIR/__CODE_COVERAGE.HTML` if -f "$CODECOVDIR/__CODE_COVERAGE.HTML.bak" ;
	# get original stat
	my $org_tot_funcs;
	my $org_cov_funcs;
	my $org_tot_blocks;
	my $org_cov_blocks;
	my @stat=`grep "</TD>" $CODECOVDIR/__CODE_COVERAGE.HTML`;
	s/[^0-9]//g foreach (@stat);
	$org_tot_funcs=$stat[4];
	$org_cov_funcs=$stat[5];
	$org_tot_blocks=$stat[8];
	$org_cov_blocks=$stat[9];

	my $func_cov_rate=sprintf("%.2f",($org_cov_funcs+$unref_funcs)*100/$org_tot_funcs);
	my $block_cov_rate=sprintf("%.2f",($org_cov_blocks+$unref_blocks)*100/$org_tot_blocks);
	
	`cp $CODECOVDIR/__CODE_COVERAGE.HTML $CODECOVDIR/__CODE_COVERAGE.HTML.$$`;
	`cp $CODECOVDIR/__CODE_COVERAGE.HTML $CODECOVDIR/__CODE_COVERAGE.HTML.bak`;
	system("sed -e \"30,31s/4/6/\" -e \"40 a <TH ALIGN=\\\"center\\\">unrefered</TH>\" -e \"41 a <TH ALIGN=\\\"center\\\">actual\%</TH>\" -e \"44 a <TH ALIGN=\\\"center\\\">unrefered</TH>\" -e \"45 a <TH ALIGN=\\\"center\\\">actual\%</TH>\" -e \"56 a <TD ALIGN=\\\"center\\\">$unref_funcs</TD>\" -e \"57 a <TD ALIGN=\\\"center\\\" STYLE=\"font-weight:bold\">$func_cov_rate\%</TD>\" -e \"60 a <TD ALIGN=\\\"center\\\">$unref_blocks</TD>\" -e \"61 a <TD ALIGN=\\\"center\\\" STYLE=\"font-weight:bold\">$block_cov_rate\%</TD>\" $CODECOVDIR/__CODE_COVERAGE.HTML > $CODECOVDIR/__CODE_COVERAGE.HTML.$$");
	`mv $CODECOVDIR/__CODE_COVERAGE.HTML.$$ $CODECOVDIR/__CODE_COVERAGE.HTML`;
#=cut
}

sub getEndLine
{
	my ($src,$line)=@_;
	open INPUT,$src;					
	my $curLine=0;
	while(<INPUT>){
		$curLine++;
		last if $curLine==$line;
	}
	die $_ unless /{/;
	my $inclosure=1;
	while(<INPUT>){
		$curLine++;
		$inclosure++ if /{/;
		$inclosure-- if /}/;
		last if 0==$inclosure;
	}
	$curLine;
}	
