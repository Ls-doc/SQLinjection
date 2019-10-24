--TEST--
Check if sqreen is loaded
--SKIPIF--
<?php
if (!extension_loaded('sqreen')) {
	echo 'skip';
}
?>
--FILE--
<?php
echo 'The extension "sqreen" is available';
?>
--EXPECT--
The extension "sqreen" is available
