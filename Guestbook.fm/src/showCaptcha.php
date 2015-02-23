<?php
/*
 * Copyright (c) 2009 Christian Benjamin Ries
 * Website: https://github.com/cbries/utilities
 * License: MIT
 */
	require_once("captcha.class.php");

	session_start();
	
	$captcha = new Captcha();
	// Uncomment this part to create a new captcha value.
	//$captcha->init();
	$captcha->showImage();
?>
