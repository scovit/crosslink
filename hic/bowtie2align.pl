

sub bowtie2align
{
    print "aligning $_[0] to $_[1]\n";
#    my $settings="--score-min L,0.6,0.2 --very-sensitive -p 16 -q";
    my $settings="--very-sensitive -p 14 --reorder -q";
    my $ref=$_[1];
    my $comando="bowtie2 $settings -x $ref -U $_[0]";

    local *FH;
    print $comando, "\n";
    open (FH, $comando . " | cut -f 1,2,4,5 |") or return undef;
    return *FH;
}

1;
