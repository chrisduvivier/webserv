#!/usr/bin/perl -wT
# Print a formatted list of all the environment variables

use strict;
print "Content-type: text/html\n\n";

my $var_name;
foreach $var_name ( sort keys %ENV ) {
    print "
    $var_name";
    print $ENV{$var_name};
}
