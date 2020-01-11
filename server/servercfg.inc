<?php

$servers = serverlist();
$svr = $_REQUEST['svr'];
$msg = '';

$CTRLerror = false;
// check if form is submitted
if (isset($_POST['address']) && isset($_POST['prt']) && isset($_POST['ctrl_user']) && isset($_POST['ctrl_pass'])) {
	if (!strstr($_POST['ctrl_user'], '@')) {
		if ($_POST['ctrl_pass'] == '') { // use old password
		$ctrl_pass = $servers[$svr][3];
		} else {
			$ctrl_pass = xmcrypt($_POST['ctrl_pass']);
		}
		$use_ssh = (isset($_POST['use_ssh'])) ? 1 : 0;
		if ($use_ssh && !test_ssh($_POST['ctrl_user'], $_POST['address'])) {
			$msg  = "SSH FAILED!!!<br>";
			$msg .= "Server <b>$svr</b> not changed.<br>";
		} else {
			$servers[$svr] = array($_POST['address'], $_POST['prt'], $_POST['ctrl_user'], $ctrl_pass, $use_ssh);
			if (serverwrite($servers)) {
				$msg = "<img src=gfx/ico_check.gif> Server <b>$svr</b> changed successfully.<br>";
			} else {
				$msg = "<img src=gfx/ico_warn.gif> Error saving server: " . $mail_server->xm_err_msg .'<br>';
			}
		}
	} else {
		$CTRLerror = true;
	}
}

$servers = serverlist();
if ($CTRLerror || strstr($servers[$svr][2], '@')) {
	$msg .= "<img src=gfx/ico_warn.gif> E-mail address in 'CTRL account' is not allowed. (Check also ctrlaccounts.tab: in case, fix it)</p>";
	$FntColored = '<font color="red">';
} else {
	$FntColored = '';
}

?>

<form name='frmServerCfg' action='main.php' method='post'>
        <input type='hidden' name='action' value='servercfg'>
        <input type='hidden' name='svr' value='<?php echo $svr; ?>'>
        <?php echo $msg; ?>
        <table>
                <tr>
                        <td><b>Server name:<b>
                        <td><?php echo $svr; ?>
                <tr>
                        <td><b>Server address:</b>
                        <td><input type='text' name='address' size='30' style='width: 400px;' value="<?php echo $servers[$svr][0]; ?>">
                <tr>
                        <td><b>Server port:</b>
                        <td><input type='text' name='prt' size='30' style='width: 400px;' value="<?php echo $servers[$svr][1]; ?>">
                <tr>
                        <td><?php echo $FntColored ?><b>CTRL account:</b></font></td>
                        <td><input type='text' name='ctrl_user' size='30' style='width: 400px;' value="<?php echo $servers[$svr][2]; ?>">
                <tr>
                        <td><b>CTRL pass: *</b>
                        <td><input type='text' name='ctrl_pass' size='30' style='width: 400px;' value="">
<!--
                <tr>
                        <td><b>Use SSH: **</b>
                        <td>
                                <input type='checkbox' name='use_ssh' value="1" <?php if ($servers[$svr][4]) echo 'checked'; ?>>
                                <input type='button' value=' test SSH ' onclick="window.open('checkssh.php?user=<?php echo $servers[$svr][2]; ?>&server=<?php echo $servers[$svr][0]; ?>', null, null)">
                                (could crash session!!!)
-->
                <tr>
                        <td colspan="2" align="right">
                                <input type='submit' value=' submit '>
        </table>
</form>
<hr>
<img src=gfx/ico_info.gif align=absmiddle title=info> * CTRL pass is not shown for security reasons. Leave blank to keep old password.<br>
<!--** SSH must be manually setup at <?php echo $svr; ?> before you can use this!!!<br>-->
<hr>
<br><br>