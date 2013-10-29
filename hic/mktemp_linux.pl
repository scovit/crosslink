
my @deletion_list;
sub mktemp_linux
{
    my $fname=`mktemp $_[0]`;
    $fname =~ s/^\s+|\s+$//g ;
    push(@deletion_list, $fname);
    return $fname;
}

END {
    print "Cleaning up!\n";
    foreach (@deletion_list) {
	unlink($_) or warn "Could not unlink $origreads: $!";
    }
}

1;
