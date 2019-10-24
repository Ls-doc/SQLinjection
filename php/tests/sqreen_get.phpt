--TEST--
sqreen SQLi Basic test GET
--SKIPIF--
<?php
if (!extension_loaded('sqreen')) {
	echo 'skip';
}
?>
--GET--
user='admin' or 1=1--
--FILE--
<?php
var_dump($_GET["user"]);
?>
--EXPECT--
string(16) "'admin' or 1=1--"
