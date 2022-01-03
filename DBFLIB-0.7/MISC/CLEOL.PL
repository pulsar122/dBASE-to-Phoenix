#!/usr/bin/perl
#
# change \r\n to <BR>, sometimes needed with
# memo-files /fields ...
# rasca, berlin 1996
#
# call e.g. (assuming there is also foo.fpt) "dbfconv foo.dbf | cleol.pl"
#

while (<>) {
	s/\r\n/<BR>/g;
	s/ +;/;/g;
	print;
}

