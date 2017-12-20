use strict;
use Test::More tests => 12;


# checking the algorithm to show hour progress
is(seconds_to_bits(0), 0, '0 -> 0');
is(seconds_to_bits(399), 0, '399 -> 0');
is(seconds_to_bits(400), 1, '400 -> 1');
is(seconds_to_bits(799), 1, '799 -> 1');
is(seconds_to_bits(800), 3, '800 -> 3');
is(seconds_to_bits(1200), 7, '1200 -> 7');
is(seconds_to_bits(1600), 15, '1600 -> 15');
is(seconds_to_bits(2000), 31, '2000 -> 31');
is(seconds_to_bits(2400), 63, '2400 -> 63');
is(seconds_to_bits(2800), 127, '2800 -> 127');
is(seconds_to_bits(3200), 255, '3200 -> 255');
is(seconds_to_bits(3599), 255, '3599 -> 255');

sub seconds_to_bits {
    my $seconds = shift;

    my $hour_part = (($seconds % 3600) / 400);
    (1 << $hour_part) -1;
}
