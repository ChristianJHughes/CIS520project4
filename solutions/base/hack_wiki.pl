#!/usr/bin/perl
use 5.6.0;
use Time::HiRes;
use POSIX ":sys_wait_h";

# This script loads 1,000,000 lines of max length 2000 characters (avg 1708.5/line)
# It counts the number of lines containing each of 50,000 words from an
# American hacker dictionary, with 10,000 each of length 4, 5, 6, 7, and 8 characters.
# It essentually does a grep and does not look for isolated words.
# The script spawns 24 threads on our Haswell nodes

# Cache optimization does not help since the entire 1.6 GB data file could
# be pulled in from main memory in ~ 0.1 seconds.

# Further optimizations could pre-process the data file to block on 
# non-alphabetic characters.  
# Doing group word searches by searching for 3 common starting letters
# then trying to match individual words should also speed the algorithm up.

$nprocs = 24;
$wordfile = "/scratch/dan/length.4-8.50k";
#$wordfile = "length.4-8.1k";
$wikifile = "/scratch/dan//wiki.1Mshort";
$wikiout = "wiki.out";

open( WIKI, "< $wikifile" )
   or die "Could not open $wikifile\n";

$tstart = Time::HiRes::gettimeofday();

$nlines = 0;
foreach $item ( <WIKI> ) {
   $nlines++;
   chomp( $item );
   push( @lines, $item );
}
close( WIKIFILE );

$tread = Time::HiRes::gettimeofday() - $tstart;
printf( "%d lines were read in %0.3lf seconds\n", $nlines, $tread);


   # Read in all 50k dictionary words

$tstart = Time::HiRes::gettimeofday();

open( WORDLIST, "<$wordfile" )
   or die "Could not open $wordfile\n";

$nwords = 0;
@count = ();
foreach $item ( <WORDLIST> ) {
   $nwords++;
   chomp( $item );
   push( @word, $item );
   push( @count, 0 );
}
close WORDLIST;

$tread = Time::HiRes::gettimeofday() - $tstart;
printf( "%d words were read in %0.3lf seconds\n", $nwords, $tread);


   # Spawn a thread on each core and process all words

$tstart = Time::HiRes::gettimeofday();

for( $myproc = 0; $myproc < $nprocs; $myproc++ ) {

      # Fork a child process to launch the test

   defined( $pid[$myproc] = fork ) or die "Cannot fork for proc $myproc\n";
   unless( $pid[$myproc] ) {

      printf "spawned proc $myproc\n";

         # Pull in 1 line at a time and search through all the words

      #foreach $line ( @lines ) {              # For each line test all 10K words

         #for( $i = $myproc; $i < $nwords; $i += $nprocs ) {
            #if( $line =~ /$word[$i]/ ) {
               #$count[$i]++;
               ##print "inside if $myproc $i $word[$i] $count[$i]\n";
            #}
         #}
      #}

            # For each word, search through all the lines

      for( $i = $myproc; $i < $nwords; $i += $nprocs ) {

         foreach $line ( @lines ) {
            if( $line =~ /$word[$i]/ ) { $count[$i]++; }
         }
         ##$count[$i] = `grep -c $word[$i] $wikifile`;
         ##print "$myproc $i $word[$i] $count[$i]\n";
      }

         # Dump counts to the output file - must do inside the fork

      printf "$myproc writing output to $wikiout.$myproc\n";
      open( WIKIOUT, "> $wikiout.$myproc" )
         or die "Could not open $wikiout.$myproc\n";

      for( $i = $myproc; $i < $nwords; $i += $nprocs ) {
         print WIKIOUT "$i $word[$i] $count[$i]\n";
      }
      close WIKIOUT;
      printf "$myproc completed so exiting the thread\n";
      exit(0);
   }
}
      # Competion check

printf "Main proc waiting on for threads to complete\n";
for( $i = 0; $i < $nprocs; $i++ ) {
   while( waitpid( $pid[$i], NOHANG) == 0 ) {
      sleep 1;
   }
   printf "Thread $i has completed\n";
}
printf "All threads have completed\n";

   # Print the total time

$ttotal = Time::HiRes::gettimeofday() - $tstart;
printf ("Total time for %d procs was %0.3lf seconds\n", $nprocs, $ttotal);


