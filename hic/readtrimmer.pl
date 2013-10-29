#!/usr/bin/perl
use strict;
use warnings;

# The function takes two files as input:
#   the first is the fastq.gz file,
#   the second is the two columned file with the read size
# The output are the trimmed reads, still two files
#

sub readtrimmer
{
    if ($#_ != 5 ) {
	print "readtrimmer needs 4 arguments: fastqfile leftlength rightlength outputleft.fastq.gz outputright.fastq.gz indexl indexr\n";
	exit;
    };
    
    print "readtrimmer.pl Warning: This subroutine has been made for equal length pair-ended sequencing\n";
    
    my ($fastqfilename, $readl, $leftlength, $rightlength, $outputleftfilename, $outputrightfilename)=@_;
    
    open OUTPUT1, "| gzip > $outputleftfilename";
    open OUTPUT2, "| gzip > $outputrightfilename";
    
    my $maxtrim = 0;
    my $lefttrim;
    my $righttrim;
    
    open FILE, $fastqfilename;
    while (<FILE>) {
	my $i = ($. - 1) / 4;
	$lefttrim=${ $leftlength }[$i];
	$righttrim=${ $rightlength }[$i];
	$maxtrim = $lefttrim if $lefttrim > $maxtrim;
        $maxtrim = $righttrim if $righttrim > $maxtrim;

	if ($. % 2 == 0) {
	    if ($lefttrim != 0) {
		print OUTPUT1 substr($_,0,$lefttrim), "\n";
	    }
	    if ($righttrim != 0) {
		print OUTPUT2 substr($_, $readl/2, $righttrim), "\n";
# other side: length() - 1 - $righttrim,$righttrim), "\n";
	    }
	} else {
	    if ($lefttrim != 0) {
		print OUTPUT1;
	    }
	    if ($righttrim != 0) {
		print OUTPUT2;
	    }
	}
    }
    close FILE;

    close OUTPUT1;
    close OUTPUT2;

    return $maxtrim;
}

1;

