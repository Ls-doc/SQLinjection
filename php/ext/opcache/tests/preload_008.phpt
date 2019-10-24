--TEST--
Preloading of anonymous class
--INI--
opcache.enable=1
opcache.enable_cli=1
opcache.optimization_level=-1
opcache.preload={PWD}/preload.inc
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
var_dump(get_anon());
?>
--EXPECT--
object(class@anonymous)#1 (0) {
}
