--TEST--
sqreen SQLi Basic test POST
--SKIPIF--
<?php
if (!extension_loaded('sqreen')) {
	echo 'skip';
}
?>
--POST--
user='administrator' or 2=2--
--FILE--
<?php
var_dump($_POST["user"]);
?>
--EXPECT--
string(24) "'administrator' or 2=2--"
