#!/usr/bin/perl
use strict;
use warnings;

# This the mapping pipeline (Mirny)
#

if ($#ARGV != 3) {
	print "usage: ./map.pl leftmap rightmap rsttable classification\n";
	exit;
};

my ($leftmapfn, $rightmapfn, $rsttablefn, $classificationfn) = @ARGV;

my $genomel = 4639675;
my $rstdistmin = 2;
my $basedistmin = 30;

open RSTTABLE, "<", $rsttablefn or die $!;
my @start; my @stop;
while (<RSTTABLE>) {
    chomp;
    my ($num, $st, $en) = split("\t", $_);
    push @start, $st;
    push @stop, $en;
}
close RSTTABLE;

sub findinrst {
    my $ele = $_[0];

    my $top = $#start; my $bottom = 0;
    while (1) {
        my $index = int(($top + $bottom)/2);

        if ($ele >= $start[$index] && $ele < $stop[$index]) {
            return $index;
            last;
        } elsif ($ele < $start[$index]) {
            $top = $index - 1;
        } elsif ($ele >= $stop[$index]) {
            $bottom = $index + 1;
        }
    }
}



open(LEFTMAP, "zcat $leftmapfn |");
open(RIGHTMAP, "zcat $rightmapfn |");
open(CLASSIFIC, "| gzip > $classificationfn");

print "Starting classification\n";

use constant {
    FL_LEFT_NOTFOUND => 2,
    FL_RIGHT_NOTFOUND => 1,
    FL_LEFT_INVERSE => 8,
    FL_RIGHT_INVERSE => 4,
    FL_INVERSE => 16,
};


my $flag = 0;
my @left = ();
my @right = ();
for (my $num = 0; ; $num++) {

    if (((~$flag) & FL_LEFT_NOTFOUND ) && !eof(LEFTMAP)) {
	my $ls = <LEFTMAP>;
	chomp($ls); @left = split("\t", $ls);
    }
    if (((~$flag) & FL_RIGHT_NOTFOUND ) && !eof(RIGHTMAP)){
	my $rs = <RIGHTMAP>;
	chomp($rs); @right = split("\t", $rs);
    }


    my $leftpos = -1; my $rightpos = -1;
    my $leftrst = -1; my $rightrst = -1;

    $flag = 0;
    if ($left[0] != $num) {
	$flag |= FL_LEFT_NOTFOUND;
    } else { 
	$leftpos = $left[3];
	$leftrst = findinrst($leftpos);

	my $leftpos2 = $leftpos;
	if ($left[2] & 16) {
	    $flag |= FL_LEFT_INVERSE;
	}
    }
    if ($right[0] != $num) {
	$flag |= FL_RIGHT_NOTFOUND;
    } else {
	$rightpos = $right[3];
	$rightrst = findinrst($rightpos);

	if ($right[2] & 16) {
	    $flag |= FL_RIGHT_INVERSE;
	}
    }

    my $distance = 0;
    my $rstdist = 0;
    if ($flag & (FL_LEFT_NOTFOUND | FL_RIGHT_NOTFOUND)) {
	$distance = -1;
	$rstdist = -1;
    } else {
	if ($leftpos >= $rightpos) {
	    $distance = $leftpos - $rightpos;
	    $rstdist = $leftrst - $rightrst;
	    if ($distance > $genomel / 2) {
		$distance = $genomel - $distance;
                $rstdist = $#start - $rstdist;
	    } else {
		$flag |= FL_INVERSE;
	    }
	} else {
	    $distance = $rightpos - $leftpos;
	    $rstdist = $rightrst - $leftrst;
            if ($distance > $genomel / 2) {
                $distance = $genomel - $distance;
		$rstdist = $#start - $rstdist;
		$flag |= FL_INVERSE;
	    }
	}
    }

    printf CLASSIFIC "%d\t%.5b\t%d\t%d\t%d\t%d\t%d\t%d\n", $num, $flag, $leftpos, $rightpos, $distance, $leftrst, $rightrst, $rstdist;

    last if (eof(LEFTMAP) && eof(RIGHTMAP));
}

close CLASSIFIC;
close LEFTMAP;
close RIGHTMAP;

exit;
