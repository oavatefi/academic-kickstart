use strict;
use Data::Dumper;
my %summary;
while (<>) {
    my @entries = split /\t/;
    $entries[5] = "default" if ($entries[5] eq '');
    $summary{$entries[6]}{$entries[0]} += $entries[3];
    $summary{$entries[6]}{"module_name"} = $entries[5];
}

foreach (sort keys %summary) {
    print $_ . "| " . $summary{$_}{".text"} . " | 0 | " . ($summary{$_}{".bss"} + $summary{$_}{".data"}) . "| 0 | 0 | 0 | 0 | 0 | 0 | 0 |\n";
}