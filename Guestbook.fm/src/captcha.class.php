<?php
/*
 * Copyright (c) 2009 Christian Benjamin Ries
 * Website: https://github.com/cbries/utilities
 * License: MIT
 */
?>
<?php
if(!defined("captcha.class.php")) {
	define("captcha.class.php",1);
	
class Captcha {

	var $number1;
	var $number2;
	var $res;
	
	function __construct() {
		global $_SESSION;
		
		srand(microtime()*1000000);
		
		$number1 = 0;
		$number2 = 0;
		$res = 0;
		
		if(!isSet($_SESSION['captchares'])) {
			$this->init();
		} else {
			$this->number1 = $_SESSION['captchanumber1'];
			$this->number2 = $_SESSION['captchanumber2'];
			$this->res = $_SESSION['captchares'];
		}
	}
	
	/**
	 * Initializises the class default values.
	 */
	function init() {
		global $_SESSION;
		
		$this->number1 = rand(1,291);
		$this->number2 = rand(5,11258);
		$this->res= $this->number1 + $this->number2;
		
		$_SESSION['captchanumber1'] = $this->number1;
		$_SESSION['captchanumber2'] = $this->number2;
		$_SESSION['captchares'] = $this->res;
	}
	
	/**
	 * 
	 */
	function showImage() {
		$txt = $this->number1." + ".$this->number2." = ???";
		// Source: http://de2.php.net/imagecreate
		header("Content-Type: image/png");
		$im = @imagecreate(110, 25)
	    	or die("Cannot Initialize new GD image stream");
		$background_color = imagecolorallocate($im, 0, 0, 0);
		$text_color = imagecolorallocate($im, 240, 14, 91);
		imagestring($im, 2, 5, 5, $txt, $text_color);
		imagepng($im);
		imagedestroy($im);
	}	
}
	
} // captcha.class.php
?>