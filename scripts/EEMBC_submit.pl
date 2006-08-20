my $PERL_POST_CMD="c:/perl/bin/post.bat";
my $selfhelp_submit_url="http://sh1sdev149:8080/webtime/servlet/Task";

my %params;
$params{'test_type'}='msaEEMBC1_1Test';
$params{'compiler_branch'}='MSA4';
#$params{'selected_apps'}=['tcp_offload','ipv4_diffserv-oc192_pos-egress'];
#$params{'selected_apps'}=['ALL'];
$params{'submitter_group'}="QA";
$params{'compiler_type'}="both";
$params{'user_name'}='EEMBC_Daily';
$params{'user_email'}='xin.hao@intel.com';
$params{'user_dir'}='\\\\sh1sdev145\\msa_builds\\RCT_SHARED\\builds\\today';
$params{'test_description'}='just a try';

$params{'test_type'}=~s/([^_\-\.\@A-Za-z0-9])/sprintf("%%%02X", ord($1))/seg;
my $post_data="test_type=$params{'test_type'}";

my $refArray=$params{'selected_apps'};
foreach my $app (@$refArray)
{
	$app=~s/([^_\-\.\@A-Za-z0-9])/sprintf("%%%02X", ord($1))/seg;
	$post_data.="&selected_apps=$app";
}

foreach my $name (keys %params)
{
	next if $name eq 'test_type' || $name eq 'selected_apps';
	$name=~s/([^_\-\.\@A-Za-z0-9])/sprintf("%%%02X", ord($1))/seg;
	$params{$name}=~s/([^_\-\.\@A-Za-z0-9])/sprintf("%%%02X", ord($1))/seg;
	$post_data.="&$name=$params{$name}";
}
print "$post_data\n";
open (OPIPE,"|$PERL_POST_CMD $selfhelp_submit_url");
print OPIPE"$post_data";
close OPIPE;
