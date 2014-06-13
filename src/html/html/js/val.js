
function valDoValidateUserName(Name,Result)
{
   if(Name.match("^[0-9]") != null) { return("A name can not start with a number."); }
   if(Name.match("[^0-9a-zA-Z_-]") != null) { return("A name can only have alpha-numeric characters"); }
   else if(Name.length > 64) { return("A name must contain 0..64 characters"); }
   return null;
}

function valDoValidateName(Name,Result)
{
   if(Name.match("^[0-9]") != null) { return("A name can not start with a number."); }
   if(Name.match("[^0-9a-zA-Z_-]") != null) { return("A name can only have alpha-numeric characters"); }
   else if((Name.length == 0) || (Name.length > 32)) { return("A name must contain 1..32 characters"); }
   return null;
}

function valDoValidateInteger(Integer,Result)
{   
   if(Integer.match("^[0-9]+\$")) { return null;}
   return("An integer can only have digits");
}

function valDoValidateIp(Address)
{
  var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits;
  var i;
  var error=null;

  if(address == null) { error="Invalid IP address"; }
  else
  {
    digits = address[0].split(".");
    for(i=0; i < 4; i++)
    {
      if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ) || (Number(digits[0]) > 223))
      { error="Invalid IP address"; break; }
    }
  }
  return error;
}

function valDoValidateNet(Address)
{
  var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits;
  var error=null;

  if(address == null) { error="Invalid network ID"; return error; }
  if(address[0] == "255.255.255.255") { error="Invalid network ID"; return error; }
  digits = address[0].split(".");
  for(i=1; i <= 4; i++)
   if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ))
    { error="Invalid network ID"; break; }
   return error;
}

function valDoValidateMask(Mask)
{
  var mask = Mask.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits;
  var bMask = 0;
  var watch = false;
  var i;
  var error=null;

  if(mask == null) { error="Invalid network mask"; return error; }
  digits = mask[0].split(".");
  for(i=0; i < 4; i++)
  {
    if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ))
     { error="Invalid network mask"; return error; }
	bMask = (bMask << 8) | Number(digits[i]);
  }
  bMask = bMask & 0x0FFFFFFFF;
  for(i=0; i<32; i++)
  {
    if((watch==true) && ((bMask & 0x1)==0)) { error="Invalid network mask"; return error; }
	bMask = bMask >> 1;
	if((bMask & 0x01) == 1) watch=true;
  }
  return error;
}

function valDoValidateHostName(HostName)
{
   if (HostName.match("^[a-zA-Z]+") == null) return ("Host Name must start with a character");
   else if (HostName.match("^([a-zA-Z]+[a-zA-Z0-9_-]*)(\.*[a-zA-Z0-9])+$") == null) return ("Host Name does not have correct format");
   else if ((HostName.length == 0) || (HostName.length > 63)) { return("A host name must contain 1..63 characters"); }
   return null;
}

function valDoValidateGateway(Address)
{
  var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits;
  var i;
  var error;

  if(address == null) { error="Invalid Gateway IP address"; return error; }
  digits = address[0].split(".");
  for(i=0; i < 4; i++)
  {
    if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ) || (Number(digits[0]) > 223))
    { error="Invalid Gateway IP address"; return error; }
  }
   return true;
}

function valDoValidateMAC(Addr)
{
   if(Name.match("[^0-9a-fA-F_-]") != null) { return("A mac address can only have characters 0 to 9 and A to F octet separated by -"); }
   return null;
}


