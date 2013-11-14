#!/usr/bin/perl
use strict;
use warnings;

my ($filename, $substr) = @ARGV;

open FILE, $filename or die "Couldn't open file: $!";
my $string = <FILE>;
chomp($string);
close FILE;

my $offset = 0;

my $result = index($string, $substr, $offset);

while ($result != -1) {
  print "$result\n";

  $offset = $result + 1;
  $result = index($string, $substr, $offset);
}
