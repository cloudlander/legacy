my $PERL_POST_CMD="c:/perl/bin/post.bat";
my $selfhelp_submit_url="http://sh1sdev195:8080/webtime/servlet/Task";

my %params;
$params{'test_type'}='ixpcDebugCheckerTest';
$params{'compiler_branch'}='develop_branch';
#$params{'selected_apps'}=['tcp_offload','ipv4_diffserv-oc192_pos-egress'];
$params{'selected_apps'}=['ALL'];
$params{'user_name'}='Zhang, Xiaomin';
$params{'user_email'}='xiaomin.zhang@intel.com';
$params{'user_dir'}='\\\\shxpwjzhuan2\incoming\DGI\opt';
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

open (OPIPE,"|$PERL_POST_CMD $selfhelp_submit_url");
print OPIPE"$post_data";
close OPIPE;
