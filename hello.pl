#!/usr/bin/perl
# this script is called when asterisk gets incoming call 
# you are "printing to stdouput the commands to asterisk to manage user
#ex: play file 
#get dtmf
#...
use strict;
use POSIX;

$|=1;
# Set up some variables
my %AGI; my $tests = 0; my $fail = 0; my $pass = 0;
while(<STDIN>) {
        chomp;
        last unless length($_);
        if (/^agi_(\w+)\:\s+(.*)$/) {
                $AGI{$1} = $2;
        }
}
print STDERR "AGI Environment Dump:\n";
foreach my $i (sort keys %AGI) {
        print STDERR " -- $i = $AGI{$i}\n";
}
sub checkresult {
        my ($res) = @_;
        my $retval;
        $tests++;
        chomp $res;
        if ($res =~ /^200/) {
                $res =~ /result=(-?\d+)/;
                if (!length($1)) {
                        print STDERR "FAIL ($res)\n";
                        $fail++;
                } else {
                        print STDERR "PASS ($1)\n";
                        $pass++;
                }
        } else {
                print STDERR "FAIL (unexpected result '$res')\n";
                $fail++;
        }
}

					print STDERR "1.  Testing 'streaming  sound file and tones'...";
print "STREAM FILE beep \"\"\n";
#tt-weasels.gsm is in sftp://root@debian-asterisk/usr/share/asterisk/sounds/en_US_f_Allison/
print "STREAM FILE tt-weasels \"\"\n";
# the sftp://root@debian-asterisk/usr/share/asterisk/sounds/outfile.gsmhas poor quality  is created with "sox infile.wav outfile.gsm"
#print "STREAM FILE outfile \"\"\n";
my $result = <STDIN>;
&checkresult($result);
#does not work 
#file does not exist print "STREAM FILE 8 \"\"\n";
#file not found print "STREAM FILE testagi \"\"\n";
 
#my $result = <STDIN>;
#&checkresult($result);

					print STDERR "2.  Testing 'sendtext'...";
print "SEND TEXT \"hello world\"\n";
my $result = <STDIN>;
&checkresult($result);

					print STDERR "3.  Testing 'sendimage'...";
print "SEND IMAGE asterisk-image\n";
my $result = <STDIN>;
&checkresult($result);
					print STDERR "4.  Testing 'saynumber'...";
print "SAY NUMBER 192837465 \"\"\n";
my $result = <STDIN>;
&checkresult($result);
print "STREAM FILE beep \"\"\n";
my $result = <STDIN>;
&checkresult($result);
					print STDERR "5.  Read dtmf on incoming line 'waitdtmf  10s'...\n";
# This application returns '200 result=49' the result is decimal ASCII value of the digit that was pressed.
# ascii 1=>49
print "WAIT FOR DIGIT  10000 \n";

my $result = <STDIN>;
&checkresult($result);
#;my $res =($result=~/result=\(-?\d+)/g);
my $res=$result =~ /=(\d+)/;

if(($result=~m/result=(\d+)/)){
#correction for ascii
$res=$1 -48;
}


my $user = `whoami`;
my $now = strftime("%Y.%m.%d.%R", localtime(time));
my $file = "/tmp/hello.pl_AGI_script_debug_output_${user}_${now}.txt";

open FILE, ">$file" or die "unable to open $file $!";

print FILE "this line is fake to print a first line  to your $file output file if you don't see entire output this script may have crashed in asterisk check your sintax \n";
print FILE "complete reply from asterisk: $result\n";
print FILE " extracted  result  res is :  $res\n";
					print STDERR "5b.  got escape digit'...\n";
					print FILE "5b.  got escape digit'...\n";
print "STREAM FILE beep \"\"\n";
my $result = <STDIN>;
&checkresult($result);
					print STDERR "6.  Testing 'record until digit 1 or 10s'...";
					 print FILE  "6.  Testing 'record until digit 1 or 10s'...";
print "RECORD FILE testaginewlocation gsm 1 10000";
my $result = <STDIN>;
&checkresult($result);
print "STREAM FILE beep \"\"\n";
my $result = <STDIN>;
&checkresult($result);
					print STDERR "6a.  Testing 'record playback'...";
					print FILE  "6a.  Testing 'record playback'...";
print "STREAM FILE testaginewlocation \"\"\n";
my $result = <STDIN>;
&checkresult($result);
					print STDERR "7a.  Testing 'hangup'...";
					print FILE  "7a.  Testing 'hangup'...";
print "HANGUP \"\"\n";
my $result = <STDIN>;
&checkresult($result);
print STDERR "================== Complete ======================\n";
print STDERR "$tests tests completed, $pass passed, $fail failed\n";
print STDERR "==================================================\n";
close (FILE); 
exit(0);
