<?php
require_once('Browser.php');
$browser = new Browser();
if( $browser->getBrowser() == Browser::BROWSER_FIREFOX && $browser->getVersion() >= 2 ) {
	echo 'You have FireFox version 2 or greater';
}
echo $browser;
?>
