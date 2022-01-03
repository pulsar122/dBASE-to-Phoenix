use ExtUtils::MakeMaker;
# See lib/ExtUtils/MakeMaker.pm for details of how to influence
# the contents of the Makefile that is written.
WriteMakefile(
    'NAME'	=> 'Dbf',
    'VERSION_FROM' => 'Dbf.pm', # finds $VERSION
    'LIBS'	=> ['-L../lib -ldbf'],   # e.g., '-lm' 
    'DEFINE'	=> '',     # e.g., '-DHAVE_SOMETHING' 
    'INC'	=> '-I../lib',     # e.g., '-I/usr/include/other' 
);
