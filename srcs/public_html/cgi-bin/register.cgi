#!/usr/bin/perl

read (STDIN, $buffer, 2000);
print($buffer);
# @pairs = split(/&/, $buffer);
# foreach $pair (@pairs) 
# {
#     ($name, $value) = split(/=/, $pair);
#     $value =~ tr/+/ /;
#     $value =~ s/%([a-fA-F0-9] [a-fA-F0-9])/pack("C", hex($1))/eg;
#     $value =~ s/~!/ ~!/g;
#     $FORM{$name} = $value;
# }

# $name= $FORM{'name'};
# $email= $FORM{'email'};

print "hello world";
