--TEST--
sqreen SQLi Basic test POST
--SKIPIF--
<?php
if (!extension_loaded('sqreen')) {
	echo 'skip';
}
?>
--POST--
user='administrator' or --
--FILE--
<?php
var_dump($_POST["user"]);
?>
--EXPECT--
string(21) "'administrator' or --"
