<?php
	

	$msg = '';
	$alias = $_REQUEST['alias'];

	// check if form is submitted
	if (!empty($_POST)) {
		if (isset($_POST['yes'])) {
                        $alias = $_POST['alias'];
                        $at = strpos($alias, '@');
                        if($at) {
                                $alias_domain = substr($alias, $at + 1);
                                $alias_alias = substr($alias, 0, $at);
                                $alias_username = $_POST['username'] .'@'. $domain;
                        } else {
                                $alias_domain = $domain;
                                $alias_alias = $_POST['alias'];
                                $alias_username = $_POST['username'];
                        }
			if ($mail_server->aliasdel($alias_domain, $alias_alias, $alias_username)) {
				$msg = "<img src=gfx/ico_check.gif> Alias <b>$alias_alias@$alias_domain</b> deleted successfully.";
			} else {
				$msg = '<img src=gfx/ico_warn.gif> Error deleting alias: ' . $mail_server->xm_err_msg;
			}

		} else {
			$msg = "<img src=gfx/ico_stop.gif> Alias <b>$alias@$domain</b> is <b>NOT</b> deleted";
		}

	}

?>

<?php if (empty($_POST)) { ?>

	<p>You are about to remove alias <?php echo $alias; ?></p>
	<p>Do you really want to remove this alias?</p>

	<form name='frmDelAlias' action='main.php' method='post'>
		<input type='hidden' name='action' value='aliasdel'>
		<input type='hidden' name='domain' value='<?php echo $domain; ?>'>
		<input type='hidden' name='username' value='<?php echo $username; ?>'>
		<input type='hidden' name='alias' value='<?php echo $alias; ?>'>
		<input type='submit' name='yes' style='width: 50px;' value=' Yes '>
		<input type='submit' name='no' style='width: 50px;' value=' No '>
	</form>

<?php } else { ?>

	<p><?php echo $msg; ?></p>

	<form action='main.php' method='post'>
		<input type='hidden' name='action' value='aliaslist'>
		<input type='hidden' name='domain' value='<?php echo $domain; ?>'>
		<input type='hidden' name='username' value='<?php echo $username; ?>'>
		<input type='submit' name='click' style='width: 50px;' value=' OK '>
	</form>

<?php } ?>
				
