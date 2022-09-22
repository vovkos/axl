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

# abort with an empty string if no submodules were modified

$submoduleCount > 0 || die("\n");

# concat and escape quotation marks

my $message = "$prefix\n\n$summary";
$message =~ s/\n\n$/\n/;
$message =~ s/"/\\"/g;

# output the result

print($message);
