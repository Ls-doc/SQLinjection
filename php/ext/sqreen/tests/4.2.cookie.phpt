--TEST--
sqreen SQLi Basic test COOKIE
--SKIPIF--
<?php
if (!extension_loaded('sqreen')) {
	echo 'skip';
}
?>
--COOKIE--
user='administrator' or --
--FILE--
<?php
var_dump($_COOKIE["user"]);
?>
--EXPECT--
string(21) "'administrator' or --"
