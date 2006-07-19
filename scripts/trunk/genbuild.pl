#!c:/perl/bin/perl

$temp=`sed -n 1p command.tst`;
$temp=~s/[ #\-]//g;
chomp($temp);
print 'src=$1';
print "\n";
print 'prefix=${src%%.*}';
print "\n";
system('sed -e "1d" command.tst | sed -e "/#/,$ d" | sed -e "$ d" | sed -e "s/\/lib//g" | sed -e "s/libc.c/d:\/lib\/libc.c/" | sed -e "s/intrinsic.c/d:\/lib\/intrinsic.c/" | sed -e "s/rtl.c/d:\/lib\/rtl.c/" | sed -e "s/testutil.c/d:\/lib\/testutil.c/" |'."sed -e s/"."$temp/".'$prefix/g');
print "read\n";
print 'ixp2800_c0 < $prefix.ind'."\n";
