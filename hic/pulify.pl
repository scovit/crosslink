#!/usr/bin/perl

use strict;
use warnings;

# This the mapping pipeline (Mirny)

if ($#ARGV != 2) {
	print "usage: ./pulify.pl classification rsttable cleaned\n";
	exit;
};

my ($classificationfn, $rsttablefn, $cleanedfn) = @ARGV;

my $genomel = 4639675;
my $rstdistmin = 2;
my $basedistmin = 30;

open RSTTABLE, "<", $rsttablefn or die $!;

my @start;
my @stop;

while (<RSTTABLE>) {
    chomp;
    my ($num, $st, $en) = split("\t", $_);
    push @start, $st;
    push @stop, $en;
}

close RSTTABLE;

open(CLASSIFIC, "zcat $classificationfn |") or die $!;
open(FCLEANED, "| gzip > $cleanedfn") or die $!;

print "Starting pulification\n";

use constant {
    FL_LEFT_NOTFOUND => 2,
    FL_RIGHT_NOTFOUND => 1,
    FL_LEFT_INVERSE => 8,
    FL_RIGHT_INVERSE => 4,
    FL_INVERSE => 16,
};

sub findinrst {
    my $top = $#start; my $bottom = 0;
    while (1) {
        my $index = int(($top + $bottom)/2);

        if ($_[0] >= $start[$index] && $_[0] < $stop[$index]) {
            return $index;
	    last;
        } elsif ($_[0] < $start[$index]) {
            $top = $index;
        } elsif ($_[0] >= $stop[$index]) {
            $bottom = $index;
        }
    }
}

while (<CLASSIFIC>) {
    chomp;

    my ($num, $flagstr, $left, $right, $dist) = split("\t", $_);
    my $flag = oct("0b".$flagstr);

    if ($flag & (FL_LEFT_NOTFOUND | FL_RIGHT_NOTFOUND)) {
	next;
    }

    my $beforeleft = findinrst($left);
    my $beforeright = findinrst($right);

    my $rstdist = abs($beforeright - $beforeleft);

    if (($rstdist >= $rstdistmin) && ($dist >= $basedistmin)) {
	printf "%s\t%s\t%s\t%s\t%d\t%d\n", $num, $flagstr, $left,
               $right, $dist, $rstdist;
    }
}

close FCLEANED;
close CLASSIFIC;

exit;
