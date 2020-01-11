<?php

$msg = '';
$vars = '';
$ExcludedParms = "SmtpPerms, DisableEmitAuthUser, PopEnable, RealName, HomePage, Address, Telephone";
$username = $_REQUEST['mlname'];

// check if form is submitted
if (!empty($_POST)) {
	// change password if needed
	if ($_POST['password'] != '') {
		if ($mail_server->userpasswd($domain, $username, $_POST['password'])) {
			$msg = "<img src=gfx/ico_check.gif> Password for <b>$username@$domain</b> changed successfully.<br>";
		} else {
			$msg = '<img src=gfx/ico_warn.gif> Error changing password: ' . $mail_server->xm_err_msg;
		}
	}
	// save user vars
	foreach($_POST as $var=>$val) { // loop through all vars
	if (preg_match("var_", $var)) {
		$var = substr($var, 4);
		if ($val == '') $val = '.|rm'; // remove value
		$vars .= "$var\t$val\t";
	}
	}
	if ($_POST['xtr_var'] != '' && $_POST['xtr_value'] != '')
	$vars .= "$_POST[xtr_var]\t$_POST[xtr_value]\t";
	if ($mail_server->uservarsset($domain, $username, $vars)) {
		$msg .= "<img src=gfx/ico_check.gif> Values for <b>$username@$domain</b> saved successfully";
	} else {
		$msg .= '<img src=gfx/ico_warn.gif> Error saving values: ' . $mail_server->xm_err_msg;
	}

} 

$usr = $mail_server->userlist($domain, $username);
$password = $usr[0][2];
$uservars = $mail_server->uservars($domain, $username);
?>

<form name='frmUserDetails' action='main.php' method='post'>
    <input type='hidden' name='action' value='mluservars'>
    <input type='hidden' name='domain' value='<?php echo $domain; ?>'>
    <input type='hidden' name='mlname' value='<?php echo $username; ?>'>
    <input type='hidden' name='ch_pass' value=''>
    <?php echo $msg; ?>
    <table border='0' class='tbl'>
        <tr>
            <th width="150"></th>
            <th width="*"></th>
        </tr>
        <tr>
            <td><b>Username<b></td>
            <td><input type='text' name='username' readonly='yes' size='30' style='width: 400px;' value='<?php echo $username; ?>'></td>
        </tr>
        <tr>
            <td><b>Password*</b></td>
            <td><input type='password' name='password' size='30' style='width: 400px;' value=''></td>
        </tr>
        <?php 
        foreach ($uservars as $key => $value) {
        	if (strpos($ExcludedParms, $key) === false) {?>
		    <tr>
		        <td><b><?php echo $key; ?><b></td>
		        <td><input type='text' name='var_<?php echo $key; ?>' size='30' style='width: 400px;' value='<?php echo $value; ?>'></td>
		    </tr>
        <?php }} ?>
        <tr>
            <td><input type='text' name='xtr_var' size='30' style='width: 150px;'></td>
            <td><input type='text' name='xtr_value' size='30' style='width: 400px;'></td>
        </tr>
        <tr>
            <td colspan="2" align="right"><input type='submit' name='save' value=' Save values '></td>
        </tr>
    </table>
</form>

<hr>
<img src=gfx/ico_info.gif align=absmiddle title=info> Password is not shown for security reasons. Leave blank to keep old password.<br>
<hr>
<br><br>
