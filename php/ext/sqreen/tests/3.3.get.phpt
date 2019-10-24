--TEST--
sqreen SQLi Basic test GET
--SKIPIF--
<?php
if (!extension_loaded('sqreen')) {
	echo 'skip';
}
?>
--GET--
user='administrator' or 3=3--
--FILE--
<?php
var_dump($_GET["user"]);
?>
--EXPECT--
string(24) "'administrator' or 3=3--"
