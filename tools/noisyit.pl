#!/usr/bin/perl

if ($#ARGV != 0) {
    die "Usage ./noisyit.pl N\n";
}

my ($N) = @ARGV;

print "Hello world. $N\n";

use push instead of print

my @sorted_strings = sort @strings;
print "@sorted_strings\n";
