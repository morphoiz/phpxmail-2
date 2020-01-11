<?php

$msg = '';
//$mlname = $_REQUEST['mlname'];
$MailAddress = $_REQUEST['mailaddress'];

// check if form is submitted
if (!empty($_POST)) {
	if (isset($_POST['read'])) {$Read = $_POST['read'];}
	if (isset($_POST['write'])) {$Write = $_POST['write'];} else {$Write = '';}
	if (isset($_POST['auth'])) {$Write = str_replace('W', $_POST['auth'], $Write);}

	if ($Read .$Write == "") {
		//no rights were defined, so set them to the default...
		$Read='R';
		$Write='W';
		$msg = '<img src=gfx/ico_notice.gif> Rights were assigned to the default<br>';
	}

	$perms = $Read .$Write;
	if ($mail_server->mluserdel($domain, $mlname, $_POST['mailaddress'])){
		if ($mail_server->mluseradd($domain, $mlname, $_POST['mailaddress'], $perms)) {
			$msg .= "<img src=gfx/ico_check.gif> Address <b>$_POST[mailaddress]</b> was changed successfully.";
		} else {
			$msg = '<img src=gfx/ico_warn.gif> Error changing rights in address: ' . $mail_server->xm_err_msg;
		}
	} else {
		$msg = '<img src=gfx/ico_warn.gif> Error changing rights in address: ' . $mail_server->xm_err_msg;
	}
} else {
	$parm = $_REQUEST['parm'];
	if (strpos($parm, "R") === false){
		$Read = "";
	} else {
		$Read = "R";
	}
	if (strpos($parm, "W") === false){
		$Write = "";
	} else {
		$Write = "W";
	}
	if (strpos($parm, "A") === false){
	} else {
		$Write = "A";
	}
}

?>

<form name='frmEditUser' action='main.php' method='post'>
    <input type='hidden' name='action' value='mlusrchgedit'>
    <input type='hidden' name='domain' value='<?php echo $domain; ?>'>
    <input type='hidden' name='mlname' value='<?php echo $mlname; ?>'>
    <?php echo $msg; ?>
    <table>
        <tr>
            <td><b>Mail address:<b></td>
            <td><input type='text' name='mailaddress' readonly='yes' value='<?php echo $MailAddress; ?>' size='30' style='width: 400px;'></td>
        </tr>
        <tr>
            <td><b>Read permission:</b></td>
            <td><input type='checkbox' name='read' value='R'<?php if ($Read != "") echo ' checked';?>></td>
        </tr>
        <tr>
            <td><b>Write permission:</b></td>
            <td><input type='checkbox' name='write' id='write' value='W'<?php if ($Write != "") echo ' checked';?> onclick="WriteClicked()"></td>
        </tr>
        <tr>
            <td><B>Check done using: </B></td>
            <td><select name="auth" id="auth" <?php if ($Write == "") echo ' disabled';?> style='width: 400px;'>
                	<option value="W" <?php if ($Write == "W") echo ' selected';?>>e-mail address in &lt;FROM:...&gt; return path</OPTION>
                	<option value="A" <?php if ($Write == "A") echo ' selected';?>>e-mail address used in SMTP client authentication</OPTION>
                </select></td>
        </tr>
        <tr>
            <td colspan="2" align="right"><input type='submit' value=' submit '></td>
        </tr>
    </table>
</form>
<hr>
<br><br>
<script type="text/javascript"><!--
function WriteClicked() {
	Chk1 = document.getElementById("write");
	List1 = document.getElementById("auth");
	List1.disabled = !Chk1.checked ;
}
//-->
</script>
