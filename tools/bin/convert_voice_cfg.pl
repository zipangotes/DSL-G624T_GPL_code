#!/usr/bin/perl
#####################################################################################
##
## file  : convert_voice_cfg.pl
## Usage : convert_voice_cfg.pl  <config.bin> <targetXMLFile>
##         <config.bin> - binary configuration saved using WUI 
##         <targetXMLFile>  Target XML file to be modified using the voice configuration
##         in config.bin 
##
##
## location: tools/bin
## Author: Murali Karicheri
## Date  : 8/3/2004
##
#####################################################################################
$numArgs = $#ARGV + 1;

if ($numArgs != 2) 
{
     print "Usage: convert_voice_cfg.pl <config.bin> <TargetXMLFile>\n";
     print "Convert the voice configuration in the target XML file to the one in config.bin \n";
     print "Procedure: create the config.bin using the WUI\n";
     print "           run the script with arguments as given below\n";
     print "           >./convert_voice_cfg.pl config.bin  config.xml\n";
     print "           The converted file is overwritten to config.xml and original is\n"; 
     print "           copied to config.xml.old\n";
     print "           need following scripts: mkconfig & xmlindent\n";
     exit;
}

$tempOutFile = $ARGV[1].".old";


$tempInFile = "./tmp_config.xml";

$binInFile = $ARGV[0];

print "Converting $ARGV[1]\n";


if (! -e $ARGV[0])
{
     print "$ARGV[0] doesn't exist\n";
     exit;
}

if (! -e $ARGV[1])
{
     print "$ARGV[1] doesn't exist\n";
     exit;
}

#use mkconfig to convert the file to xml format
system ("./mkconfig", "-d", "-i", $binInFile, "-o", $tempInFile);

# change the permission of the target file
system ("chmod", "644",$ARGV[1]);
if (! -w $ARGV[1])
{
     print "$ARGV[1] is not writeable\n";
     exit;
}

open (REF_FILE, "$tempInFile") || die "Unable to open file $tempInFile\n";
open (OUT_FILE, "$ARGV[1]") || die "Unable to open file $ARGV[1] for reading\n";
open (BACKUP_FILE, ">$tempOutFile") || die "Unable to open $tempOutFile\n";

while (<OUT_FILE>)
{
     print BACKUP_FILE $_;
}

close (OUT_FILE);
close (BACKUP_FILE);


open (OUT_FILE, ">$ARGV[1]") || die "Unable to open file $ARGV[1] for writing\n";
open (BACKUP_FILE, "<$tempOutFile") || die "Unable to open file $tempOutFile\n";

@bufIn  = <REF_FILE>;
@bufOut = <BACKUP_FILE>;

$fullTextIn = join('',@bufIn);
$fullTextOut = join('',@bufOut);

$_=  $fullTextIn;

if (!((/<voice>/gi) && (/<\/voice>/gi)))

{
     print "Voice blob not found in $tempInFile\n";
     close(BACKUP_FILE);
     close(OUT_FILE);
     close (REF_FILE);
     system ("mv",$tempOutFile, $ARGV[1]);
     system ("rm","-f",$tempInFile);
     exit;
}

$_ = $fullTextOut;

if (!((/<voice>/gi) && (/<\/voice>/gi)))

{
     print "Voice blob not found in $ARGV[1]\n";
     close(BACKUP_FILE);
     close(OUT_FILE);
     close (REF_FILE);
     system ("mv",$tempOutFile, $ARGV[1]);
     system ("rm","-f",$tempInFile);
     exit;
} 

$_=  $fullTextIn;

#extract the voice blob from reference file
/<voice>(.*?)<\/voice>/gis;

if ($1 eq "")
{
     print "expty voice blob in $tempInFile";
     close(BACKUP_FILE);
     close(OUT_FILE);
     close (REF_FILE);
     system ("mv",$tempOutFile, $ARGV[1]);
     system ("rm","-f",$tempInFile);
     exit;
}

$blob_ref = $1;

$_ = $fullTextOut;

$res = s/(<voice>)(.*?)(<\/voice>)/\1$blob_ref\3/gis;
if (!$res)
{
     print "error while replacing the voice blob";
     close(BACKUP_FILE);
     close(OUT_FILE);
     close (REF_FILE);
     system ("mv",$tempOutFile, $tempInFile);
     system ("rm","-f",$tempInFile);
     exit;
}

#Save the output file 
print OUT_FILE $_;

print "Formatting the XML file $ARGV[1]\n";
#format the XML file
if (! -e "./xmlindent") 
{
     print "Unable to find the xmlindent for indentation\n";
}
system("./xmlindent", "-w", "-nas", "-nbs", "-nbe", "-f", "-i4", $ARGV[1]);

$backup = $ARGV[1]."~";
system("rm", "-f", $backup);

print "Conversion complete\n";

close (OUT_FILE);
close (REF_FILE);
close (BACKUP_FILE);
system ("rm","-f",$tempInFile);

