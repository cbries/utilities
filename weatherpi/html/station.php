<?php

if(!isset($_POST["data"]))
	die('{"error":"no weather data"}');

$jsonObject = json_decode($_POST["data"], true);
if($jsonObject == null)
	die('{"error":"weather data not valid"}');

$ImageName = $jsonObject["Name"];
$ImageData = $jsonObject["Data"];

function base64_to_jpeg($base64_string, $output_file) {
	$ifp = fopen($output_file, "wb"); 
	$data = explode(',', $base64_string);
	fwrite($ifp, base64_decode($data[1])); 
	fclose($ifp);
	return $output_file; 
}

echo base64_to_jpeg($ImageData, "data/" . $ImageName);

?>
