<?php


$svr = $_REQUEST['svr'];
$msg = '';

// check if form is submitted
if (!empty($_POST)) {
	if (isset($_POST['yes'])) {
		$servers = serverlist();
		unset($servers[$svr]); //remove unwanted server
		if (serverwrite($servers)) {
			$msg = "<img src=gfx/ico_check.gif> Server <b>$svr</b> deleted successfully.";
			// if there are no more servers then refresh the left menu pane
			if (count(serverlist($cfg_servers)) == 0){
				?>
				<script type="text/javascript"><!--
				parent.menu.location.reload();
				//-->
				</script>
				<?php
			}
		} else {
			$msg = '<img src=gfx/ico_warn.gif> Error deleting server: ' . $mail_server->xm_err_msg;
		}
	} else {
		$msg = "<img src=gfx/ico_stop.gif> Server <b>$svr</b> is <b>NOT</b> deleted";
	}
}

?>

<?php if (empty($_POST)) { ?>

	<p>You are about to remove XMail server <b><?php echo $svr; ?></b>.</p>
	<p>Do you really want to remove this server?</p>

	<form action='main.php' method='post'>
		<input type='hidden' name='action' value='serverdel'>
		<input type='hidden' name='svr' value='<?php echo $svr; ?>'>
		<input type='submit' name='yes' style='width: 50px;' value=' Yes '>
		<input type='submit' name='no' style='width: 50px;' value=' No '>
	</form>

<?php } else { ?>

	<p><?php echo $msg; ?></p>

	<form action='main.php' method='post'>
		<input type='hidden' name='action' value='serverlist'>
		<input type='submit' name='click' style='width: 50px;' value=' OK '>
	</form>

<?php } ?>
