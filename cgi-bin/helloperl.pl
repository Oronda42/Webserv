#!/usr/bin/perl
  
# Modules used
use strict;
use warnings;
  

my @content = (
	"<html>",
	"<head>",
	"<title>Hello - Second CGI Program</title>",
	"</head>",
	"<body>",
	"<h2>Hello Pouet pouet pouet</h2>",
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


print("HTTP/1.1 200 OK");
print("Content-Length: $contentLength");
print("Content-type: text/html\r\n\r\n");

foreach ( @content )
{
	print($_);
}