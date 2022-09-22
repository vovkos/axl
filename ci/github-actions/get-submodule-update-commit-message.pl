use strict;

# get summary for submodule modifications

my $summary=`git submodule summary`;

# iterate and add all modified submodules to the brief commit message

my $prefix = "submodule update:";
my $delim = " ";
my $submoduleCount = 0;
while ($summary =~ /(^\*|\n\*)\s+([^\s]+)\s/g) {
    $prefix = "$prefix$delim$2";
    $delim = ", ";
    $submoduleCount++;
}

# return an empty string if no submodules were modified

if ($submoduleCount == 0) {
	exit(0);
}

# concat and escape quotation marks

my $message = "$prefix\n\n$summary";
$message =~ s/\n\n$/\n/;
$message =~ s/"/\\"/g;

# output the result

print($message);
