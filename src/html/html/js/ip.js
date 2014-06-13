
function doValidateIP(Address)
{
  var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits;
  var i;

  if(address == null) { alert('Invalid IP address ' + Address); return false; }
  digits = address[0].split(".");
  for(i=0; i < 4; i++)
  {
    if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ) || (Number(digits[0]) > 223))
    { alert('Invalid IP address '  + address[0]); return false; }
  }
   return true;
}

function doValidateNet(Address)
{
  var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits;

  if(address == null) { alert('Invalid network ' + Address); return false; }
  if(address[0] == "255.255.255.255") { alert('Invalid network ' + address[0]); return false; }
  digits = address[0].split(".");
  for(i=1; i <= 4; i++)
   if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ))
    { alert('Invalid network address ' + address[0]); return false; }
   return true;
}

function doValidateMask(Mask)
{
  var mask = Mask.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits;
  var bMask = 0;
  var watch = false;
  var i;

  if(mask == null) { alert('Invalid network mask ' + Mask); return false; }
  digits = mask[0].split(".");
  for(i=0; i < 4; i++)
  {
    if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ))
     { alert('Invalid network mask ' + mask[0]); return false; }
	bMask = (bMask << 8) | Number(digits[i]);
  }
  bMask = bMask & 0x0FFFFFFFF;
  for(i=0; i<32; i++)
  {
    if((watch==true) && ((bMask & 0x1)==0)) { alert("Invalid Mask " + Mask + " (not contiguous)"); return false; }
	bMask = bMask >> 1;
	if((bMask & 0x01) == 1) watch=true;
  }

   return true;
}

function doValidateHostName(Name)
{
   if(Name.match("[^0-9a-zA-Z_-]") != null) { alert("A name can only have\nalpha-numeric characters"); return false; }
   return true;
}

function doValidateGateway(Address)
{
  var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits;
  var i;

  if(address == null) { alert('Invalid Gateway IP address ' + Address); return false; }
  digits = address[0].split(".");
  for(i=0; i < 4; i++)
  {
    if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ) || (Number(digits[0]) > 223))
    { alert('Invalid IP address '  + address[0]); return false; }
  }
   return true;
}

