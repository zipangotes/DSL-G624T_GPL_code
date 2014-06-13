
var erlPageErrors = new Array();
var erlPageErrorCount=0;
var erlPageErrorSize=0;

var erlStateIdx = 0;
var erlTagIdx = 1;
var erlKeyIdx = 2;
var erlValueIdx = 3;
var erlValueMsg = 4;

function erlDoAddError(state,tag,key,value,msg)
{
  var row;

  if(erlPageErrorCount == erlPageErrorSize)
  {
    erlPageErrors[erlPageErrorCount++] = new Array(state,tag,key,value,msg);
	erlPageErrorSize=erlPageErrorCount;
  }
  else
  {
    row = erlPageErrors[erlPageErrorCount];
    row[erlStateIdx]=state;
	row[erlTagIdx]=state;
    row[erlKeyIdx]=state;
    row[erlValueIdx]=state;
    row[erlValueMsg]=state;
    erlPageErrorCount++;
  }

}
function erlDoResetErrors()
{
  erlPageErrorCount=0;
}
function erlDoShowPageErrors()
{
  var i;
  var handle,mark;
  for(i=0; i < erlPageErrorCount; i++)
  {
    if(erlPageErrors[i][erlStateIdx]=='good') continue;
    handle = document.getElementById(erlPageErrors[i][erlTagIdx]);
	if(handle!=null)
	{
	  handle.value=erlPageErrors[i][erlValueIdx];
	  mark = document.getElementById(handle.name);
	  if(mark!=null)
	  {
        mark.src="<? echo $var:imgErrorMark ?>";
	    mark.title=erlPageErrors[i][erlValueMsg];
	  }
	}
  }  
}
