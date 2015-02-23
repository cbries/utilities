#!/usr/local/sbin/php
<?php
if( $argc <= 1 ) {
	printf("Usage: ".$argv[1]." ip portstart portend\n");
	exit(0);
}

printf("portscan active:\n");
for( $i=$argv[2]; $i<=$argv[3]; $i++ ) {
	$r = @fsockopen( $argv[1], $i);
	if($r) {
		print "port ".$i." is open!\n";
	}
}
printf("portscan finished\n");
exit(0);
?>
