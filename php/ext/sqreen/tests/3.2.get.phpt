--TEST--
sqreen SQLi Basic test GET
--SKIPIF--
<?php
if (!extension_loaded('sqreen')) {
	echo 'skip';
}
?>
--GET--
user='administrator' or --
--FILE--
<?php
var_dump($_GET["user"]);
?>
--EXPECT--
string(21) "'administrator' or --"
