<?php

$HOST = "192.168.137.200";
$PORT = 80;
$PATH = "/weatherpi/html/station.php";
$TARGETURL = sprintf("http://%s:%d%s", $HOST, $PORT, $PATH);

$files = array(
  "../rrdtool/graph00.png"
, "../rrdtool/graph01.png"
, "../rrdtool/graph02.png"
, "../rrdtool/graph03.png"
, "../rrdtool/graph04.png"
);

for($i=0; $i < count($files); ++$i)
{
	$filename = $files[$i];

	if(!file_exists($filename))
		continue;

	$filetype = pathinfo($filename, PATHINFO_EXTENSION);
	$filedata = file_get_contents($filename);
	$filebase64 = 'data:image/' . $filetype . ';base64,' . base64_encode($filedata);

	$json = sprintf('{"Name":"%s","Data":"%s"}', basename($filename), $filebase64);
	$data = array('data' => $json);
	$options = array(
    		'http' => array(
    	    'header'  => "Content-type: application/x-www-form-urlencoded\r\n",
    	    'method'  => 'POST',
    	    'content' => http_build_query($data),
    	),
	);
	$context  = stream_context_create($options);
	$result = file_get_contents($TARGETURL, false, $context);

	ob_start();
	var_dump($result);
	$r = ob_get_clean();
	$r .= "\n\n" . date('Y-m-d H:i:s') . "\n";
	file_put_contents("station.push.log", $r);
}

?>
