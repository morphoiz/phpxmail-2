<?php

require_once('class.xmail.php');
require_once('config.php');
require_once('environment.inc');
session_start();

if ($cfg_debug_stat == "on") {
	error_reporting(E_ALL);
}

if (isset($_SESSION['mail_server']))
$mail_server = $_SESSION['mail_server'];
else      $mail_server = new XMail();
?>

<html>
<head>
    <title></title>
    <link rel="stylesheet" href="main.css" type="text/css">
	<base target='main'>
</head>

<body class="menu">

<img src='gfx/phpx.gif' alt='phpxmail' border='0'>

<div class='content'>

<?php

echo "<div align=center><b>PHPXmail&nbsp;<a href=http://nextcode.org/forums/ target=_blank>support</a><br>";

echo "Version: $cfg_version_num";

if ($cfg_update_chk == "on"){
	$available = file ('http://phpxmail.sourceforge.net/version.txt');
	$installed = $cfg_version_num;
	if ($available){
		if ($installed < rtrim($available[0])) {
			echo "&nbsp<img src=gfx/ico_warn.gif align=absmiddle>&nbsp<a href='main.php?action=update'> update </a></b>";
		}
	}
} else {
	echo "&nbsp<a href='main.php?action=update'> update </a>";
}

echo "</div>\n";

if (isset($_SESSION['logged_in'])){
	if ($_SESSION['logged_in'] == "The operation completed successfully.") {

		if ($mail_server->noop()){
			echo "<div align=center><br><b>Xmail server: " .$mail_server->xm_ver ."</b></div><br><br>";
		}

		switch ($mail_server->xm_ctrl_t) {
			case 'r_user': // user menu
			echo "<a href='main.php?action=userchange'><div class='menulink'>change settings</div></a>";
			break;

			case 'd_admin': // domain admin menu
			echo "<a href='main.php?action=userlist'><div class='menulink'>users</div></a>";
			break;

			case 's_admin': // server admin menu
			echo "<a href='main.php?action=servervars'><div class='menulink'>server config</div></a>\n";
			echo "<a href='main.php?action=domainlist'><div class='menulink'>server domains</div></a>\n";
			echo "<a href='main.php?action=serverfilt'><div class='menulink'>server filters</div></a>\n";
			echo "<a href='main.php?action=spamlist'><div class='menulink'>server spam lists</div></a>\n";
			echo "<a href='main.php?action=serverlogs'><div class='menulink'>server logs</div></a>\n";
			echo "<a href='main.php?action=frozlist'><div class='menulink'>server spool</div></a>\n";
			echo "<br>\n";
			echo "<a href='main.php?action=config'><div class='menulink'>phpxmail config</div></a>\n";
			echo "<a href='main.php?action=serverlist'><div class='menulink'>phpxmail setup</div></a>\n";
			break;
		}

		echo "<br>\n";
		echo "<a href='main.php?action=logout'><div class='menulink'>logout</div></a><br>\n";

	}  else { // show login

	echo "<br>\n";
	echo "<a href='main.php?action=login'><div class='menulink'>login</div></a><br>\n";

	}
} else {
	echo "<br>\n";
	// show login (if was set almost one server)
	if (count(serverlist($cfg_servers))){
		echo "<a href='main.php?action=login'><div class='menulink'>login</div></a><br>\n";

		//show register
		if ($cfg_visitor_reg == "on"){
			echo "<a href='main.php?action=register'><div class='menulink'>register</div></a><br>";
		}
	}
}
if (count(serverlist($cfg_servers)) == 0){
	echo "<a href='main.php?action=serverlist'><div class='menulink'>setup</div></a><br>";
}

echo "<a href='main.php?action=legend'><div class='menulink'>legend</div></a><br>";

?>

</div>

</body>
</html>