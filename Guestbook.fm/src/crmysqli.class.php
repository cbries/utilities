<?php
/*
 * Copyright (c) 2009 Christian Benjamin Ries
 * Website: https://github.com/cbries/utilities
 * License: MIT
 */
?>
<?php
if(!defined("crmysqli.class.php")) {
	define("crmysqli.class.php",1);

class CRMySQLi {
		
		var $username;
		var $password;
		var $hostname;
		var $port;
		var $dbname;
		var $socket;
		var $dbConnection;
		var $query;
		var $res;
		var $rec;
		var $error;
		
		/**
		 * 
		 */
		function __construct() {			
			$this->setConnectionParameter("127.0.0.1", "");
		}

		/**
		 * 
		 * @param $hostname
		 * @param $dbname
		 * @param $username
		 * @param $password
		 * @param $port
		 * @param $socket
		 * @return MySQL connection link id or null.
		 */
		function setConnectionParameter(
				$hostname, 
				$dbname,
				$username = "root", 
				$password = "", 
				$port = 3306,
				$socket = "" ) 
		{
			$this->resetError();
			$this->hostname = $hostname;
			$this->dbname = $dbname;
			$this->username = $username;
			$this->password = $password;
			$this->port = $port;
			$this->socket = $socket;

			if($this->dbConnection==null) {
				$this->dbConnection = @mysqli_connect(
					$this->hostname,
					$this->username,
					$this->password,
					$this->dbname,
					$this->port,
					$this->socket
				);
				if($this->dbConnection==null) {
					$this->error = "Connection failed!";
					return false;
				}
			}
			return $this->dbConnection;
		}
		
		/**
		 * 
		 * @param $sql
		 * @param $link
		 * @return MySQLi Result set
		 */
		function q($sql, $link=null) {
			$this->resetError();
			if($link==null) {
				$link = $this->dbConnection;
			}
			$this->res = @mysqli_query($link, $sql);
			$this->query = $sql;
			if($this->res==null || $this->res==false) {
				$this->error=mysqli_error($link);
				return false;
			}
			return $this->res;
		}
		
		/**
		 * 
		 * @return MySQLi Record set
		 */
		function assoc() {
			$this->resetError();
			$this->rec = @mysqli_fetch_array($this->res, MYSQLI_ASSOC);
			return $this->rec;
		}
		
		/**
		 * 
		 * @param $result
		 * @return
		 */
        function getInfo($result) {
			$this->resetError();
        	return @mysqli_info($result) ;
        }
        
        /**
         * 
         * @return true/false
         */
        function isError() {
        	return (strlen($this->error)>0?true:false);
        }
        
        /**
         * 
         * @return string
         */
        function getError() {
        	return $this->error;
        }
        
        /**
         * 
         */
        function resetError() {
        	$this->error= "";
        }
}

} // crmysqli.class.php
?>