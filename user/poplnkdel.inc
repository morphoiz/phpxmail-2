<?php
	

	$msg = '';
	$p_username = $_REQUEST['p_username'];
	$p_domain = $_REQUEST['p_domain'];

	// check if form is submitted
	if (!empty($_POST)) {
		if (isset($_POST['yes'])) {

			if ($mail_server->poplnkdel($domain, $username, $p_domain, $p_username)) {
				$msg = "<img src=gfx/ico_check.gif> POP3 link <b>$p_username@$p_domain</b> deleted successfully.";
			} else {
				$msg = '<img src=gfx/ico_warn.gif> Error deleting POP3 link: ' . $mail_server->xm_err_msg;
			}

		} else {
			$msg = "<img src=gfx/ico_stop.gif> POP3 link <b>$p_username@$p_domain</b> is <b>NOT</b> deleted";
		}

	}

?>

<?php if (empty($_POST)) { ?>

	<p>You are about to remove POP3 link <?php echo "$p_username@$p_domain"; ?></p>
	<p>Do you really want to remove this link?</p>

	<form name='frmDelPop3' action='main.php' method='post'>
		<input type='hidden' name='action' value='poplnkdel'>
		<input type='hidden' name='domain' value='<?php echo $domain; ?>'>
		<input type='hidden' name='username' value='<?php echo $username; ?>'>
		<input type='hidden' name='p_domain' value='<?php echo $p_domain; ?>'>
		<input type='hidden' name='p_username' value='<?php echo $p_username; ?>'>
		<input type='submit' name='yes' style='width: 50px;' value=' Yes '>
		<input type='submit' name='no' style='width: 50px;' value=' No '>
	</form>

<?php } else { ?>

	<p><?php echo $msg; ?></p>

	<form action='main.php' method='post'>
		<input type='hidden' name='action' value='poplnklist'>
		<input type='hidden' name='domain' value='<?php echo $domain; ?>'>
		<input type='hidden' name='username' value='<?php echo $username; ?>'>
		<input type='submit' name='click' style='width: 50px;' value=' OK '>
	</form>

<?php } ?>
				