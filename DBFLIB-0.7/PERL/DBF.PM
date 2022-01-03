package Dbf;
#
# published under the GNU Library General Public License
# rasca, berlin 1996, 1997
# rasca@mozart.komm.hdk-berlin.de, or rasca gmelch, 2:2410/305.4
#
use strict;
use vars qw($VERSION @ISA @EXPORT);

require Exporter;
require DynaLoader;

@ISA = qw(Exporter DynaLoader);
# Items to export into callers namespace by default. Note: do not export
# names by default without a very good reason. Use EXPORT_OK instead.
# Do not simply export all your public functions/methods/constants.
@EXPORT = qw(
);

$VERSION = '0.10';

bootstrap Dbf $VERSION;

# Preloaded methods go here.

# create a new object
sub new {
	my $class = shift;
	my $file = shift;
	return bless {'file'=> $file}, $class;
}


sub name {
	my $self = shift;
	return ($self->{'file'});
}


sub open {
	my $self = shift;
	my $mode = shift;

	$mode = "DB_READ" unless defined $mode;
	if (($mode eq "rw") || ($mode eq "write") || ($mode eq "DB_WRITE")) {
		$mode = "DB_WRITE";
	}
	$self->{'hnd'} = db_open($self->name(), $mode);
	if ($self->{'hnd'} > 0) {
		$self->{'is_open'} = 1;
		return 1;
	} else {
		$self->{'hnd'} = 0;
		return 0;
	}
}

sub open_dbf {
	my $self = shift;
	$self->{'file'} = shift;
	$self->{'idx'} = shift;
	return ($self->open(shift));
}


# create a new table
#
sub create {
	my $self = shift;
	my $nof  = shift;
	my $type = shift;
	if (!$nof) {
		return (0);
	}
	$type = "DB_DBASE3" unless defined $type;
	if ($type =~ /fox/i) {
		$type = "DB_FOXPRO";
	} elsif ($type =~ /db.*4/i) {
		$type = "DB_DBASE4";
	} else {
		$type = "DB_DBASE3";
	}
	$self->{'hnd'} = db_create ($self->{'file'}, $nof, $type);
	if ($self->{'hnd'} > 0) {
		$self->{'is_open'} = 1;
		return (1);
	} else {
		$self->{'hnd'} = 0;
		return (0);
	}
}


# close a table
#
sub close {
	my $self = shift;
	my $rc;
	$rc = db_close ($self->{'hnd'});
	$self->{'is_open'} = 0;
	$self->{'hnd'} = 0;
	return ($rc == 1 ? 1 : 0);
}


# same as before
sub close_dbf {
	my $self = shift;
	return ($self->close());
}

#
sub handle {
	my $self = shift;
	return ($self->{'hnd'});
}


# returns the number of records in the table
#
sub records {
	my $self = shift;
	return (db_get_num_of_recs($self->{'hnd'}));
}


# returns the number of fields in the table
#
sub fields {
	my $self = shift;
	return (db_get_num_of_fields($self->{'hnd'}));
}


sub next {
	my ($self, $go) = @_;
	my $num;
	my $fld;
	my @rec;

	$go = 1 unless defined $go; # default: after reading go to the next record
	$num = $self->fields();
	foreach $fld (0 .. $num-1) {
		$rec[$fld] = db_tr_sel_by_number($self->{'hnd'}, $fld+1, 0);
		if (!defined $rec[$fld]) {
			return;
		}
	}
	if ($go) {
		$self->skip(1);
	}
	return (@rec);
}


sub get_rec {
	my $self = shift;
	return ($self->next(0));
}


sub get_record {
	my $self = shift;
	my $num = $self->fields();
	my $fld;
	my @rec;
	foreach $fld (0 .. $num-1) {
		$rec[$fld] = db_raw_field($self->{'hnd'}, $fld+1);
		if (!defined $rec[$fld]) {
			return;
		}
	}
	return (@rec);
}


# get content of a named field
#
sub get_fld {
	my $self = shift;
	my $fld = shift;

	return (db_tr_sel_by_name($self->{'hnd'}, $fld, 0));
}


sub get_field {
	my $self = shift;
	my $fld = shift;
	my $rc;

	if ($fld eq "_DELETED") {
		return ($self->is_del());
	} else {
		$rc = db_get_field_num($self->{'hnd'}, $fld);
		if ($rc > 0) {
			return (db_raw_field($self->{'hnd'}, $rc));
		} else {
			return;
		}
	}
}


# initializise a field, call immidiatly after db->create()
#
sub init {
	my $self = shift;
	my $fnr = shift;
	my $name = shift;
	my $type = shift;
	my $length = shift;
	my $dec = shift;
	if ($length < 1) {
		if ($type eq 'N') {
			$length = 10;
		} else {
			$length = 1;
		}
	}
	$dec = 0 unless defined $dec;
	return (db_init_field($self->{'hnd'}, $fnr, $name, $type, $length, $dec));
}


# append a new blank record
#
sub append {
	my $self = shift;
	my $num = shift;
	my $rc;
	if (!$num) { $num = 1; }
	while ($num--) {
		$rc = db_append_blank($self->{'hnd'});
	}
	return ($rc);
}


sub change {
	my $self = shift;
	my $rnr = shift;
	my $fnr = shift;
	my $value = shift;
	return (db_change_field($self->{'hnd'}, $rnr, $fnr, $value));
}


sub error {
	my $self = shift;
	return (db_error());
}


sub goto {
	my $self = shift;
	my $num = shift;
	return (db_goto_nr($self->{'hnd'}, $num));
}

sub go_top {
	my $self = shift;
	return $self->goto(1);
}

sub go_bottom {
	my $self = shift;
	return ($self->goto($self->records()));
}

sub go_next {
	my $self = shift;
	return ($self->skip(1));
}

sub go_prev {
	my $self = shift;
	return ($self->skip(-1));
}

sub lastrec {
	my $self = shift;
	return ($self->records());
}

sub fld_name {
	my $self = shift;
	my $num = shift;
	return (db_get_field_name($self->{'hnd'}, $num));
}

sub fld_names {
	my $self = shift;
	my $num = $self->fields();
	my @fldnames;
	while ($num--) {
		$fldnames[$num] = $self->fld_name($num+1);
	}
	return @fldnames;
}

sub fld_type {
	my $self = shift;
	my $fldnum = shift;
	return (db_get_field_type($self->{'hnd'}, $fldnum));
}

sub fld_length {
	my $self = shift;
	my $fldnum = shift;
	return (db_get_field_length($self->{'hnd'}, $fldnum));
}


# returns the number of the named field
sub fld_num {
	my $self = shift;
	my $fld = shift;
	return (db_get_field_num ($self->{'hnd'}, $fld));
}


sub fld_dec {
	my $self = shift;
	my $fldnum = shift;
	return (db_get_field_dec($self->{'hnd'}, $fldnum));
}


sub dbf_type {
	my $self = shift;
	my $rc;

	$rc = db_type ($self->{'hnd'});
	if ($rc == 0x03 || $rc == 0x83) {
		return ("DBF3");
	} elsif ($rc == 0x8B) {
		return ("DBF4");
	} elsif ($rc == 0xF5) {
		return ("FOX");
	} else {
		return ("UNKNOWN");
	}
}


sub dbf_struct {
	my $self = shift;
	my ($n, $i);
	$n = $self->fields();
	if (!$n) {
		return;
	}
	foreach $i ("No.", "Field", "Type", "Len", "Dec") {
		print "$i\t";
	}
	print "\n";
	foreach $i (1..$n) {
		print "$i\t";
		print $self->fld_name($i) . "\t";
		print $self->fld_type($i) . "\t";
		print $self->fld_length($i) . "\t";
		print $self->fld_dec($i);
		print "\n";
	}
}


sub last_update {
	my $self = shift;
	return (db_last_update($self->{'hnd'}));
}


sub update {
	my $self = shift;
	my $num = shift;
	my @rec = @_;
	my $i;
	my $rc;
	for ($i = 1; $i < ($#rec + 2); $i++) {
		$rc = db_change_field($self->{'hnd'}, $num, $i, $rec[$i-1]);
		return if ! $rc;
	}
	return (1);
}

sub skip {
	my $self = shift;
	my $offset = shift;
	return (db_wind($self->{'hnd'}, $offset));
}

# delete a list of records
sub del {
	my $self = shift;
	my $rnr;
	foreach $rnr (@_) {
		if (db_del_record($self->{'hnd'}, $rnr) != 1) {
			return (0);
		}
	}
	return (1);
}


sub undel {
	my $self = shift;
	my $rnr;
	foreach $rnr (@_) {
		if (db_undel_record($self->{'hnd'}, $rnr) != 1) {
			return (0);
		}
	}
	return (1);
}


sub is_del {
	my $self = shift;
	my $rnr = shift;
	my $rc;
	if (!$rnr) {
		$rnr = "0";
	}
	$rc = db_is_del ($self->{'hnd'}, $rnr);
	if ($rc == 0 || $rc == 1) {
		return ($rc);
	} else {
		return;
	}
}

# returns the current record number
sub current {
	my $self = shift;
	return (db_current($self->{'hnd'}));
}

sub recno {
	my $self = shift;
	return (db_current($self->{'hnd'}));
}


sub pack {
	my $self = shift;
	return (db_pack($self->{'hnd'}));
}

sub unpack {
	my $self = shift;
	return (db_unpack($self->{'hnd'}));
}

sub eof {
	my $self = shift;
	return (db_eof($self->{'hnd'}));
}

sub locate {
	my $self = shift;
	my $fld = shift;
	my $pattern = shift;
	my $i;
	my $rc;
	my $recs = $self->records();
	$fld = db_get_field_num($self->{'hnd'}, $fld);
	if ($fld <= 0) {
		return;
	}
	$self->go_top();
	$self->{'last_field'} = $fld;
	$self->{'last_pattern'} = $pattern;

	for ($i = 0; $i < $recs; $i++) {
		$rc = db_tr_sel_by_number($self->{'hnd'}, $fld, 1);
		if ($rc =~ /^$pattern$/) {
			$self->go_prev();
			$self->{'last_locate'} = $i+1;
			return ($i+1);
		}
	}
	return;
}

sub continue {
	my $self = shift;
	my $i;
	my $rc;
	my $recs = $self->records();
	$self->goto($self->{'last_locate'} +1) || return;
	for ($i = $self->{'last_locate'}; $i < $recs; $i++) {
		$rc = db_tr_sel_by_number($self->{'hnd'}, $self->{'last_field'}, 1);
		if ($rc =~ /^$self->{'last_pattern'}$/) {
			$self->go_prev();
			$self->{'last_locate'} = $i+1;
			return ($i+1);
		}
	}	
	return;
}


sub DESTROY {
	my $self = shift;
	if ($self->{'is_open'}) {
		$self->close();
	}
}

# Autoload methods go after =cut, and are processed by the autosplit program.
1;
__END__
# Below is the stub of documentation for the module.

=head1 NAME

Dbf - Perl extension for accessing xBase files

=head1 SYNOPSIS

  use Dbf;
  $db = Dbf->new($file);
  $db->open($mode);
  $db->create($num_of_fields);
  $db->close();
  $db->open_dbf($file, $idx, $mode); *)
  $db->close_dbf(); *)
  $db->name();
  $db->records();
  $db->lastrec(); *)
  $db->fields();
  $db->handle();
  $db->next();
  $db->get_fld($fieldname);
  $db->get_field($fieldname);
  $db->get_rec();
  $db->get_record();
  $db->init($fnr, $fld, $type, $len);
  $db->append($num);
  $db->change($recnr, $fnr, $val);
  $db->error();
  $db->goto($num);
  $db->go_top();
  $db->go_bottom();
  $db->go_next();
  $db->go_prev();
  $db->fld_names();
  $db->fld_name($fld_num);
  $db->fld_type($fld_num);
  $db->fld_length($fld_num);
  $db->fld_num($fieldname);
  $db->fld_dec($fld_num);
  $db->update($recnr, @array);
  $db->skip($num);
  $db->del($num, ..);
  $db->undel($num, ..);
  $db->is_del($num);
  $db->current();
  $db->recno();  *)
  $db->pack();
  $db->unpack();
  $db->dbf_type();
  $db->dbf_struct();
  $db->last_update();
  $db->eof();
  $db->locate($field, $pattern);
  $db->continue();

=head1 DESCRIPTION

Open, create, read or write xBase files .. all methods which
return or get a record number start with "1" as the first
record (not "0" as you perhaps assume).

There are some methods which are not necessarry but are defined
to be as much as possible compatible with the Xbase module from
Pratap Pereira. These redundant methods are marked with "*)".

Index files are not supported, memo files - dbase and foxpro
- should work.

$db = Dbf->new("file_name"); creates a new object. As next you
can call the methods open() or create($number_of_fields). To define
a new table you have also to call init() after calling create() for
every field in the new xBase file! Let's show you a simple example
how to use new(), open(), records() and close():

	$db = Dbf->new("test.dbf");
	$db->open() || die "can't open xBase file: " . $db->name();
	print "Number of records: " . $db->records() . "\n";
	$db->close();

Some descriptions of available methods:

$db->open() opens the object (table). The default mode which
is used for opening is "read only". You can give the words
"rw" or "write" as the first argument to open in read-write
mode.

$db->create($num) creates a new table with $num fields. The second
optional argument is for the type of table. Default is DB_DBASE3,
use "fox" or "DB_FOXPRO" for foxpro like tables.

$db->init() this must be called directly after a create() call
for every field to define type, length etc.. E.g.

	$db->create(4);	# create a table with four fields
	$db->init(1, "ID", "N", 8);
	$db->init(2, "NAME", "C", 20);
	$db->init(3, "REMARK", "M");	# for long text entries
	$db->init(4, "FLOAT", "F", 10, 5);	# fifth arg also optional

Type could be C (string), N (number), F (float), M (memo), D (date) and
L (boolean). For boolean, date and memo fields you have not to define
the length, they will be adjusted to the length which is needed.
Optional given length for this fields will be ignored. The date field
has the form DD.MM.YYYY or DD-MM-YYYY. The fifth argument is also
optional and contains the number of dicimal places.

$db->close() closes the object (table). This will also be done
by the destructor if you forget it.

$db->handle() returns the internal handle (integer) of the table
which is needed by the library functions to reference an table.
In normal cases you don't need this method - it's for debuging
or simular ..

$db->name() returns the file name which you have defined while
creating the object with the new() method.

$db->records() gives you the number of records in the xBase
table.

$db->fields() gives you the number of fields in the xBase
table.

$db->next() reads the next record and returns it as an array. The
internal file pointer is moved to the next record. This gives you
the possibility to go through a file with a
"while (@rec = db->next()) { something ..};"
construct. If you give "0" ($go) as an argument a call will NOT move
the file pointer to the next record. The method $db->get_rec() is
the same as $db->next(0).

$db->get_fld($field_name) returns the content of the named field of
the current record. next(), get_rec() and get_fld() does some conversions
for you. If you want raw access to the field content use "get_record()"
or "get_field()".

$db->append($num) appends $num new blank records to the table. If
$num is omitted, "1" is assumend.

The method $db->change ($rnr,$fnr,$val) changes the value in the $rnr-nth
record of the $fnr-nth field to the value "$val".

$db->error() returns the last error (dberror) and resets
the internal value to "0".

$db->goto($num) goes to the record number "$num". There are also
the self-descripting methods go_top(), go_bottom(), go_next(), go_prev().

$db->fld_names() and $db->fld_name($num) return all field names
and the field name spcified by $num.

$db->fld_type($num) returns the type of the num-th field,
C, N, F, D, M and L.

$db->fld_dec($num) gives you the number of decimal places of
field $num.

$db->last_update () retuns the date of the last modification.

$db->update ($num, LIST) updates the named record. If the
LIST is shorter than the number of field of the table, only
the first fields are updated, e.g. update (1, "hi", "you") will
update the first two fields of the first record.

$db->del ($num, ..) deletes the record(s) of the LIST.

$db->undel ($num, ..) undeletes the record(s) of the LIST.

$db->skip($num) goes $num records forward or backward.

$db->is_del($num) says you if record $num is deleted or
not.

$db->current() returns the number of the current record.

$db->pack() packs a xbase file. It means all deleted records
are not available by "undel()" after this.

$db->unpack() tries to make as much as possible an undo of
pack(). Don't use it if you don't know what this makes.

$db->eof() returns 1 if the file pointer is behind the last
record.

$db->locate ($fld, $pattern) finds the first occurrence of $pattern
in $fld starting at the top of the table.

$db->continue() continues searching through the table. This method
starts behind the last found record - found by locate() or an previous
call of continue(), e.g.:

 if ($no = $db->locate("ID", "12A5")) {
	# $no holds the record no. of the first occurence .. print it
	print $db->next();
	# find next
	while ($db->continue()) {
		print $db->next();
	}
 }


=head1 AUTHOR

rasca gmelch, internet: rasca@komm.hdk-berlin.de, fidonet: 2:2410/305.4

published under the GNU Library General Public License

=head1 SEE ALSO

perl(1) and dbf(3).

=cut

