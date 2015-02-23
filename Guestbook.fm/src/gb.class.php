<?php
/*
 * Copyright (c) 2009 Christian Benjamin Ries
 * Website: https://github.com/cbries/utilities
 * License: MIT
 */
?>
<?php
if(!defined("gb.class.php")) {
	define("gb.class.php",1);

require_once("crmysqli.class.php");
require_once("captcha.class.php");

class GB {
	var $databaseConnection;
	var $errorMessage;
	
	/**
	 * 
	 * @param $db
	 */
	function __construct($db) {
		$this->databaseConnection = $db;
		$errorMessage = null;
	}
	
	/**
	 * Verifies the input of the user.
	 * @param $data Assoc array of the input.
	 * @return true if every input is valid and cause no impact to business logic, otherwise false.
	 */
	function verify($data) {
		global $_SESSION;
		
		$this->errorMessage = "";
		
		$p = $data;
		$s = $_SESSION;
		
		if(	$s['captchares'] != $p['captchares'] ||
			empty($p['captchares'])
		   ) {
			$this->errorMessage .= " - Check the captcha entry.<br>";
		} 
		if(empty($p['username']) || strlen($p['username'])<5) {
			$this->errorMessage .= " - Add a username or more than 5 letter.<br>";
		} 
		$m = $p['mail'];
		if(empty($m) || !isValidEmail($m)) {
			$this->errorMessage .= " - Add a mail address.<br>";
		} 
		if(empty($p['subject']) || strlen($p['subject'])<10) {
			$this->errorMessage .= " - Add a subject or more than 10 letter.<br>";
		} 
		if(empty($p['message']) || strlen($p['message'])<10) {
			$this->errorMessage .= " - Add a message text or more than 10 letter.<br>";
		} 
		
		if(!empty($this->errorMessage)) {
			$_SESSION['frm']['username'] = $p['username'];
			$_SESSION['frm']['mail'] = $p['mail'];
			$_SESSION['frm']['subject'] = $p['subject'];
			$_SESSION['frm']['message'] = $p['message'];
			
			$captcha = new Captcha();
			$captcha->init();
			
			return false; 
		}
		return true;
	}
	
	/**
	 * 
	 * @param $data 
	 * 	['username']
	 *  ['mail']
	 *  ['subject']
	 *  ['message']
	 * return Increment id of the new row.
	 */
	function add($data) {
		$onlyFields = array("username"=>"", "mail"=>"", "subject"=>"", "message"=>"");
		
		$f = "";	// fields
		$v = "";	// values
		foreach($data as $key => $value) {			
			if(array_key_exists($key,$onlyFields)) {
				$f .= $key.",";
				if(is_string($value)) {
					$v .= "'".strip_tags($value)."',";
				} else {
					$v .= $value.",";
				}
			}
		}
		
		$f = substr($f,0,strlen($f)-1);
		$v = substr($v,0,strlen($v)-1);
		
		$sql = "INSERT INTO gb(".$f.") VALUES (".$v.")";		

		$captcha = new Captcha();
		$captcha->init();
		
		return $this->databaseConnection->q($sql);
	}
	
	/**
	 * 
	 * @param $id
	 * @return true if deleted, otherwise false.
	 */
	function delete($id) {
		return $this->databaseConnection->q("DELETE FROM gb WHERE id=" . $id . " LIMIT 1");
	}
	
	/**
	 * Returns a list of all entries of one page.
	 * @param $pagenumber
	 * @param $entries
	 * @return Array
	 */
	function get($pagenumber, $entries) {
		$sql = "SELECT * FROM gb ORDER BY ts DESC LIMIT "
			.(($pagenumber-1)*$entries).", ".$entries;
			
		$list = array();
		if($this->databaseConnection->q($sql)) {
			while($r=$this->databaseConnection->assoc()) {
				$list[] = $r;
			}
		}
		return $list;
	}
	
	/**
	 * 
	 * @return string of error
	 */
	function getErrorMessage() {
		return $this->errorMessage;
	}
}

} // gb.class.php
?>