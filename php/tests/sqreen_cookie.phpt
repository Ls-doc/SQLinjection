--TEST--
sqreen SQLi Basic test COOKIE
--SKIPIF--
<?php
if (!extension_loaded('sqreen')) {
	echo 'skip';
}
?>
--COOKIE--
user='admin' or 1=1--
--FILE--
<?php
var_dump($_COOKIE["user"]);
?>
--EXPECT--
string(16) "'admin' or 1=1--"
