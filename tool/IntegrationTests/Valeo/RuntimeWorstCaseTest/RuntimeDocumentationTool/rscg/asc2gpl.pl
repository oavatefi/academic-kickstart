#! /usr/bin/perl
use Getopt::Long;
use strict;

sub print_help_and_exit()
{
    print "Usage: perl asc2gpl.pl [OPTION]... <file.asc> \n\n\t--can-dbc=DBC_FILE_AS_STRING\n".
                                "\t--can-signal-path=SIGNAL_PATH_AS_STRING\n".
                                "\t--help\n";
    exit();
}

sub max {
    return (@_[0] < @_[1])? @_[1]: @_[0];
}

sub min {
    return (@_[0] > @_[1])? @_[1]: @_[0];
}

sub get_signal_value {
    my $values = shift;
    my $signal = shift;
    my $start = $signal->{'start'};
    my $length = $signal->{'length'};
    my $intel = $signal->{'format'} == 1;
    my $value;
    my $shift_left_count;

    while ($length > 0) {
        my $sbi = int($start / 8);
        if ($sbi >= 0 and $sbi < 8)
        {
            my $handled_bit_count = (($sbi + 1) * 8) - $start;
            my $shift_right_count = 8 - $handled_bit_count;
            $value = $value | ((($values->[$sbi] >> $shift_right_count) & ((2 ** min(8, $length)) - 1)) << $shift_left_count);
            $length -= $handled_bit_count;
            if ($intel) {
                $start += $handled_bit_count;
            } else {
                $start -= $handled_bit_count;
            }
            $shift_left_count += 8;
        } else {
            print STDERR "Warning: byte index out of range!\n";
        }
    }

    return ($value * $signal->{'factor'}) + $signal->{'offset'};
}

sub get_signal_from_dbc {
    my $dbc = shift;
    my @msg_signal = split '\.', shift;
    my $parser_state;
    my $signal;
    my $mux;
    my $mux_signal;
    my $hex_msg;
    my $mux_required;
    my $signal_found;
    my $mux_found;
    open DBC, "<". "$dbc" or die("Can not open dbc file $dbc");
    while (<DBC>) {
        if ($parser_state == 0) {
            if (/BO_\s+(\d+)\s+$msg_signal[0]:/) {
                $parser_state = 1;
                $hex_msg = sprintf("%Xx", ($1 - 2 ** 31)) if ($1 > 2 ** 31);
                $hex_msg = sprintf("%X", $1) if ($1 <= 2 ** 31);
            }
        } else {
            if ($parser_state == 1) {
                if (/\s+SG_\s+$msg_signal[1]\s+((?:m\d+)?)\s*:\s+(\d+)\|(\d+)@([01])([+-])\s+\((\d+),(\d+)\)/) {
                    $signal->{'msg'} = $hex_msg;
                    $signal->{'start'} = $2;
                    $signal->{'length'} = $3;
                    $signal->{'format'} = $4;
                    print STDERR "Warning: Byte format '$4' not supported by this parser!\n" if (($4 ne 1) and ($4 ne 0));
                    $signal->{'signed'} = 1 if ($5 eq '-');
                    print STDERR "Warning: Signal type '$5' not supported by this parser!\n" if (($5 ne '+') and ($5 ne '-'));
                    $signal->{'factor'} = $6;
                    $signal->{'offset'} = $7;
                    if ($1 ne '') {
                        $signal->{'mvalue'} = $1;
                        $signal->{'mvalue'} =~ s/m//g;
                    }
                    $signal_found = 1;
                } else  {
                    if (/\s+SG_\s+[^\s]+\s+M\s+:\s+(\d+)\|(\d+)@([01])([+-])\s+\((\d+),(\d+)\)/) {
                        $mux->{'msg'} = $hex_msg;
                        $mux->{'start'} = $1;
                        $mux->{'length'} = $2;
                        $mux->{'format'} = $3;
                        print STDERR "Warning: Byte format '$4' not supported by this parser!\n" if (($3 ne 1) and ($3 ne 0));
                        $mux->{'signed'} = 1 if ($4 eq '-');
                        print STDERR "Warning: Signal type '$4' not supported by this parser!\n" if (($4 ne '+') and ($4 ne '-'));
                        $mux->{'factor'} = $5;
                        $mux->{'offset'} = $6;
                        $mux_found = 1;
                    }
                }
                last if (/BO_.*/);
            }
        }
    }

    $signal->{'mux'} = $mux if defined $signal->{'mvalue'} and $mux_found;

    die("Signal path '$msg_signal[0].$msg_signal[1]' can not be found in dbc '$dbc'") if (!$signal_found);
    return $signal;
}

sub main() {
    my $can_dbc;
    my $can_signal_path;
    my $signal_found;
    my $help;
    GetOptions ("can-dbc=s" => \$can_dbc,
          "can-signal-path=s"   => \$can_signal_path,
          "help"   => \$help) or print_help_and_exit();

    print_help_and_exit() if ($help or $can_dbc eq "" or $can_signal_path eq "");

    my $signal = get_signal_from_dbc $can_dbc, $can_signal_path;
    
    my @message_name = split('\.', $can_signal_path);
    while (<>) {
        my @msg_values;
        if ($_ =~ m/(\d+\.\d+).*?$signal->{'msg'}\s+?[TR]x\s+?d\s8\s(.*?)\sLength.*/ or $_ =~ m/(\d+\.\d+).*?$message_name[0]\s+?Rx\s+?d\s8\s(.*?)\sLength.*/) {
            my $mux_fits = 1;
            @msg_values = map hex, split " ", $2;
            if (defined($signal->{'mux'})) {
                my $mux_value = get_signal_value \@msg_values, $signal->{'mux'};
                $mux_fits = $mux_value eq $signal->{'mvalue'};
            }
            if ($mux_fits) {
                my $value = get_signal_value(\@msg_values, $signal);
                print $1 . " " . $value . "\n";
            }
            $signal_found = 1;
        }
    }

    die("Signal path '$can_signal_path' can not be found in input trace file!") if (!$signal_found);
}

main();
