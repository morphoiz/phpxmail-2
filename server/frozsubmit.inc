<?php
	

	// check if form is submitted
	if (!empty($_POST)) {
		if (isset($_POST['yes'])) {
			if ($mail_server->frozsubmit($_REQUEST['del_froz_msgfile'], $_REQUEST['del_froz_lev0'], $_REQUEST['del_froz_lev1'])) {
				$msg = "<img src=gfx/ico_check.gif> Frozen message <b></b> resubmitted successfully.";
			} else {
				$msg = '<img src=gfx/ico_warn.gif> Error resubmitting message: ' . $mail_server->xm_err_msg;
			}
		} else {
			$msg = "<img src=gfx/ico_stop.gif> Frozen message <b></b> has <b>NOT</b> been resubmitted";
		}
	} else {
		$frozmsg = explode(":", $_REQUEST['sel_frozmsg']);
	}

?>

<?php if (empty($_POST)) { ?>

	<p>You are about to resubmit the frozen msg <b><?php echo "$frozmsg[0], $frozmsg[1], $frozmsg[2]"; ?></b></p>
	<p>Do you really want to resubmit this frozen msg?</p>

	<form action='main.php' method='post'>
		<input type='hidden' name='action' value='frozsubmit'>
		<input type='hidden' name='del_froz_msgfile' value='<?php echo $frozmsg[0]; ?>'>
		<input type='hidden' name='del_froz_lev0' value='<?php echo $frozmsg[1]; ?>'>
		<input type='hidden' name='del_froz_lev1' value='<?php echo $frozmsg[2]; ?>'>
		<input type='submit' name='yes' style='width: 50px;' value=' Yes '>
		<input type='submit' name='no' style='width: 50px;' value=' No '>
	</form>

<?php } else { ?>

	<p><?php echo $msg; ?></p>

	<form action='main.php' method='post'>
		<input type='hidden' name='action' value='frozlist'>
		<input type='submit' name='click' style='width: 50px;' value=' OK '>
	</form>

<?php } ?>
