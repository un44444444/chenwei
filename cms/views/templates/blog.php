<?php
header("Cache-Control: must-revalidate");
$offset = 60 * 60 * 24 * 10;
$ExpStr = "Expires: " . gmdate("D, d M Y H:i:s", time() + $offset) . " GMT";
header($ExpStr);
if (stristr($_SERVER["HTTP_ACCEPT"],"application/xhtml+xml"))
{
	header('Content-type: application/xhtml+xml; charset=utf-8');
	echo "".'<?xml version="1.0" encoding="utf-8"?>'."\n";
}
else
{
	header('Content-type: application/xml; charset=utf-8');
	echo "".'<?xml version="1.0" encoding="utf-8"?>'."\n";
	echo "".'<?xml-stylesheet type="text/xsl" href="/kohana/assets/copy.xsl"?>'."\n";
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML Basic 1.1//EN" "http://www.w3.org/TR/xhtml-basic/xhtml-basic11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en">
<head> 
	<title> chenw home </title> 
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" /> 
	<meta name="keywords" content="chenw" /> 
	<meta name="description" content="chenw home page" /> 
<style type="text/css"> 
	html,body {width:100%;height:100%;}
	* {margin:0;padding:0;}
	div.wide_row {width:100%;}
	div#header {height:60px;position:absolute;background:blue;}
	div#maincontent {height:auto;top:60px;bottom:56px;overflow:auto;position:absolute;background:#eee}
	div#sidebar {float:left;width:160px;height:100%;background:red}
	div#content {float:right;width:auto;height:100%;min-width:300px;left:160px;right:0px;position:absolute;background:yellow}
	div#footer {height:56px;bottom:0px;position:absolute;background:green}
</style> 
</head> 
<body> 
	<div id="header" class="wide_row"></div> 
	<div id="maincontent" class="wide_row"> 
<?php echo $content;?>
	</div> 
	<div id="footer" class="wide_row"></div> 
</body> 
</html>
