--TEST--
sqreen SQLi Basic test COOKIE
--SKIPIF--
<?php
if (!extension_loaded('sqreen')) {
	echo 'skip';
}
?>
--COOKIE--
user='administrator' or 4=4--
--FILE--
<?php
var_dump($_COOKIE["user"]);
?>
--EXPECT--
string(24) "'administrator' or 4=4--"
