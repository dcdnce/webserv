#!/usr/bin/perl 

use strict;
use warnings;
use CGI;

my $cgi = CGI->new();
my %params = $cgi->Vars;

print "content-type: text/html\n\n"; 
print "<html><head><title>Test</title></head>\n"; 
print "<body>\n"; 

foreach my $param (keys %params){
	my $value = $params{$param};
	print "<p>$param = $value</p>";
}

print "</body></html>\n";