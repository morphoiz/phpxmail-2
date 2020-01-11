<?php

$msg = '';
$mlname = $_REQUEST['mlname'];

// check if form is submitted
if (!empty($_POST)) {
	$perms = '';
	if (isset($_POST['read'])) {$Read = $_POST['read'];}
	if (isset($_POST['write'])) {$Write = $_POST['write'];} else {$Write = '';}
	if (isset($_POST['auth'])) {$Write = str_replace('W', 'A', $Write);}
	
	if ($Read .$Write == "") {
		//no rights were defined, so set them to the default...
		$Read='R';
		$Write='W';
		$msg = '<img src=gfx/ico_notice.gif> Rights were assigned to the default<br>';
	}
	
	$perms = $Read .$Write;
	if ($mail_server->mluseradd($domain, $mlname, $_POST['mailaddress'], $perms)) {
		$msg .= "<img src=gfx/ico_check.gif> Address <b>$_POST[mailaddress]</b> was added successfully to <b>$mlname@$domain</b>.";
	} else {
		$msg .= '<img src=gfx/ico_warn.gif> Error adding address: ' . $mail_server->xm_err_msg;
	}
}

?>

<form name='frmNewUser' action='main.php' method='post'>
    <input type='hidden' name='action' value='mlusrchgadd'>
    <input type='hidden' name='domain' value='<?php echo $domain; ?>'>
    <input type='hidden' name='mlname' value='<?php echo $mlname; ?>'>
    <?php echo $msg; ?>
    <table>
        <tr>
            <td><b>Mail address:<b></td>
            <td><input type='text' name='mailaddress' size='30' style='width: 400px;'></td>
        </tr>
        <tr>
            <td><b>Read permission</b></td>
            <td><input type='checkbox' name='read' value='R' checked></td>
        </tr>
        <tr>
            <td><b>Write permission</b></td>
            <td><input type='checkbox' name='write' value='W'>&nbsp;(use Auth SMTP validation <input type='checkbox' name='auth' value='A'>)</td>
        </tr>
        <tr>
            <td colspan="2" align="right"><input type='submit' value=' submit '></td>
        </tr>
    </table>
</form>
<hr>
<br><br>

<!-- Set focus on 1st field... -->
<script type="text/javascript"><!--
document.frmNewUser.mailaddress.focus();
//-->
</script>
