<?php
/*
 * Copyright (c) 2009 Christian Benjamin Ries
 * Website: https://github.com/cbries/utilities
 * License: MIT
 */
?>
<?php
	// The configuration.inc.php file should be outside 
	// of the htdocs directory. Under misconfiguration this
	// file can be readable.
	require_once("configuration.inc.php");
	require_once("crmysqli.class.php");
	require_once("gb.class.php");
	require_once("helper.func.php");
	
	session_start();
	
	$db = new CRMySQLi();
	$db->setConnectionParameter(
		$cfg['db']['hostname'],
		$cfg['db']['dbname'],
		$cfg['db']['username'],
		$cfg['db']['password']
	);
	if($db->isError()) {
		echo "Error: " . $db->getError() . "<br>";
	}
	
	$gb = new GB($db);
		
	/**
	 * Initializes test data and stores it in the database.
	 * @var testdata
	 * @web http://me/gb.php?init=yes 
	 */
	if(getParameter('init')) {
		$testdata = array();
		for($i=0; $i<30; $i++) {
			$testdata[] = array("username" => "root".$i,
								"mail" => "root".$i."@root.net",
								"subject" => "Testentry No.".$i,
								"message" => "Blubberblase");
			$gb->add($testdata[$i]);
		}
		echo "Testdata initialized: " . count($testdata) . "<br>";
	}	
	
	/**
	 * Checks the form data and stores them in the database if everything is fine.
	 */
	if(getParameter("gbaction")=="add") {
		$formularData = getParameter("frm");
		if($gb->verify($formularData)) {
			if($gb->add($formularData)) {
				echo "Saved!";
				unset($_SESSION['frm']);
				$_SESSION['frm'] = null;
			}
		} else {
			echo "Please check the following data:<br><br>";
			echo $gb->getErrorMessage()."<br>";
		}
	}
	
	/**
	 * Paging
	 */
	$pagestart = getParameterDefault("pagestart",1);
	$pagesize  = getParameterDefault("pagesize",10);
	
	if($pagestart<=0) {
		$pagestart = 1;
	}
	if($pagesize<=0) {
		$pagesize = 10;
	}
	
	/**
	 * Retrieves only entries in a selected page.
	 */
	$entries = $gb->get($pagestart, $pagesize);
	
	/**
	 * View!
	 */
	echo "Page: " . $pagestart . " / Entries: " . $pagesize . "<br>";
	echo "Count of entries: " . count($entries) . "<br>";
	
	include("input.php");
	
	echo('<b><a href="?pagestart='.($pagestart-1).'">Previous Page</a></b>');
	echo(' - ');
	echo('<b><a href="?pagestart='.($pagestart+1).'">Next Page</a></b><br><br>');	
	
	foreach($entries as $entry) {
		echo "<b>".htmlspecialchars(nl2br($entry['username']))."</b><br>";
		echo "<b>Mail: ".htmlspecialchars(nl2br($entry['mail']))."</b> - ".$entry['ts']."<br>";
		echo "-> ".htmlspecialchars(nl2br($entry['subject']))."<br>";
		echo "<div style='padding:10px; border:1px solid red; width:500px; font-size:12px;'>".htmlspecialchars(nl2br($entry['message']))."</div>";
		echo "<br>";
	}
?>