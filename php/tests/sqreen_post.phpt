--TEST--
sqreen SQLi Basic test POST
--SKIPIF--
<?php
if (!extension_loaded('sqreen')) {
	echo 'skip';
}
?>
--POST--
user='admin' or 1=1--
--FILE--
<?php
var_dump($_POST["user"]);
?>
--EXPECT--
string(16) "'admin' or 1=1--"
