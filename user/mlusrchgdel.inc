<?php
	

	$msg = '';
	$mlname = $_REQUEST['mlname'];
	$mailaddress = $_REQUEST['mailaddress'];

	// check if form is submitted
	if (!empty($_POST)) {
		if (isset($_POST['yes'])) {

			if ($mail_server->mluserdel($domain, $mlname, $mailaddress)) {
				$msg = "<img src=gfx/ico_check.gif> address <b>$mailaddress</b> removed successfully.";
			} else {
				$msg = '<img src=gfx/ico_warn.gif> Error removing address: ' . $mail_server->xm_err_msg;
			}

		} else {
			$msg = "<img src=gfx/ico_stop.gif> address <b>$mailaddress</b> is <b>NOT</b> removed";
		}

	}

?>

<?php if (empty($_POST)) { ?>

	<p>You are about to remove mailing list user <b><?php echo "$mailaddress"; ?></b>.</p>
	<p>Do you really want to remove this user?</p>

	<form action='main.php' method='post'>
		<input type='hidden' name='action' value='mlusrchgdel'>
		<input type='hidden' name='domain' value='<?php echo $domain; ?>'>
		<input type='hidden' name='mlname' value='<?php echo $mlname; ?>'>
		<input type='hidden' name='mailaddress' value='<?php echo $mailaddress; ?>'>
		<input type='submit' name='yes' style='width: 50px;' value=' Yes '>
		<input type='submit' name='no' style='width: 50px;' value=' No '>
	</form>

<?php } else { ?>

	<p><?php echo $msg; ?></p>

	<form action='main.php' method='post'>
		<input type='hidden' name='action' value='mlusrchglst'>
		<input type='hidden' name='domain' value='<?php echo $domain; ?>'>
		<input type='hidden' name='mlname' value='<?php echo $mlname; ?>'>
		<input type='submit' name='click' style='width: 50px;' value=' OK '>
	</form>

<?php } ?>