#!/usr/bin/perl
#
# strip empty lines from the beginning of an ascii file
# needed by "make html" and "make dbf.txt"
#

$begin = 0;
while (<>) {
	if (!$begin && $_ ne "\n")
		{ $begin = 1; print "\n";}
	if ($begin) { print; }
}
