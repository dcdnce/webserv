#!/usr/bin/perl


# Modules used
use strict;
use warnings;
use CGI;

my $cgi = CGI->new;

my $fname = $cgi->param('fname');
my $lname = $cgi->param('lname');

# Check if fname and lname are intiailize
if (not defined $fname) {
	$fname = "(undefined)";
}

if (not defined $lname) {
	$lname = "(undefined)";
}

my @content = (
	"<html>",
	"<head>",
	"<title>Hello - Second CGI Program</title>",
	"</head>",
	"<body>",
	"<h2>Hello $fname $lname</h2>",
	"</body>",
	"</html>"
);

my $contentLength = 0;
foreach ( @content )
{
	$contentLength += length($_)
}

# https://stackoverflow.com/questions/2899367/how-can-perls-print-add-a-newline-by-default
$\ = "\n";


print("Content-Length: $contentLength");
print("Content-type: text/html\r\n\r\n");

foreach ( @content )
{
	print($_);
}
