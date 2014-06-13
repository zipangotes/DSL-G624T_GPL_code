
<? setvariable var:topmenuhandlers `onmouseover="jslDoMenuRollOver('top',id)" onmouseout="jslDoMenuRollOut('top')"` ?>
<? setvariable var:leftmenuhandlers `onmouseover="jslDoMenuRollOver('left',id)" onmouseout="jslDoMenuRollOut('left')"` ?>

<script language="JavaScript">
var uiMenu = new Array();

<? include /usr/www_safe/html/defs/$var:style/menus/<? echo $var:main ?>_top.js ?>
<? include /usr/www_safe/html/defs/$var:style/menus/<? echo $var:main ?>_$var:menu.js ?>

function jslDoMenuRollOver(menu,item_id)
{
  var node,id;
  for(node=uiMenu[menu];node!=null;node=uiMenu[node["next"]])
  {
    id=node["id"];
    if(id==null) continue;
    node["btn_cur"]=document.getElementById(id).src;
    if(id==item_id) document.getElementById(id).src=node["btn_on"];
     else document.getElementById(id).src=node["btn_off"];
  }
}

function jslDoMenuRollOut(menu)
{
  var node,id;
  for(node=uiMenu[menu];node!=null;node=uiMenu[node["next"]])
  {
    id=node["id"];
    if(id==null) continue;
    document.getElementById(id).src=node["btn_cur"];
  }
}
</script>
