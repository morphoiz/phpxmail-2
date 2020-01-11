<?php
	
	require('class.xmail.php');
	session_start();
	$mail_server = $_SESSION['mail_server'];

	if ($cfg_debug_stat == "on") {
	error_reporting(E_ALL);
	}

	$user = $_REQUEST['user'];
	$server = $_REQUEST['server'];
	$ssh_success = test_ssh($user, $server);

?>

<html>
<head>
	<title>SSH check</title>
	<link rel="stylesheet" href="gfx/main.css" type="text/css">
	<script>window.resizeTo(400,150)</script>
</head>
<body>

<div class='content'>
<?php if ($ssh_success) { ?>
	Connection to <?php echo $server; ?> was a success<br>
<?php } else { ?>
	Could not make a connection to <?php echo $server; ?><br>
	<li>check if <?php echo $server; ?> is in known_host on <?php echo $_SERVER["SERVER_NAME"]; ?>
	<li>check if <?php echo $user; ?> is in authorized_keys <?php echo $server; ?><br>
<?php } ?>
<a href="#" onclick="window.close()">close window</a>
</div>
</body>
</html>