<?php
/*
 * Copyright (c) 2009 Christian Benjamin Ries
 * Website: https://github.com/cbries/utilities
 * License: MIT
 */
?>
<?php
if(!defined("helper.func.php")) {
	define("helper.func.php", 1);
	
function getParameter($name) {
	global $_GET, $_POST;
	return (!isSet($_GET[$name])?$_POST[$name]:$_GET[$name]);
}

function getParameterDefault($name, $default) {
	global $_GET, $_POST;
	
	$var = $_GET[$name];
	if(!isSet($var)) {
		$var = $_POST[$name];
	} else {
		return $var;
	}
	if(!isSet($var)) {
		return $default;
	}
	return $var;
}

/**
 * Seems wrong :)
 * @param $email
 */
function isValidEmail($email) {
	$res = preg_match("/[a-zA-Z0-9_-][.+a-zA-Z0-9_-]+@[a-zA-Z0-9-]+.[a-zA-Z]+/", $email);
    if($res > 0) {
    	return true;
    } else {
		return false;
    }
}

}
?>