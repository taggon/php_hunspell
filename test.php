<?php
$dir = dirname(__FILE__);
$spell = hunspell_create($dir.'/en_US.aff', $dir.'/en_US.dic');

$arr = hunspell_suggest($spell, 'tets');
print_r($arr);

hunspell_destroy($spell);
