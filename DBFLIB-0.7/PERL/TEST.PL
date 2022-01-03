# Before `make install' is performed this script should be runnable with
# `make test'. After `make install' it should work as `perl test.pl'

######################### We start with some black magic to print on failure.

# Change 1..1 below to 1..last_test_to_print .
# (It may become useful if the test is moved to ./t subdirectory.)

BEGIN { $| = 1; print "1..37\n"; }
END {print "not ok 1\n" unless $loaded;}
use Dbf;
$loaded = 1;
print "ok 1\n";

######################### End of black magic.

# Insert your test code below (better if it prints "ok 13"
# (correspondingly "not ok 13") depending on the success of chunk 13
# of the test code):

$file = "test.dbf";
$i = 2;
$hnd = &Dbf::db_open ($file, "DB_READ");
print $hnd > 0 ? "ok $i\n" : "not ok $i\n";
$i++;
$rc = &Dbf::db_tr_str_next ($hnd, ";");
print $rc ne "" ? "ok $i\n" : "not ok $i\n";
$i++;
$hnd = &Dbf::db_close ($hnd);
print $hnd > 0 ? "ok $i\n" : "not ok $i\n";
$i++;

# some tests with the class/object
#
$db = Dbf->new($file);
print $db ? "ok $i\n" : "not ok $i OBJ->new()\n";
$i++;
$rc = $db->name();
print $rc ? "ok $i\n" : "not ok $i OBJ->name(): \"$rc\"\n";
$i++;

print $db->open()	? "ok $i\n" : "not ok $i OBJ->open()\n";
$i++;
print $db->close()	? "ok $i\n" : "not ok $i OBJ->close()\n";
$i++;

$db->open();
$db->error();
$rc = $db->records();
print $db->error() == 0	? "ok $i\n" : "not ok $i OBJ->records(): $rc\n";
$i++;
$rc = $db->fields();
print $rc > 0 ? "ok $i\n" : "not ok $i OBJ->fields(): rc = $rc\n";
$i++;
print $db->handle() ? "ok $i\n" : "not ok $i OBJ->handle()\n";
$i++;
@rc = $db->next();
print $#rc == ($db->fields() - 1) ? "ok $i\n" : "not ok $i OBJ->next()\n";
$i++;
$db->close();
$db = Dbf->new("test.dbf");
print $db->create(7) ? "ok $i\n" : "not ok $i OBJ->create(4)\n";
$i++;
print $db->init(1, "id", "N", 4) ? "ok $i\n" : "not ok $i  OBJ->init()\n";
$i++;
print $db->init(2, "name", "C", 8) ? "ok $i\n" : "not ok $i  OBJ->init()\n";
$i++;
print $db->init(3, "Date", "D") ? "ok $i\n" : "not ok $i  OBJ->init()\n";
$i++;
print $db->init(4, "FLOAT", "F", 10, 3) ? "ok $i\n" : "not ok $i\n";
$i++;
print $db->init(5, "BOOL", "L", 10) ? "ok $i\n" : "not ok $i OBJ->init()\n";
$i++;
print $db->init(6, "LONG", "N", 12) ? "ok $i\n" : "not ok $i OBJ->init()\n";
print $db->init(7, "MEMO", "M") ? "ok $i\n" : "not ok $i OBJ->init()\n";
$i++;
print $db->append(5) ? "ok $i\n" : "not ok $i OBJ->append()\n";
$i++;
print $db->change(1, 1, "01") ? "ok $i\n" : "not ok $i OBJ->change()\n";
$i++;
print $db->change(1, 2, "Peter") ? "ok $i\n" : "not ok $i\n";
$i++;
print $db->change(1, 3, "02-08-1996") ? "ok $i\n" : "not ok $i\n";
$i++;
print $db->change(1, 4, "0") ? "ok $i\n" : "not ok $i\n";
$i++;
print $db->change(2, 7, "memo_entry") ? "ok $i\n" : "not ok $i\n";
$i++;

print $db->update(2, "02 ", "Mike", "27.03.1996", "1.2", "T", "12345678") ? "ok $i\n" : "not ok $i\n";
$i++;
print $db->update(3, "03", "Hans", " 27-03-1996 ", "3.7", "F", "-3") ? "ok $i\n" : "not ok $i\n";
$i++;
print $db->update(4, "04", "Hubert", "27.03.1996", "-.3", "T") ? "ok $i\n" : "not ok $i\n";
$i++;

print $db->goto(5) ? "ok $i\n" : "not ok $i OBJ->goto()\n";
$i++;
print $db->update(5, " 05 ", "Franz", "08/07/1996", "-.3", "T", "-44", "") ? "ok $i\n" : "not ok $i\n";
$i++;
print $db->close() ? "ok $i\n" : "not ok $i OBJ->close()\n";
$i++;

print $db->open("rw") ? "ok $i\n" : "not ok $i\n";
$i++;
print !$db->eof() ? "ok $i\n" : "not ok $i\n";
$i++;
$rc = $db->fld_name(1);
print $rc ? "ok $i\n" : "not ok $i OBJ->fld_name()\n";
$i++;
@rc = $db->fld_names();
print $rc ? "ok $i\n" : "not ok $i OBJ->fld_names()\n";
$i++;
print $db->del(2) ? "ok $i\n" : "not ok $i\n";
$i++;
print $db->is_del(2) ? "ok $i\n" : "not ok $i\n";
$i++;
print $db->undel(2) ? "ok $i\n" : "not ok $i\n";
$i++;
$db->close();

