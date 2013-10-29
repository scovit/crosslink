#!/usr/bin/perl
use strict;
use warnings;

require 'mktemp_linux.pl';
require 'readtrimmer.pl';
require 'bowtie2align.pl';

# This the mapping pipeline (Mirny)
#

if ($#ARGV != 3) {
	print "usage: ./map.pl fastqfile readlength leftmap rightmap\n";
	exit;
};

my ($fastqfilename, $readlength, $leftmapfn, $rightmapfn) = @ARGV;

my $TMPDIR="/home/scolari/tmp";

my $origreads=mktemp_linux("$TMPDIR/tmp.XXXXXXXX.fastq");
my $leftreads=mktemp_linux("$TMPDIR/tmp.XXXXXXXX.fastq.gz");
my $rightreads=mktemp_linux("$TMPDIR/tmp.XXXXXXXX.fastq.gz");

print "Extracting fastq file\n";
my $N=int(`zcat $fastqfilename | tee $origreads | wc -l`) / 4;

print $N, " reads found\n";

my $stepl = 10;
my $minqual = 30;
my @leftl = ($stepl) x $N;
my @rightl = ($stepl) x $N;

sub appendmap {
    my ($mapfile, $samfile, $leftlength) = @_;

    my $index = 0;
    while (<$samfile>) {
        chomp;
        if ($_ =~ /^@/)
        {
            print $_,"\n";
        }
        else
        {
            my ($NAME, $FLAG, $POS, $MAPQ) = split("\t");
            while (${ $leftlength }[$index] == 0) {
                $index++;
            }

            if ($MAPQ < $minqual) {
# Unmapped
		${ $leftlength }[$index] += $stepl;
                $index++;
                next;
            }
# Mapped
	    print $mapfile $index, "\t", $NAME, "\t", $FLAG, "\t", $POS, "\t", $MAPQ, "\t", ${ $leftlength }[$index], "\n";
            ${ $leftlength }[$index] = 0;
            $index++;
        }
    }
}

open( my $leftmap, "| sort -g --temporary-directory=$TMPDIR | gzip > $leftmapfn");
open( my $rightmap, "| sort -g --temporary-directory=$TMPDIR | gzip > $rightmapfn");

print "Starting trimmering\n";
while (readtrimmer($origreads, $readlength, \@leftl, \@rightl, $leftreads, $rightreads) <= $readlength/2) {
    my $alignedfile = bowtie2align($leftreads);
    appendmap($leftmap, $alignedfile, \@leftl);
    close ($alignedfile);

    $alignedfile = bowtie2align($rightreads);
    appendmap($rightmap, $alignedfile, \@rightl);
    close ($alignedfile);
}

close $leftmap;
close $rightmap;

exit;
